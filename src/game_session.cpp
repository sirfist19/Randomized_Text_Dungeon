#include "game_session.h"
#include "commands.h"
#include "db/rooms_db.h"
#include "db/static_objects_db.h"
#include "game_errors.h"
#include "game_input.h"
#include "helper_fxns.h"
#include "constants.h"

#include <cstdlib>
#include <memory>
#include <stdexcept>

static std::string env_or_default(const char* key, const char* fallback) {
	if (const char* v = std::getenv(key)) {
		return std::string(v);
	}
	return std::string(fallback);
}

GameSession::GameSession(std::string content_root)
	: content_root_(std::move(content_root)) {
	if (content_root_.empty()) {
		content_root_ = env_or_default("CONTENT_ROOT", "../content");
	}
	if (!content_root_.empty() && content_root_.back() == '/') {
		content_root_.pop_back();
	}
}

void GameSession::bind_io() {
	set_current_game_io(&buffered_io_);
	set_buffered_input_mode(false);
	set_wait_fast(false);
}

void GameSession::flush_output(StepResult& r) {
	r.output = buffered_io_.take_lines();
	r.clearViewport = buffered_io_.take_viewport_clear_requested();
}

StepResult GameSession::bootstrap() {
	StepResult r;
	phase_ = SessionPhase::Boot;
	buffered_io_.clear();
	bind_io();

	try {
		RoomsDB::load(content_root_ + "/rooms.json");
		StaticObjectsDB::load(content_root_ + "/static_objects.json");

		printEquals();
		print();
		print("Welcome to Randomized Text Dungeon!");
		print("\t- An adventure game by Aidan Cini");
		print();
		print("This is a randomized text dungeon. The rooms, enemies, and items are randomized so every time you play, the dungeon will be different. Playing in fullscreen mode is recommended. It's more immersive and looks nicer.\n");
		print("You are an explorer that has just found the dungeon that you have been searching for for 14 years. A massive entrance lined with columns stands leads into the depths below. Enter if you dare.");
		printEquals();
		print();
		print("\n1. Enter");
		print("2. Leave");

		phase_ = SessionPhase::WelcomeMenu;
		r.phase = "welcome";
		r.awaiting_input = true;
	} catch (const std::exception& e) {
		r.error = e.what();
		r.session_ended = true;
	}
	flush_output(r);
	return r;
}

