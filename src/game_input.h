#ifndef game_input_h
#define game_input_h

#include <deque>
#include <string>
#include <stdexcept>

// Thrown when buffered mode needs another line from the client (HTTP step).
struct NeedMoreInputException : public std::runtime_error {
	NeedMoreInputException() : std::runtime_error("need_more_input") {}
};

void set_buffered_input_mode(bool enabled);
bool is_buffered_input_mode();

void push_input_line(std::string line);
std::string pop_input_line_or_throw();

#endif
