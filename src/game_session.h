#ifndef game_session_h
#define game_session_h

#include "commands.h"
#include "game_io.h"
#include <memory>
#include <string>
#include <vector>

struct StepResult {
	std::vector<std::string> output;
	std::string phase;
	bool awaiting_input = true;
	bool session_ended = false;
	bool game_won = false;
	std::string error;
	bool clearViewport = false;
};

enum class SessionPhase {
	Boot,
	WelcomeMenu,
	NameEntry,
	NameConfirm,
	StartingGame,
	Intro,
	Playing,
	GameOverMenu,
	Ended
};

class GameSession {
public:
	explicit GameSession(std::string content_root);

	StepResult bootstrap();
	StepResult step(const std::string& line);

	std::string content_root() const { return content_root_; }

private:
	std::string content_root_;
	SessionPhase phase_ = SessionPhase::Boot;

	std::string pending_name_;
	std::unique_ptr<commands> game_;

	bool first_time_enter_ = true;
	bool game_over_ = false;
	bool quit_to_title_ = false;

	BufferedGameIO buffered_io_;

	void bind_io();
	void flush_output(StepResult& r);
	StepResult run_playing_step(const std::string& line);
};

#endif
