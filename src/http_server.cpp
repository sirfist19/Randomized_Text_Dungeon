#include "game_session.h"
#include "helper_fxns.h"
#include "third_party/cpp-httplib/httplib.h"
#include "third_party/nlohmann/json.hpp"

#include <atomic>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include <mutex>
#include <random>
#include <sstream>
#include <string>
#include <unordered_map>

using json = nlohmann::json;

namespace {

std::mutex g_mutex;
std::unordered_map<std::string, std::unique_ptr<GameSession>> g_sessions;

std::string make_session_id() {
	static std::atomic<uint64_t> seq{1};
	std::random_device rd;
	std::mt19937_64 gen(rd());
	std::stringstream ss;
	ss << std::hex << gen() << "-" << seq.fetch_add(1);
	return ss.str();
}

json step_result_to_json(const StepResult& r) {
	json j;
	j["phase"] = r.phase;
	j["awaitingInput"] = r.awaiting_input;
	j["sessionEnded"] = r.session_ended;
	j["gameWon"] = r.game_won;
	j["output"] = r.output;
	if (r.clearViewport) {
		j["clearViewport"] = true;
	}
	if (!r.error.empty()) {
		j["error"] = r.error;
	}
	return j;
}

} // namespace

int main(int argc, char* argv[]) {
	set_wait_fast(true);

	// Default 8765 — 8080 is often taken on macOS (e.g. AirPlay Receiver).
	const char* host = "0.0.0.0";
	int port = 8765;
	if (argc >= 2) {
		port = std::atoi(argv[1]);
	}

	httplib::Server svr;

	svr.Get("/health", [](const httplib::Request&, httplib::Response& res) {
		res.status = 200;
		res.set_content("{\"ok\":true}\n", "application/json");
	});

	svr.Post("/api/sessions", [](const httplib::Request& req, httplib::Response& res) {
		std::string content_root = "../content";
		int line_width = 0;
		try {
			if (!req.body.empty()) {
				json body = json::parse(req.body);
				if (body.contains("contentRoot") && body["contentRoot"].is_string()) {
					content_root = body["contentRoot"].get<std::string>();
				}
				if (body.contains("lineWidth") && body["lineWidth"].is_number_integer()) {
					line_width = body["lineWidth"].get<int>();
				}
			}
		} catch (...) {
			res.status = 400;
			res.set_content("{\"error\":\"invalid JSON\"}\n", "application/json");
			return;
		}

		std::string id = make_session_id();
		auto sess = std::make_unique<GameSession>(content_root, line_width);
		StepResult boot = sess->bootstrap();

		json out = step_result_to_json(boot);
		out["sessionId"] = id;

		std::lock_guard<std::mutex> lock(g_mutex);
		g_sessions[id] = std::move(sess);

		res.status = 201;
		res.set_content(out.dump(2) + "\n", "application/json");
	});

	svr.Post(R"(/api/sessions/([^/]+)/step)", [](const httplib::Request& req, httplib::Response& res) {
		std::string id = req.matches[1];
		json body;
		try {
			body = json::parse(req.body.empty() ? "{}" : req.body);
		} catch (...) {
			res.status = 400;
			res.set_content("{\"error\":\"invalid JSON\"}\n", "application/json");
			return;
		}

		std::string line;
		int line_width = 0;
		if (body.contains("line") && body["line"].is_string()) {
			line = body["line"].get<std::string>();
		}
		if (body.contains("lineWidth") && body["lineWidth"].is_number_integer()) {
			line_width = body["lineWidth"].get<int>();
		}

		std::lock_guard<std::mutex> lock(g_mutex);
		auto it = g_sessions.find(id);
		if (it == g_sessions.end()) {
			res.status = 404;
			res.set_content("{\"error\":\"session not found\"}\n", "application/json");
			return;
		}

		if (line_width > 0) {
			it->second->set_line_width(line_width);
		}
		StepResult r = it->second->step(line);
		res.status = 200;
		res.set_content(step_result_to_json(r).dump(2) + "\n", "application/json");
	});

	svr.Delete(R"(/api/sessions/([^/]+))", [](const httplib::Request& req, httplib::Response& res) {
		std::string id = req.matches[1];
		std::lock_guard<std::mutex> lock(g_mutex);
		if (g_sessions.erase(id) == 0) {
			res.status = 404;
			res.set_content("{\"error\":\"session not found\"}\n", "application/json");
			return;
		}
		res.status = 204;
	});

	std::cout << "Listening on http://" << host << ":" << port << "\n";
	if (!svr.listen(host, port)) {
		std::cerr << "Failed to listen";
		if (errno != 0) {
			std::cerr << ": " << std::strerror(errno);
		}
		std::cerr << " (try another port: ./http_server 9000)\n";
		return 1;
	}
	return 0;
}
