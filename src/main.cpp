#include "game_session.h"
#include "helper_fxns.h"
#include "game_io.h"

#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string>

static void print_step(const StepResult& r) {
	for (const auto& line : r.output) {
		std::cout << line << "\n";
	}
	if (!r.error.empty()) {
		std::cerr << "Error: " << r.error << "\n";
	}
}

int main()
{
	set_wait_fast(false);
	set_cli_os_screen_clear_allowed(true);
	set_current_game_io(nullptr);

	auto terminal_columns = []() -> int {
		struct winsize ws {};
		if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0 && ws.ws_col > 0) {
			return static_cast<int>(ws.ws_col);
		}
		return 0;
	};

	int cols = terminal_columns();
	if (cols <= 0) {
		cols = 120;
	}

	GameSession session("", cols);
	StepResult r = session.bootstrap();
	print_step(r);

	std::string line;
	while (!r.session_ended) {
		std::cout << '>' << std::flush;
		std::getline(std::cin, line);
		r = session.step(line);
		print_step(r);
	}

	return 0;
}
