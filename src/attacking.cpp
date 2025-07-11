#include "commands.h"

void enemy_drop_items(Enemy* enemy, Player* player) ;
void print_attack(bool player_attacking, Enemy* enemy, Player* player, int damage, bool is_heavy_attack, bool missed);
void attacks(Player* player, Enemy* enemy, bool is_heavy_attack, bool player_attacks);
void inflict_status(Enemy* enemy, Player* player, bool player_turn);
bool before_turn_status_effect_processing(Player* player, Enemy* enemy, Alive_Entity* attacking_entity);
void after_turn_status_effect_processing(Player* player, Enemy* enemy);
bool try_crit(Alive_Entity* attacking_entity);

const int ATTACK_WAIT_SECONDS = 5;

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
			printEquals();
			print();

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
			fighting_input_loop(game_over, player, enemy, ran_away, cur_room);

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
		player->get_status_effect()->reset_status_effect();
		//player->printTopBar();
		//player->get_cur_room()->display_room();
	}
}
void commands::fighting_input_loop(bool& game_over, Player* player, 
	Enemy* enemy, bool& ran_away, room* cur_room)
{
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
			clear_();
			display_cur_room_with_top_bar(cur_room);
			printEquals();
			print();

			attack(game_over, player, enemy, only_enemy_attacks, is_player_heavy_attack);
		}
		else if (str_input_accepted(usr_text, valid_light_attack_opt, 3, valid_input)) {
			clear_();
			display_cur_room_with_top_bar(cur_room);
			printEquals();
			print();
			
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
					{
						std::cout << "Direction not not recognized.\n";
						return;
					}
					display_cur_room_with_top_bar(player->get_cur_room());
					printEquals();
					print();	
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
void commands::attack(bool& game_over, Player* player, Enemy* enemy, 
	bool& only_enemy_attacks, bool& is_player_heavy_attack)
{
	// The player attacks first
	if (!only_enemy_attacks)
	{
		attacks(player, enemy, is_player_heavy_attack, true); // player attacks

		if (!enemy->is_alive())
		{
			enemy_drop_items(enemy, player);
			return;
		}
	}
	else //if only the enemy attacks (coming from not being able to run from the encounter)
	{
		std::cout << "You couldn't get away!\n";
	}

	attacks(player, enemy, false, false); // enemy attacks

	after_turn_status_effect_processing(player, enemy);

	game_over = !(player->is_alive());
	
}

void inflict_status(Enemy* enemy, Player* player, bool player_turn) 
{
	weapon* weapon = nullptr;
	if (player_turn)
	{
		weapon = player->get_weapon();
	}
	else 
	{
		weapon = enemy->get_weapon();
	}

	Weapon_Effect effect = Weapon_Effect::NONE;
	if (weapon->hasEnchantment()) 
	{
		effect = weapon->use_enchantment();
	}

	// inflict the effect on the opponent
	if (effect != Weapon_Effect::NONE) 
	{
		Status_Effect_Type type = Status_Effect_Type::NONE_;

		switch(effect) {
			case Weapon_Effect::BURNING:
				type = Status_Effect_Type::BURN;
				break;
			case Weapon_Effect::POISON:
				type = Status_Effect_Type::POISON_;
				break;
			case Weapon_Effect::ELECTRIFIED:
				type = Status_Effect_Type::PARALYSIS;
				break;
			case Weapon_Effect::FREEZING:
				type = Status_Effect_Type::FROZEN;
				break;
			default:
				break;
		}
		Status_Effect temp = Status_Effect(type);

		if(player_turn && !enemy->get_status_effect()->already_has_status_effect())
		{ // if the player is attacking and the enemy doesn't already have a status effect
			enemy->set_status(type);
			print(enemy->get_name() + " became " + temp.get_display_str() + ".");
			wait(ATTACK_WAIT_SECONDS);
		}
		else if (!player_turn && !player->get_status_effect()->already_has_status_effect())
		{ // if enemy is attacking and the player doesn't already have a status effect
			player->set_status(type);
			print(player->get_name() + " became " + temp.get_display_str() + ".");
			wait(ATTACK_WAIT_SECONDS);
		}
	}
}
void attacks(Player* player, Enemy* enemy, bool is_heavy_attack, bool player_attacks) 
{
	int hit_num = random(0, 99);
	weapon* weapon = nullptr; 
	if (player_attacks) 
		weapon = player->get_weapon();
	else // enemy attacks
	{
		weapon = enemy->get_weapon();

		int enemy_picks_heavy_attack = random(0, 99);
		if (enemy_picks_heavy_attack < ENEMY_PICKS_HEAVY_ATTACK_CHANCE)
		{
			is_heavy_attack = true;
		}
	}
	Alive_Entity* attacking_entity = player_attacks ? (Alive_Entity*)player : (Alive_Entity*)enemy;
	Alive_Entity* defending_entity = player_attacks ? (Alive_Entity*)enemy : (Alive_Entity*)player;
	
	int damage = 0;
	int hit_rate = 0;
	std::string attack_type = "";

	bool continue_on = before_turn_status_effect_processing(player, enemy, attacking_entity);
	if (!continue_on) 
		return;

	if (is_heavy_attack)
	{
		damage = weapon->get_heavy_damage();
		hit_rate = weapon->get_heavy_hit_rate();
	}
	else
	{
		damage = weapon->get_light_damage();
		hit_rate = weapon->get_light_hit_rate();
	}

	// Damage the opposing entity
	if (hit_num < hit_rate)
	{
		
		damage = ((double)damage * (double)damage) 
				/ ((double)damage + (double)(DEFENSE_MULTIPLIER_ATTACK_CALCULATION * defending_entity->get_defense()));//takes defense into account
		
		bool is_crit = try_crit(attacking_entity);
		if (is_crit)
		{
			damage *= CRIT_MULTIPLIER;
		}
		print_attack(player_attacks, enemy, player, damage, is_heavy_attack, false); // player_attacking, enemy, player, damage, is_heavy_attack, missed
		inflict_status(enemy, player, player_attacks);

		defending_entity->get_health()->damage(damage);
	}
	else //a miss
	{
		print_attack(player_attacks, enemy, player, 0, is_heavy_attack, true); // player_attacking, enemy, player, damage, is_heavy_attack, missed
	}
}
bool try_crit(Alive_Entity* attacking_entity)
{
	weapon* wp = attacking_entity->get_weapon();
	if (wp->hasEnchantment() && wp->get_enchantment().get_effect() == Weapon_Effect::CRIT)
	{
		int rand_int = random(0,99);
		float chance = wp->get_enchantment().get_chance();

	}
	return false;
}
bool before_turn_status_effect_processing(Player* player, Enemy* enemy, Alive_Entity* attacking_entity)
{
	Status_Effect* cur_status_effect = attacking_entity->get_status_effect();

	// before turn status effect processing
	if (cur_status_effect->get_status_effect_type() == Status_Effect_Type::FROZEN)
	{
		// has a chance to unfreeze and then move
		bool unfreeze = cur_status_effect->attempt_unfreeze();
		if (unfreeze) 
		{
			print(attacking_entity->get_name() + "unfroze!");
			wait(ATTACK_WAIT_SECONDS);
			cur_status_effect->reset_status_effect();
		}
		else 
		{
			print(attacking_entity->get_name() + " tried to attack but is frozen.");
			wait(ATTACK_WAIT_SECONDS);
			return false;
		}
	}
	else if (cur_status_effect->get_status_effect_type() == Status_Effect_Type::PARALYSIS)
	{
		// has a chance to not move
		bool will_move = cur_status_effect->attempt_paralysis_move();
		if (!will_move)
		{
			print(attacking_entity->get_name() + " cannot attack due to paralysis.");
			wait(ATTACK_WAIT_SECONDS);
			return false;
		}
	}
	return true;
}
void after_turn_status_effect_processing(Player* player, Enemy* enemy)
{
	// burns, poison
	for (int i = 0; i < 2; i++)
	{
		Alive_Entity* entity = nullptr;
		if (i == 0)
			entity = player;
		else if (i== 1)
			entity = enemy;
		Status_Effect* status_effect = entity->get_status_effect();

		if (status_effect->get_status_effect_type() == Status_Effect_Type::BURN)
		{
			// burns always do n% of health in damage
			const float burn_percentage = BURN_HEALTH_PERCENTAGE/100.0;
			int max_health = entity->get_health()->get_max_health();
			int damage = burn_percentage * max_health;
			entity->get_health()->damage(damage);
			print(entity->get_name() + " lost " + std::to_string(damage) + " health due to having a burn.");
			wait(ATTACK_WAIT_SECONDS);
		}
		else if (status_effect->get_status_effect_type() == Status_Effect_Type::POISON_)
		{
			status_effect->increase_turns_since_inflicted();
			const int turns_since_poison_start = status_effect->get_turns_since_inflicted();
			const float poison_percentage = (turns_since_poison_start * FIRST_TURN_POISON_PERCENTAGE)/100.0;
			int max_health = entity->get_health()->get_max_health();
			int damage = poison_percentage * max_health;
			entity->get_health()->damage(damage);
			print(entity->get_name() + " lost " + std::to_string(damage) + " health due to poison.");
			wait(ATTACK_WAIT_SECONDS);

			
		}
	}
}

void print_attack(bool player_attacking, Enemy* enemy, Player* player, int damage
, bool is_heavy_attack, bool missed) 
{
	weapon* enemy_weapon = enemy->get_weapon();
	weapon* player_weapon = player->get_weapon();
	std::string attack_type_str = is_heavy_attack ? "'s heavy attack" : "'s light attack";

	if (player_attacking)
	{
		std::string player_attack_type = " uses " + player_weapon->get_name() + attack_type_str;
		if (missed) 
		{
			std::cout << player->get_name() << player_attack_type << " but you miss.\n\n";
		}
		else 
		{
			std::cout << player->get_name() << player_attack_type << " for " << damage << " damage.\n\n";
		}
	}
	else 
	{
		std::string enemy_attack_type = " uses " + enemy_weapon->get_name() + attack_type_str;
		if (missed)
		{
			std::cout << enemy->get_name() << enemy_attack_type << " but misses.\n\n";
		}
		else 
		{
			std::cout << enemy->get_name() << enemy_attack_type << " for " << damage << " damage.\n\n";
		}
	}
	wait(ATTACK_WAIT_SECONDS);
}
void enemy_drop_items(Enemy* enemy, Player* player) 
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

	int item_num = random(1, 100);
	int gold_num = random(1, 100);
	room* cur_room = player->get_cur_room();
	object* copy_obj_to_drop = nullptr;
	object* copy_gold_to_drop = nullptr;
	if (item_num < ENEMY_DROP_ITEM_CHANCE)
	{
		int index = random(0, obj_to_drop.size() - 1);

		//drop obj_to_drop[index]
		object* to_drop = obj_to_drop[index];
		copy_obj_to_drop = to_drop->clone();//copy the object to the room (while keeping the polymorphism typing!)
		//copy_obj_to_drop->set_amt(amt_to_drop);//no duplicating items so set the correct amount that are dropping on the floor
		cur_room->add_item(copy_obj_to_drop);
	}
	if (gold_num < ENEMY_DROP_GOLD_CHANCE)
	{
		object* to_drop_gold = enemy->get_gold();
		copy_gold_to_drop = to_drop_gold->clone();
		cur_room->add_item(copy_gold_to_drop);
	}

	std::cout << "\nYou killed the " << enemy->get_name() << "!\n";
	std::vector<object*> to_drop;
	if (copy_obj_to_drop != nullptr)
		to_drop.push_back(copy_obj_to_drop);
	if (copy_gold_to_drop != nullptr)
		to_drop.push_back(copy_gold_to_drop);

	std::string before = "As it died the " + enemy->get_name() + " dropped ";
	std::string after = " on the floor.\n";
	print_items(to_drop, before, after);

	int gained_exp = enemy->get_exp();
	std::cout << "You gained " << gained_exp << "xp points.\n\n";
	player->increase_exp(gained_exp);		
}