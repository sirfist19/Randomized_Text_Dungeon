#include <string>
#include "commands.h"

//the main game loop that controls everything
void commands::game_loop(commands* game, bool& game_over, bool& first_time_enter)
{
	//check to make sure the player is still alive
	game_over = !(player->is_alive());
	if (game_over)
		return;

	bool loop = true;
	room* cur_room = player->get_cur_room();
	if (!first_time_enter)
	{
		player->printTopBar();
		cur_room->display_room();
	}
	else
	{
		first_time_enter = false;
	}
	
	fighting(cur_room, game_over, player);
	input_loop(loop, game_over);
}

//constructor
commands::commands(std::string player_name)
	:cur_noun(noun::error_), cur_verb(verb::error)
{
	//create the dungeon 
	do {
		Dungeon = new dungeon();
	} while (Dungeon->get_size() < MIN_DUNGEON_SIZE);


	//create the player
	room* start_room = Dungeon->get_start_room();
	player = new Player(player_name, start_room);

	//create the verb and noun charts
	verb_chart["go"] = verb::go;
	verb_chart["examine"] = verb::examine;
	verb_chart["use"] = verb::use;
	verb_chart["take"] = verb::take;
	verb_chart["pickup"] = verb::take;
	verb_chart["equip"] = verb::equip;
	verb_chart["quit"] = verb::quit;
	verb_chart["clear"] = verb::clear;
	verb_chart["open"] = verb::open;
	verb_chart["jump"] = verb::jump;
	verb_chart["inventory"] = verb::inventory;
	verb_chart["hello"] = verb::hello;
	verb_chart["help"] = verb::help;
	verb_chart["list"] = verb::list;
	verb_chart["drink"] = verb::drink;
	verb_chart["drop"] = verb::drop;

	noun_chart["north"] = noun::north;
	noun_chart["south"] = noun::south;
	noun_chart["east"] = noun::east;
	noun_chart["west"] = noun::west;
	noun_chart["pit"] = noun::pit;
	noun_chart["chest"] = noun::_chest;
	noun_chart["all"] = noun::all;
}

//destructor
commands::~commands()
{
	delete player;
	delete Dungeon;
}

//getter functions
Player* commands::get_player()
{
	return player;
}
dungeon* commands::get_dungeon()
{
	return Dungeon;
}

//input and parsing
void commands::input_loop(bool& loop, bool& game_over)
{
	if (game_over)
		return;

	while (loop)
	{
		cur_player_input = prompt();
		printUnderscore();
		loop = parseInputVector(game_over);

		if (loop)
			printUnderscore();
	}
}
bool commands::parseInputVector(bool& game_over)
{
	//set cur_noun and cur_verb based on user input
	//if not found in the maps then return false and print 
	//command string not found

	//returning false reprints the top bar and room info
	if (cur_player_input.size() <= 0)
	{
		print("You cannot enter nothing. That's not a command. Please try again.");
		return true;
	}

	//verb stuff
	std::unordered_map<std::string, verb>::iterator gotVerb
		= verb_chart.find(cur_player_input[0]);

	if (gotVerb == verb_chart.end())
	{
		std::cout << "Command not recognized! Please try again.\n";
		return true;
	}
	cur_verb = gotVerb->second;

	//noun stuff
	std::unordered_map<std::string, noun>::iterator gotNoun;
	if (cur_player_input.size() > 1)
	{
		gotNoun = noun_chart.find(cur_player_input[1]);
	}
	if ((cur_player_input.size() > 1) && (gotNoun != noun_chart.end()))//size is greater than 1 and noun is recognized
		cur_noun = gotNoun->second;
	else if ((cur_player_input.size() <= 1))//if a noun is not typed
		cur_noun = noun::_none;
	else
		cur_noun = noun::un_assigned;
	player_input_noun = get_player_input_noun();//set player_input_noun

	if (DEBUG_MODE)
	{
		std::cout << "Input Size: " << cur_player_input.size();
		std::cout << "Current Verb: " << cur_verb;
		std::cout << "\nCurrent Noun: " << cur_noun;
	}
	switch (cur_verb)//goes to all commands
	{
	case verb::go:
		if (cur_noun == noun::north)
		{
			return go(0);
		}
		else if (cur_noun == noun::south)
		{
			return go(1);
		}
		else if (cur_noun == noun::east)
		{
			return go(2);
		}
		else if (cur_noun == noun::west)
		{
			return go(3);
		}
		else
			std::cout << "Direction of the go command not recognized.\n";
		break;
	case verb::clear:
		clear_();
		return false;
		break;
	case verb::use:
		use();
		break;
	case verb::jump:
		jump();
		break;
	case verb::drink:
		drink();
		break;
	case verb::equip:
		equip();
		break;
	case verb::quit:
		game_over = true;
		print("\nYou quit the game. Bye bye. See you next time!\n");
		return false;
		break;
	case verb::open:
		open();
		break;
	case verb::hello:
		hello();
		break;
	case verb::inventory:
		player->display_inventory();
		break;
	case verb::drop:
		drop();
		break;
	case verb::help:
		help();
		break;
	case verb::list:
		print_all_commands();
		break;
	case verb::take:
		take();
		break;
	default:
		print("Command recognized but not implemented");
		break;
	}
	return true;
}
std::string commands::get_player_input_noun() const
{
	std::string player_input_noun = "";
	if (cur_player_input.size() == 1)
		return player_input_noun;

	for (unsigned int i = 1; i < cur_player_input.size(); i++)
	{
		if (i != 1)
		{
			player_input_noun += " ";
		}
		player_input_noun += cur_player_input[i];
	}
	return player_input_noun;
}