StepResult GameSession::step(const std::string& line) {
	StepResult r;
	buffered_io_.clear();
	bind_io();

	try {
		switch (phase_) {
		case SessionPhase::WelcomeMenu: {
			std::string welcome_input = line;
			turn_to_lower_case(welcome_input);
			std::string yes_inputs[] = { "1", "start", "enter" };
			std::string no_inputs[] = { "2", "quit", "leave", "q" };
			bool valid = false;
			if (str_input_accepted(welcome_input, no_inputs, 4, valid)) {
				throw GameQuitException();
			}
			if (str_input_accepted(welcome_input, yes_inputs, 3, valid)) {
				print("What is your name? I don't seem to remember...");
				phase_ = SessionPhase::NameEntry;
				r.phase = "name";
				r.awaiting_input = true;
				break;
			}
			invalid_input();
			r.phase = "welcome";
			r.awaiting_input = true;
			break;
		}
		case SessionPhase::NameEntry: {
			pending_name_ = line;
			io_newline();
			io_write("Are you sure that ");
			io_write(pending_name_);
			io_write(" is correct?\n");
			print("1. Yes");
			print("2. No");
			phase_ = SessionPhase::NameConfirm;
			r.phase = "confirm_name";
			r.awaiting_input = true;
			break;
		}
		case SessionPhase::NameConfirm: {
			std::string answer = line;
			turn_to_lower_case(answer);
			std::string yes_input[] = { "1", "yes" };
			std::string no_input[] = { "2", "no" };
			bool valid = false;
			if (str_input_accepted(answer, no_input, 2, valid)) {
				print("Then type your name again. Not sure how you couldn't remember your own name though.");
				phase_ = SessionPhase::NameEntry;
				r.phase = "name";
				r.awaiting_input = true;
				break;
			}
			if (str_input_accepted(answer, yes_input, 2, valid) && pending_name_.size() > 15) {
				print("That name is too long. There is a maximum name length of 15 characters. Please enter a different name.");
				phase_ = SessionPhase::NameEntry;
				r.phase = "name";
				r.awaiting_input = true;
				break;
			}
			if (!str_input_accepted(answer, yes_input, 2, valid)) {
				invalid_input();
				r.phase = "confirm_name";
				r.awaiting_input = true;
				break;
			}
			io_write("Well then ");
			io_write(pending_name_);
			io_write(" welcome to the dungeon! And best of luck.\n\n");
			wait(4);
			clear_();
			print("Generating the dungeon. Please wait.");

			game_.reset(new commands(pending_name_));
			game_over_ = false;
			quit_to_title_ = false;
			first_time_enter_ = true;
			game_->intro_cut_scene();
			phase_ = SessionPhase::Playing;
			r.phase = "playing";
			r.awaiting_input = true;
			break;
		}
		case SessionPhase::Playing:
			r = run_playing_step(line);
			break;
		case SessionPhase::GameOverMenu: {
			std::string in = line;
			turn_to_lower_case(in);
			if (in == "1" || in == "restart") {
				game_over_ = false;
				quit_to_title_ = false;
				printUnderscore();
				print("Restarting the game...");
				wait(5);
				clear_();
				game_.reset();
				RoomsDB::load(content_root_ + "/rooms.json");
				StaticObjectsDB::load(content_root_ + "/static_objects.json");
				phase_ = SessionPhase::WelcomeMenu;
				// re-show welcome
				printEquals();
				print();
				print("Welcome to Randomized Text Dungeon!");
				print("\t- An adventure game by Aidan Cini");
				print();
				print("This is a randomized text dungeon. The rooms, enemies, and items are randomized so every time you play, the dungeon will be different. Playing in fullscreen mode is recommended. It's more immersive and looks nicer.\n");
				print("You are an explorer that has just found the dungeon that you have been searching for for 14 years. A massive entrance lined with columns stands leads into the depths below. Enter if you dare.");
				printEquals();
				print();
				print("\n1. Enter");
				print("2. Leave");
				r.phase = "welcome";
				r.awaiting_input = true;
				break;
			}
			if (in == "2" || in == "quit") {
				game_over_ = true;
				quit_to_title_ = true;
				printUnderscore();
				print("Bye bye! Thanks for playing!");
				printUnderscore();
				phase_ = SessionPhase::Ended;
				r.session_ended = true;
				r.phase = "ended";
				r.awaiting_input = false;
				break;
			}
			printUnderscore();
			print("Invalid input. Please try again.");
			printUnderscore();
			r.phase = "game_over";
			r.awaiting_input = true;
			break;
		}
		default:
			r.error = "invalid phase";
			r.session_ended = true;
			break;
		}
	} catch (const GameQuitException&) {
		r.session_ended = true;
		r.phase = "quit";
		r.awaiting_input = false;
	} catch (const GameWonException&) {
		r.game_won = true;
		r.session_ended = true;
		r.phase = "won";
		r.awaiting_input = false;
	} catch (const std::exception& e) {
		r.error = e.what();
		r.session_ended = true;
	}

	flush_output(r);
	return r;
}

StepResult GameSession::run_playing_step(const std::string& line) {
	StepResult r;
	if (!game_) {
		r.error = "no game";
		r.session_ended = true;
		return r;
	}

	game_->game_loop_step(game_.get(), game_over_, quit_to_title_, first_time_enter_, line);

	if (quit_to_title_) {
		game_.reset();
		RoomsDB::load(content_root_ + "/rooms.json");
		StaticObjectsDB::load(content_root_ + "/static_objects.json");
		phase_ = SessionPhase::WelcomeMenu;
		printEquals();
		print();
		print("Welcome to Randomized Text Dungeon!");
		print("\t- An adventure game by Aidan Cini");
		print();
		print("This is a randomized text dungeon. The rooms, enemies, and items are randomized so every time you play, the dungeon will be different. Playing in fullscreen mode is recommended. It's more immersive and looks nicer.\n");
		print("You are an explorer that has just found the dungeon that you have been searching for for 14 years. A massive entrance lined with columns stands leads into the depths below. Enter if you dare.");
		printEquals();
		print();
		print("\n1. Enter");
		print("2. Leave");
		r.phase = "welcome";
		r.awaiting_input = true;
		return r;
	}

	if (game_over_) {
		printEquals();
		io_newline();
		io_write("GAME OVER!\n");
		io_write("You died. Better luck next time.\n");
		printEquals();
		print("\nDo you want to restart or quit the game?");
		print("1. Restart");
		print("2. Quit");
		phase_ = SessionPhase::GameOverMenu;
		r.phase = "game_over";
		r.awaiting_input = true;
		return r;
	}

	r.phase = "playing";
	r.awaiting_input = true;
	return r;
}
