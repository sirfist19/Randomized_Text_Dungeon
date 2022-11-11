#include "commands.h" //top level header file

int main() 
{
	bool game_over = false;
	bool quit_to_title_screen = false;

	while (!game_over)
	{
		quit_to_title_screen = false;
		std::string player_name = welcome_screen();

		commands* game = new commands(player_name);
		dungeon* Dungeon = game->get_dungeon();
		Player* player = game->get_player();
		   
		bool first_time_enter = true;
		game->intro_cut_scene();
		//Dungeon->display_debug();
		//Dungeon->display_dungeon_info();

		while (!quit_to_title_screen)
		{
			game->game_loop(game, game_over, quit_to_title_screen, first_time_enter);

			if (game_over)
			{
				game_Over(game_over, quit_to_title_screen);
			}
		}
		//print("Deleting the game.");
		delete game;
	}
    
	return 0;
}