#include "commands.h" //top level header file

int main() {
    std::string player_name = welcome_screen();

	commands* game = new commands(player_name);
	dungeon* Dungeon = game->get_dungeon();
	Player* player = game->get_player();

	bool first_time_enter = true;
	game->intro_cut_scene();
	//Dungeon->display_debug();
	
	//Dungeon->create_sorted_room_coords();//creates the vector of coords that the map uses to draw
	//Dungeon->display_all_rooms_coords();
	bool game_over = false;

	while (!game_over)
	{
		game->game_loop(game, game_over, first_time_enter);
	}
	return 0;
}