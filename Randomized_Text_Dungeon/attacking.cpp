#include "commands.h"

//attacking
void commands::fighting(room* cur_room, bool& game_over, Player* player)
{
	//fighting stuff
	std::vector<Enemy*> enemies = cur_room->get_enemies();

	if (!enemies.empty())
	{
		bool fighting = true;
		int index = enemies.size() - 1;
		Enemy* enemy = enemies[index];//grab the last enemy in the list

		//display intial attacking info
		printUnderscore();
		std::cout << "A " << enemy->get_name() << " appeared and starts attacking you!\n\n";

		while (fighting)
		{
			//display attacking info
			enemy->display_attack_info();
			player->display_attack_info();

			std::cout << "\n";
			std::cout << "1. Attack\n";
			std::cout << "2. Run\n";

			bool ran_away = false;
			fighting_input_loop(game_over, player, enemy, ran_away);

			if (ran_away)
			{
				break;
			}

			if (game_over)
			{
				game_Over();
			}
			if (!enemy->is_alive())
			{
				enemies.pop_back();
				cur_room->set_enemies(enemies);
				fighting = false;
				break;
			}
		}

		player->printTopBar();
		player->get_cur_room()->display_room();
	}
}
void commands::fighting_input_loop(bool& game_over, Player* player, Enemy* enemy, bool& ran_away)
{
	//std::vector<std::string> usr_text = prompt();
	//printUnderscore();
	//loop = parseInputVector(usr_text, game_over);

	//std::string noun = usr_text[0];

	std::string valid_attack_opt[] = { "1", "attack" };
	std::string valid_run_opt[] = { "2", "run" };

	bool valid_input = false;
	bool only_enemy_attacks = false;

	while (valid_input == false) {
		std::string usr_text = input();

		if (str_input_accepted(usr_text, valid_attack_opt, 2, valid_input)) {
			attack(game_over, player, enemy, only_enemy_attacks);
		}
		else if (str_input_accepted(usr_text, valid_run_opt, 2, valid_input)) {
			//run
			int num = random(1, 100);
			if (num < RUN_AWAY_CHANCE)
			{
				while (!ran_away)
				{
					//run away run away
					printUnderscore();
					print("Which direction do you want to run in?");
					room* cur_room = player->get_cur_room();
					cur_room->display_exit_information();
					int* room_exits = cur_room->get_exits();

					std::string in = input();
					if ((in == "north") || (in == "go north"))
					{
						ran_away = !go(0);
					}
					else if ((in == "south") || (in == "go south"))
					{
						ran_away = !go(1);
					}
					else if ((in == "east") || (in == "go east"))
					{
						ran_away = !go(2);
					}
					else if ((in == "west") || (in == "go west"))
					{
						ran_away = !go(3);
					}
					else
						std::cout << "Direction not not recognized.\n";
				}
			}
			else
			{
				only_enemy_attacks = true;
				attack(game_over, player, enemy, only_enemy_attacks);
			}
		}
		else
		{
			invalid_input();
		}
	}
}
void commands::attack(bool& game_over, Player* player, Enemy* enemy, bool& only_enemy_attacks)
{
	if (!only_enemy_attacks)
	{
		clear_();
		printUnderscore();

		//player attacks
		weapon* player_weapon = player->get_weapon();
		int player_damage = player_weapon->get_damage();
		player_damage = (player_damage * player_damage) / (player_damage + enemy->get_defense());//takes defense into account
		enemy->get_health()->damage(player_damage);
		std::cout << player->get_name() << " attacks with " << player_weapon->get_name() << " for " << player_damage << " damage.\n";

		if (!enemy->is_alive())
		{
			std::cout << "\nYou killed the " << enemy->get_name() << "!\n";
			int gained_exp = enemy->get_exp();
			std::cout << "You gained " << gained_exp << "xp points.\n\n";
			player->increase_exp(gained_exp);
			return;
		}
	}
	else //if only the enemy attacks (coming from not being able to run from the encounter)
	{
		clear_();
		printUnderscore();
		std::cout << "You couldn't get away!\n";
	}

	//enemy attacks
	weapon* enemy_weapon = enemy->get_weapon();
	int enemy_damage = enemy_weapon->get_damage();
	enemy_damage = (enemy_damage * enemy_damage) / (enemy_damage + player->get_defense());//takes defense into account
	player->get_health()->damage(enemy_damage);
	std::cout << enemy->get_name() << " attacks with " << enemy_weapon->get_name() << " for " << enemy_damage << " damage.\n\n";

	game_over = !(player->is_alive());
}