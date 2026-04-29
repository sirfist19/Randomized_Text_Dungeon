#include "game_input.h"

static thread_local bool g_buffered_input = false;
static thread_local std::deque<std::string> g_queue;

void set_buffered_input_mode(bool enabled) {
	g_buffered_input = enabled;
	if (!enabled) {
		g_queue.clear();
	}
}

bool is_buffered_input_mode() {
	return g_buffered_input;
}

void push_input_line(std::string line) {
	g_queue.push_back(std::move(line));
}

std::string pop_input_line_or_throw() {
	if (!g_buffered_input) {
		return {};
	}
	if (g_queue.empty()) {
		throw NeedMoreInputException();
	}
	std::string s = std::move(g_queue.front());
	g_queue.pop_front();
	return s;
}
