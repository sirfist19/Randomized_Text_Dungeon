#include <string>
#include "commands.h"

//the main game loop that controls everything
void commands::game_loop(commands* game, bool& game_over, bool& quit_to_title_screen, bool& first_time_enter)
{
	//check to make sure the player is still alive
	game_over = !(player->is_alive());
	if (game_over)
		return;

	bool loop = true;
	room* cur_room = player->get_cur_room();
	if (!first_time_enter)
	{
		display_cur_room_with_top_bar(cur_room);
	}
	else
	{
		first_time_enter = false;
	}
	
	fighting(cur_room, game_over, player);

	if (game_over)
		return;

	input_loop(loop, game_over, quit_to_title_screen);//handles all input
}

void commands::display_cur_room_with_top_bar(room* cur_room)
{
	player->printTopBar();
	cur_room->display_room();

	if (cur_room->get_store() != nullptr) //if there is a store
	{
		printUnderscore();

		store* cur_store = cur_room->get_store();
		cur_store->display_inventory();
		print("\n\nPlayer: " + player->get_name());
		print("Gold: " + std::to_string(player->get_amt_gold()));
		print();
		cur_store->display_options();
	}
}
//constructor
commands::commands(std::string player_name)
	:cur_noun(noun::error_), cur_verb(verb::error)
{
	//create the dungeon 
	do {
		Dungeon = new dungeon();
	} while ((Dungeon->get_size() < MIN_DUNGEON_SIZE) || (Dungeon->get_deepest_depth() < MIN_DEEPEST_DEPTH));


	//create the player
	room* start_room = Dungeon->get_start_room();
	player = new Player(player_name, start_room);

	//create the verb and noun charts
	verb_chart["go"] = verb::go;
	verb_chart["n"] = verb::go;
	verb_chart["north"] = verb::go;
	verb_chart["s"] = verb::go;
	verb_chart["south"] = verb::go;
	verb_chart["e"] = verb::go;
	verb_chart["east"] = verb::go;
	verb_chart["w"] = verb::go;
	verb_chart["west"] = verb::go;

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
	verb_chart["map"] = verb::map;

	noun_chart["north"] = noun::north;
	noun_chart["n"] = noun::north;
	noun_chart["south"] = noun::south;
	noun_chart["s"] = noun::south;
	noun_chart["east"] = noun::east;
	noun_chart["e"] = noun::east;
	noun_chart["west"] = noun::west;
	noun_chart["w"] = noun::west;

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
void commands::input_loop(bool& loop, bool& game_over, bool& quit_to_title_screen)
{
	if (game_over)
		return;

	while (loop)
	{
		cur_player_input = prompt();
		printUnderscore();

		bool res = false;
		store* cur_store = player->get_cur_room()->get_store();
		if (cur_store != nullptr)
		{
			res = store_input(cur_player_input, cur_store);//returns true if input is accepted
		}
		if(res == false)
			loop = parseInputVector(game_over, quit_to_title_screen);

		if (loop)
			printUnderscore();
	}
}
bool commands::parseInputVector(bool& game_over, bool& quit_to_title_screen)
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
	bool print_all_map = false;

	switch (cur_verb)//goes to all commands (returning false reprints the screen)
	{
	case verb::go:
		if ((cur_noun == noun::north) || (cur_player_input[0] == "n") || (cur_player_input[0] == "north"))
		{
			return go(0);
		}
		else if ((cur_noun == noun::south) || (cur_player_input[0] == "s") || (cur_player_input[0] == "south"))
		{
			return go(1);
		}
		else if ((cur_noun == noun::east) || (cur_player_input[0] == "e") || (cur_player_input[0] == "east"))
		{
			return go(2);
		}
		else if ((cur_noun == noun::west) || (cur_player_input[0] == "w") || (cur_player_input[0] == "west"))
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
		return use();
		break;
	case verb::map:
		if ((player_input_noun == "full") || (player_input_noun == "full depth"))
			print_all_map = true;
		map(print_all_map);
		break;
	case verb::jump:
		jump();
		if (!player->is_alive())
		{
			game_over = true;
			return false;
		}
		break;
	case verb::examine:
		examine();
		break;
	case verb::drink:
		drink();
		break;
	case verb::equip:
		equip();
		break;
	case verb::quit:
		quit_to_title_screen = true;
		print("\nYou quit the game and returned to the title screen. Play again or exit the game.\n");
		wait(5);
		clear_();
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
bool commands::store_input(std::vector<std::string>& cur_player_input, store* cur_store)
{
	//store input
	std::string valid_buy_opt[] = { "1", "buy" };
	std::string valid_talk_opt[] = { "2", "talk", "talk with shopkeeper" };
	std::string valid_normal_commands_opt[] = { "3" };

	bool valid_input = false;
	if (str_input_accepted(cur_player_input, valid_buy_opt, 2, valid_input)) 
	{
		//store buying function
		int gold_amt = player->get_amt_gold();
		int spent_amt = 0;
		std::vector<object*> bought_items = cur_store->buy_items(gold_amt, spent_amt);

		//decrease the money spent from the player's inventory
		player->decrease_gold_amt(spent_amt);

		
		//add bought items to the inventory
		player->add_items_to_inventory(bought_items);
		printUnderscore();
		print();
		cur_store->display_options();
		return true;
	}
	else if (str_input_accepted(cur_player_input, valid_talk_opt, 3, valid_input))
	{
		//talk function
		cur_store->talk_with_shopkeeper();
		print();
		cur_store->display_options();
		return true;
	}
	else if (str_input_accepted(cur_player_input, valid_normal_commands_opt, 1, valid_input))
	{
		print("Just use normal commands to continue.");
		return true;
	}	
	return false;
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
	clear_();
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



