#ifndef game_errors_h
#define game_errors_h

#include <exception>
#include <string>

// User quit from title / welcome (replaces exit from Quit()).
struct GameQuitException : public std::exception {
	const char* what() const noexcept override { return "quit"; }
};

// Player reached winning screen.
struct GameWonException : public std::exception {
	const char* what() const noexcept override { return "won"; }
};

// Fatal engine error (replaces exit(1) in object/dungeon).
struct GameEngineError : public std::exception {
	explicit GameEngineError(std::string msg) : msg_(std::move(msg)) {}
	const char* what() const noexcept override { return msg_.c_str(); }
private:
	std::string msg_;
};

#endif
