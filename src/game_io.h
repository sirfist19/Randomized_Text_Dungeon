#ifndef game_io_h
#define game_io_h

#include <iostream>
#include <streambuf>
#include <string>
#include <string_view>
#include <vector>

// Abstract sink for all game text. Terminal uses game_out; HTTP uses BufferedGameIO.
class IGameIO {
public:
	virtual ~IGameIO() = default;
	virtual void write(std::string_view s) = 0;
	virtual void newline() = 0;
	virtual bool supports_shell_clear() const { return true; }
};

// Output formatting width (in monospace character columns).
// Stored thread-locally so each web session/request can format independently.
void set_output_columns(int cols);
int get_output_columns();

class TerminalGameIO : public IGameIO {
public:
	void write(std::string_view s) override;
	void newline() override;
};

// Records output as lines for JSON responses. Coalesces write()+newline into one line.
class BufferedGameIO : public IGameIO {
public:
	void write(std::string_view s) override;
	void newline() override;
	bool supports_shell_clear() const override { return false; }
	void clear();
	const std::vector<std::string>& lines() const { return lines_; }
	std::vector<std::string> take_lines();
	// Web client should wipe the transcript when set; consumed in GameSession::flush_output.
	void request_viewport_clear();
	bool take_viewport_clear_requested();

private:
	std::vector<std::string> lines_;
	std::string current_line_;
	bool pending_viewport_clear_ = false;
};

// Thread-local active sink (set per session / CLI run). Null means fall back to game_out.
void set_current_game_io(IGameIO* io);
IGameIO* get_current_game_io();

void io_write(std::string_view s);
void io_newline();

// Stream that mirrors game_out but routes through io_write / io_newline (for BufferedGameIO).
class GameStreamBuf : public std::streambuf {
protected:
	int overflow(int c) override;
};

extern GameStreamBuf g_game_stream_buf;
extern std::ostream game_out;

#endif
