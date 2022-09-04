#include "helper_fxns.h"
#include "dungeon.h"
#include "room.h"
#include "player.h"
#include "enemy.h"
#include "commands.h"

int main() {
    std::string player_name = welcome_screen();

	commands* game = new commands(player_name);
	dungeon* Dungeon = game->get_dungeon();
	Player* player = game->get_player();

	bool first_time_enter = true;
	game->intro_cut_scene();
	//Dungeon->display_debug();
	bool game_over = false;

	while (!game_over)
	{
		game->game_loop(game, game_over, first_time_enter);
	}
	return 0;
}