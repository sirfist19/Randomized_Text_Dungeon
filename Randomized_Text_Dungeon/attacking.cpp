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

			weapon* player_weapon = player->get_weapon();
			int base_damage = player_weapon->get_light_damage();
			int base_hit_rate = player_weapon->get_light_hit_rate();
			int heavy_damage = player_weapon->get_heavy_damage();
			int heavy_hit_rate = player_weapon->get_heavy_hit_rate();
			std::cout << "\n";
			std::cout << "1. Heavy Attack (Base Damage: " << heavy_damage << ", Base Hit Rate: " << heavy_hit_rate << ")\n";
			std::cout << "2. Light Attack (Base Damage: "<< base_damage<<", Base Hit Rate: "<<base_hit_rate<<")\n";
			std::cout << "3. Run\n";

			bool ran_away = false;
			fighting_input_loop(game_over, player, enemy, ran_away);

			if (game_over)
				return;
			if (ran_away)
			{
				break;
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
	
	std::string valid_heavy_attack_opt[] = { "1", "heavy attack", "heavy" };
	std::string valid_light_attack_opt[] = { "2", "light attack", "light" };
	std::string valid_run_opt[] = { "3", "run" };

	bool valid_input = false;
	bool only_enemy_attacks = false;
	bool is_player_heavy_attack = false;

	while (valid_input == false) {
		std::string usr_text = input();

		if (str_input_accepted(usr_text, valid_heavy_attack_opt, 3, valid_input)) {
			is_player_heavy_attack = true;
			attack(game_over, player, enemy, only_enemy_attacks, is_player_heavy_attack);
		}
		else if (str_input_accepted(usr_text, valid_light_attack_opt, 3, valid_input)) {
			attack(game_over, player, enemy, only_enemy_attacks, is_player_heavy_attack);
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
					if ((in == "north") || (in == "go north") || (in == "go n") || (in == "n"))
					{
						ran_away = !go(0);
					}
					else if ((in == "south") || (in == "go south") || (in == "go s") || (in == "s"))
					{
						ran_away = !go(1);
					}
					else if ((in == "east") || (in == "go east") || (in == "go e") || (in == "e"))
					{
						ran_away = !go(2);
					}
					else if ((in == "west") || (in == "go west") || (in == "go w") || (in == "w"))
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
				attack(game_over, player, enemy, only_enemy_attacks, is_player_heavy_attack);
			}
		}
		else
		{
			invalid_input();
		}
	}
}
void commands::attack(bool& game_over, Player* player, Enemy* enemy, bool& only_enemy_attacks, bool& is_player_heavy_attack)
{
	if (!only_enemy_attacks)
	{
		clear_();
		printUnderscore();

		//player attacks
		int player_hit_num = random(0, 99);
		weapon* player_weapon = player->get_weapon();
		int player_damage = 0;
		int player_hit_rate = 0;
		std::string attack_type = "";

		if (is_player_heavy_attack)
		{
			player_damage = player_weapon->get_heavy_damage();
			player_hit_rate = player_weapon->get_heavy_hit_rate();
			attack_type = " uses " + player_weapon->get_name() + "'s heavy attack";
		}
		else
		{
			player_damage = player_weapon->get_light_damage();
			player_hit_rate = player_weapon->get_light_hit_rate();
			attack_type = " uses " + player_weapon->get_name() + "'s light attack";
		}

		if (player_hit_num < player_hit_rate)
		{
			player_damage = (player_damage * player_damage) / (player_damage + enemy->get_defense());//takes defense into account
			std::cout << player->get_name() << attack_type << " for " << player_damage << " damage.\n";
			enemy->get_health()->damage(player_damage);
		}
		else //a miss
		{
			std::cout << player->get_name() << attack_type << " but you miss.\n\n";
		}
		
		

		if (!enemy->is_alive())
		{
			//enemies dropping their items
			std::vector<object*> obj_to_drop;
			weapon* Weapon = enemy->get_weapon();
			helmet* Helmet = enemy->get_helmet();
			chestplate * Chestplate = enemy->get_chestplate();
			boots * Boots = enemy->get_boots();

			if (Weapon != nullptr)
				obj_to_drop.push_back(Weapon);
			if (Helmet != nullptr)
				obj_to_drop.push_back(Helmet);
			if (Chestplate != nullptr)
				obj_to_drop.push_back(Chestplate);
			if (Boots != nullptr)
				obj_to_drop.push_back(Boots);

			int num = random(1, 100);
			room* cur_room = player->get_cur_room();
			object* copy_obj_to_drop = nullptr;
			if (num < ENEMY_DROP_CHANCE)
			{
				int index = random(0, obj_to_drop.size() - 1);

				//drop obj_to_drop[index]
				object* to_drop = obj_to_drop[index];
				copy_obj_to_drop = to_drop->clone();//copy the object to the room (while keeping the polymorphism typing!)
				//copy_obj_to_drop->set_amt(amt_to_drop);//no duplicating items so set the correct amount that are dropping on the floor
				cur_room->add_item(copy_obj_to_drop);
			}

			std::cout << "\nYou killed the " << enemy->get_name() << "!\n";

			if(copy_obj_to_drop != nullptr)
				std::cout << "As it died the " << enemy->get_name() << " dropped its " << copy_obj_to_drop->get_name() << " on the floor.\n";

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
	bool is_enemy_heavy_attack = false;
	int enemy_picks_heavy_attack = random(0, 99);

	if (enemy_picks_heavy_attack < ENEMY_PICKS_HEAVY_ATTACK_CHANCE)
	{
		is_enemy_heavy_attack = true;
	}

	int enemy_hit_num = random(0, 99);
	weapon* enemy_weapon = enemy->get_weapon();
	int enemy_damage = 0;
	int enemy_hit_rate = 0;
	std::string enemy_attack_type = "";

	if (is_enemy_heavy_attack)
	{
		enemy_damage = enemy_weapon->get_heavy_damage();
		enemy_hit_rate = enemy_weapon->get_heavy_hit_rate();
		enemy_attack_type = " uses " + enemy_weapon->get_name() + "'s heavy attack";
	}
	else
	{
		enemy_damage = enemy_weapon->get_light_damage();
		enemy_hit_rate = enemy_weapon->get_light_hit_rate();
		enemy_attack_type = " uses " + enemy_weapon->get_name() + "'s light attack";
	}
	if (enemy_hit_num < enemy_hit_rate)
	{
		enemy_damage = (enemy_damage * enemy_damage) / (enemy_damage + player->get_defense());//takes defense into account
		std::cout << enemy->get_name() << enemy_attack_type << " for " << enemy_damage << " damage.\n\n";
		player->get_health()->damage(enemy_damage);
	}
	else //a miss
	{
		std::cout << enemy->get_name() << enemy_attack_type << " but misses.\n\n";
	}
	
	

	game_over = !(player->is_alive());
}