//first time entering the start room description
void commands::intro_cut_scene()
{
	player->printTopBar();
	print("You enter the dungeon and stare at the entrance room. Two large golden doors with a jade carving of a dragon breathing fire lie open. Marble covered in geometric designs line the floor. The walls are made of cold, gray, stone. You feel the temperature start to drop the farther into the room you go.\n");
	//wait(5);
	print("All of a sudden the two golden doors begin to swing shut. You run towards the entrance, but it's too late. They've already closed. You push the door with all your might, but it won't budge. You examine the door and find a slot for a key. It looks like your only way out is to explore the dungeon and find the special dragon key.");
	//wait(5);
	std::cout << "\n";
	player->get_cur_room()->display_exit_information();
	std::cout << "\n";
	print("TIP: Don't know what to type? Type 'help' for more information.");
}

//for the list command
void commands::print_all_commands()
{
	print("Note: Objects found in rooms are also valid objects.");
	std::cout << "\nList of all valid verbs: \n";
	print_all_verbs();

	std::cout << "\nList of all valid objects: \n";
	print_all_nouns();
}
void commands::print_all_verbs()
{
	int i = 1;
	for (auto it = verb_chart.begin(); it != verb_chart.end(); it++)
	{
		print(std::to_string(i) + ". " + it->first);
		i++;
	}
}
void commands::print_all_nouns()
{
	int i = 1;
	for (auto it = noun_chart.begin(); it != noun_chart.end(); it++)
	{
		print(std::to_string(i) + ". " + it->first);
		i++;
	}
}

//stack objects on the floor or in the player's inventory
void commands::stack_objects(std::vector<object*>& in)
{
	//player->display_inventory();
	//n^2 algo to stack same objects
	bool combine = false;
	for (unsigned int i = 0; i < in.size(); i++)
	{
		for (unsigned int j = i + 1; j < in.size(); j++)
		{
			combine = in[i]->is_same_type_of_object(in[j]);

			if (combine)
			{
				//std::cout << "Combining " << in[i]->get_name() << " with " << in[j]->get_name();
				in[i]->combine(in[j]);//in[j] is now invalid

				//remove in[j]
				in[j] = in[in.size() - 1];//set in[j] to the last element of the vector
				in.pop_back();
			}
		}
	}
}

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

			fighting_input_loop(game_over, player, enemy);

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
		cur_room->display_room();
	}
}
void commands::fighting_input_loop(bool& game_over, Player* player, Enemy* enemy)
{
	//std::vector<std::string> usr_text = prompt();
	//printUnderscore();
	//loop = parseInputVector(usr_text, game_over);

	//std::string noun = usr_text[0];

	std::string valid_attack_opt[] = { "1", "attack" };
	std::string valid_run_opt[] = { "2", "run" };

	bool valid_input = false;
	while (valid_input == false) {
		std::string usr_text = input();

		if (str_input_accepted(usr_text, valid_attack_opt, 2, valid_input)) {
			attack(game_over, player, enemy);
		}
		else if (str_input_accepted(usr_text, valid_run_opt, 2, valid_input)) {
			//run
			std::cout << "You cannot run from this encounter!\n";
		}
		else {
			invalid_input();
		}
	}
}
void commands::attack(bool& game_over, Player* player, Enemy* enemy)
{
	clear_();
	printUnderscore();

	//player attacks
	weapon* player_weapon = player->get_weapon();
	int player_damage = player_weapon->get_damage();
	player_damage = (player_damage * player_damage) / (player_damage + enemy->get_defense());//takes defense into account
	enemy->get_health()->damage(player_damage);
	std::cout << player->get_name() << " attacks with " << player_weapon->get_name() << " for "<<player_damage<<" damage.\n";

	if (!enemy->is_alive())
	{
		std::cout << "\nYou killed the " << enemy->get_name() << "!\n\n";
		return;
	}
		
	//enemy attacks
	weapon* enemy_weapon = enemy->get_weapon();
	int enemy_damage = enemy_weapon->get_damage();
	enemy_damage = (enemy_damage * enemy_damage) / (enemy_damage + player->get_defense());//takes defense into account
	player->get_health()->damage(enemy_damage);
	std::cout << enemy->get_name() << " attacks with " << enemy_weapon->get_name() << " for " << enemy_damage << " damage.\n\n";

	game_over = !(player->is_alive());
}