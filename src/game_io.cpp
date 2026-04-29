#include "game_io.h"

GameStreamBuf g_game_stream_buf;
std::ostream game_out(&g_game_stream_buf);

int GameStreamBuf::overflow(int c) {
	if (c == EOF) {
		return 0;
	}
	if (c == '\n') {
		io_newline();
	} else {
		char ch = static_cast<char>(c);
		io_write(std::string_view(&ch, 1));
	}
	return c;
}

static thread_local IGameIO* g_current_io = nullptr;

void set_current_game_io(IGameIO* io) {
	g_current_io = io;
}

IGameIO* get_current_game_io() {
	return g_current_io;
}

void io_write(std::string_view s) {
	IGameIO* io = get_current_game_io();
	if (io) {
		io->write(s);
	} else {
		std::cout << s;
	}
}

void io_newline() {
	IGameIO* io = get_current_game_io();
	if (io) {
		io->newline();
	} else {
		std::cout << std::endl;
	}
}

void TerminalGameIO::write(std::string_view s) {
	std::cout << s;
}

void TerminalGameIO::newline() {
	std::cout << std::endl;
}

void BufferedGameIO::write(std::string_view s) {
	size_t start = 0;
	while (start < s.size()) {
		size_t pos = s.find('\n', start);
		if (pos == std::string_view::npos) {
			current_line_.append(s.data() + start, s.size() - start);
			return;
		}
		current_line_.append(s.data() + start, pos - start);
		newline();
		start = pos + 1;
	}
}

void BufferedGameIO::newline() {
	lines_.push_back(std::move(current_line_));
	current_line_.clear();
}

void BufferedGameIO::clear() {
	lines_.clear();
	current_line_.clear();
	pending_viewport_clear_ = false;
}

void BufferedGameIO::request_viewport_clear() {
	pending_viewport_clear_ = true;
}

bool BufferedGameIO::take_viewport_clear_requested() {
	bool v = pending_viewport_clear_;
	pending_viewport_clear_ = false;
	return v;
}

std::vector<std::string> BufferedGameIO::take_lines() {
	std::vector<std::string> out = std::move(lines_);
	lines_.clear();
	if (!current_line_.empty()) {
		out.push_back(std::move(current_line_));
		current_line_.clear();
	}
	return out;
}
