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
	display_cur_room_without_top_bar(cur_room);
}

void commands::display_cur_room_without_top_bar(room* cur_room)
{
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
	:cur_noun(noun_type::noun_none),
	cur_verb(verb_type::verb_none)
	, cur_preposition(preposition_type::preposition_none),
	noun_class(new Noun()), verb_class(new Verb()), preposition_class(new Preposition()),
	cur_obj_orig_location(obj_location::loc_none)
{
	//create the dungeon 
	do {
		Dungeon = new dungeon();
	} while ((Dungeon->get_size() < MIN_DUNGEON_SIZE) || (Dungeon->get_deepest_depth() < MIN_DEEPEST_DEPTH));


	//create the player
	room* start_room = Dungeon->get_start_room();
	player = new Player(player_name, start_room);

	//add all object names 
	all_object_names.push_back(teleporter().get_name());
	all_object_names.push_back(fists().get_name());
	all_object_names.push_back(claws().get_name());
	all_object_names.push_back(broken_slime_shooter().get_name());
	all_object_names.push_back(slime_shooter().get_name());
	all_object_names.push_back(dagger().get_name());
	all_object_names.push_back(bow().get_name());
	all_object_names.push_back(shortsword().get_name());
	all_object_names.push_back(sword().get_name());
	all_object_names.push_back(long_bow().get_name());
	all_object_names.push_back(axe().get_name());
	all_object_names.push_back(nunchucks().get_name());
	all_object_names.push_back(katana().get_name());
	all_object_names.push_back(long_axe().get_name());
	all_object_names.push_back(daedric_sword().get_name());
	all_object_names.push_back(mjolnir().get_name());
	all_object_names.push_back(excalibur().get_name());
	all_object_names.push_back(dragon_fire().get_name());

	//terrain objects
	all_object_names.push_back(Waterfall().get_name());
	all_object_names.push_back(Pit().get_name());
	all_object_names.push_back(Pedistal().get_name());
	all_object_names.push_back(Moss().get_name());

	//potions
	all_object_names.push_back(lesser_healing_potion().get_name());
	all_object_names.push_back(common_healing_potion().get_name());
	all_object_names.push_back(greater_healing_potion().get_name());

	all_object_names.push_back(gold().get_name());
	all_object_names.push_back(dragon_key().get_name());
	all_object_names.push_back(compass().get_name());
	
	//armor
	all_object_names.push_back(chainmail_boots().get_name());
	all_object_names.push_back(chainmail_chestplate().get_name());
	all_object_names.push_back(chainmail_helmet().get_name());
	all_object_names.push_back(bronze_boots().get_name());
	all_object_names.push_back(bronze_chestplate().get_name());
	all_object_names.push_back(bronze_helmet().get_name());
	all_object_names.push_back(silver_boots().get_name());
	all_object_names.push_back(silver_chestplate().get_name());
	all_object_names.push_back(silver_helmet().get_name());
	all_object_names.push_back(dragon_scale_chestplate().get_name());
	all_object_names.push_back(dragon_scale_boots().get_name());
	all_object_names.push_back(dragon_scale_helmet().get_name());

	//chests
	all_object_names.push_back(wooden_chest(open_method::openable).get_name());
	all_object_names.push_back(gold_chest(open_method::openable).get_name());
	all_object_names.push_back(dragon_chest(open_method::openable).get_name());
	all_object_names.push_back("chest");
}

//destructor
commands::~commands()
{
	delete player;
	delete Dungeon;
	delete cur_obj;
	delete verb_class;
	delete noun_class;
	delete preposition_class;
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

	//first reset the previous verb, noun, prep, and desired object
	cur_verb = verb_type::verb_none;
	cur_preposition = preposition_type::preposition_none;
	cur_noun = noun_type::noun_none;
	cur_obj = nullptr;
	cur_obj_orig_location = obj_location::loc_none;

	//returning false reprints the top bar and room info
	if (cur_player_input.size() <= 0)
	{
		print("You cannot enter nothing. That's not a command. Please try again.");
		return true;
	}

	//create an vector that is the same size of cur_player_input and holds bools
	//if everything is recognized that the player typed, then all the bools should be turned to true
	std::vector<bool> isRecognized;
	for (unsigned int i = 0; i < cur_player_input.size(); i++)
	{
		isRecognized.push_back(false);
	}
	//go through the cur_player_input and assign the part of speech to each
	
	//FIND THE VERB
	int start_index = 0;//the start of the specific cut
	int ending_index = 0;
	while (cur_verb == verb_type::verb_none)
	{
		if (ending_index == cur_player_input.size())
		{
			print("Command not recognized! Please try again. If you are confused, type 'help' for more information.");
			return true; // loops and doesn't reprint screen
		}
		//create a new vector of only the words to search
		std::vector<std::string> verb_vector_to_find = 
			slice_vector(cur_player_input, start_index, ending_index);
		ending_index++;

		cur_verb = verb_class->get_verb_type(verb_vector_to_find);

		if (cur_verb != verb_type::verb_none)
		{
			//make everything from start to end index true for isRecognized
			for (int i = start_index; i < ending_index; i++)
			{
				isRecognized[i] = true;
			}
		}
	}
	int verb_ending_index = ending_index;
	start_index = ending_index;

	//FIND THE PREPOSITION if there is one
	while ((cur_preposition == preposition_type::preposition_none) && (ending_index != cur_player_input.size()))
	{
		std::vector<std::string> prep_vector_to_find =
			slice_vector(cur_player_input, start_index, ending_index);
		ending_index++;

		cur_preposition = preposition_class->get_preposition_type(prep_vector_to_find);

		if (cur_preposition != preposition_type::preposition_none)
		{
			//make everything from start to end index true for isRecognized
			for (int i = start_index; i < ending_index; i++)
			{
				isRecognized[i] = true;
			}
		}
	}

	if (ending_index == cur_player_input.size()) //if no preposition was found set it back to the end of the verb
		ending_index = verb_ending_index;
	else //set the start to the end of the preposition
		start_index = ending_index;

	//FIND THE NOUN if there is one
	bool object_is_an_item_name = false;
	bool at_least_one_object_is_an_item_name = false;

	while ((cur_noun == noun_type::noun_none) && (cur_obj == nullptr) && (ending_index != cur_player_input.size()))
	{
		std::vector<std::string> noun_vector_to_find =
			slice_vector(cur_player_input, start_index, ending_index);
		ending_index++;

		cur_noun = noun_class->get_noun_type(noun_vector_to_find);
		cur_obj = noun_class->get_matching_object(noun_vector_to_find, player, cur_obj_orig_location);

		//sees if the current string matches any object present in the game
		for (unsigned int i = 0; i < all_object_names.size(); i++)
		{
			std::string cur_name = all_object_names[i];
			turn_to_lower_case(cur_name);
			if (place_parsed_back_together(noun_vector_to_find) == cur_name)
			{
				object_is_an_item_name = true;
				at_least_one_object_is_an_item_name = true;
				break;
			}
		}

		//if the current string is recognized, change the isRecognized array to reflect that
		if ((cur_noun != noun_type::noun_none) || (cur_obj != nullptr) || (object_is_an_item_name == true))
		{
			//make everything from start to end index true for isRecognized
			for (int i = start_index; i < ending_index; i++)
			{
				isRecognized[i] = true;
			}
		}
		object_is_an_item_name = false;
	}

	bool isValidInput = true;
	for (unsigned int i = 0; i < isRecognized.size(); i++)
	{
		if (isRecognized[i] == false)
			isValidInput = false;
	}

	//if the object typed exists, the sentence makes sense, but no noun or cur_obj was found
	if ((isValidInput) && (at_least_one_object_is_an_item_name) && ((cur_noun == noun_type::noun_none) && (cur_obj == nullptr)))
	{
		print("That item is not in the room.");
		return true;
	}
	if (!isValidInput) 
	{
		if ((cur_noun == noun_type::noun_none) && (cur_obj == nullptr))
		{
			print("Unrecognized object.");
		}
		else
		{
			print("I don't understand that sentence.");
		}
		return true;//doesn't clear the screen
	}

	if (DISPLAY_PARSED_INFO)
	{
		std::cout << "\nVerb: " << cur_verb << std::endl;
		std::cout << "Preposition: " << cur_preposition << std::endl;
		std::cout << "Noun: " << cur_noun << std::endl;
		std::cout << "Cur obj: " << cur_obj << std::endl;
	}
	
	return pick_command_fxn(game_over, quit_to_title_screen);//picks the command using the selected verb, noun, and preposition
}
bool commands::pick_command_fxn(bool& game_over, bool& quit_to_title_screen)
{
	bool print_all_map = false;
	switch (cur_verb)//goes to all commands (returning false reprints the screen)
	{
	case verb_type::go:
		if ((cur_noun == noun_type::north) || (cur_player_input[0] == "n") || (cur_player_input[0] == "north"))
		{
			return go(0);
		}
		else if ((cur_noun == noun_type::south) || (cur_player_input[0] == "s") || (cur_player_input[0] == "south"))
		{
			return go(1);
		}
		else if ((cur_noun == noun_type::east) || (cur_player_input[0] == "e") || (cur_player_input[0] == "east"))
		{
			return go(2);
		}
		else if ((cur_noun == noun_type::west) || (cur_player_input[0] == "w") || (cur_player_input[0] == "west"))
		{
			return go(3);
		}
		else
			std::cout << "Direction of the go command not recognized.\n";
		break;
	case verb_type::clear:
		clear_();
		return false;
		break;
	case verb_type::use:
		return use();
		break;
	case verb_type::map:
		if ((cur_noun == noun_type::full) || (cur_noun == noun_type::full_depth))
			print_all_map = true;
		map(print_all_map);
		break;
	case verb_type::jump:
		jump();
		if (!player->is_alive())
		{
			game_over = true;
			return false;
		}
		break;
	case verb_type::look:
		display_cur_room_without_top_bar(player->get_cur_room());
		break;
	case verb_type::examine:
		examine();
		break;
	case verb_type::drink:
		drink();
		break;
	case verb_type::equip:
		equip();
		break;
	case verb_type::quit:
		quit_to_title_screen = true;
		print("\nYou quit the game and returned to the title screen. Play again or exit the game.\n");
		wait(5);
		clear_();
		return false;
		break;
	case verb_type::open:
		open();
		break;
	case verb_type::hello:
		hello();
		break;
	case verb_type::inventory:
		player->display_inventory();
		break;
	case verb_type::drop:
		drop();
		break;
	case verb_type::help:
		help();
		break;
	case verb_type::list:
		print_all_commands();
		break;
	case verb_type::take:
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
	print("All of a sudden the two golden doors begin to swing shut. You run towards the entrance, but it's too late. They've already closed. You push the door with all your might, but it won't budge. It looks like you'll need to figure out a way out on your own.");
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
	print("Function not working right now.");
	//int i = 1;
	//for (auto it = verb_chart.begin(); it != verb_chart.end(); it++)
	//{
	//	print(std::to_string(i) + ". " + it->first);
	//	i++;
	//}
}
void commands::print_all_nouns()
{
	print("Function not working right now.");
	//int i = 1;
	//for (auto it = noun_chart.begin(); it != noun_chart.end(); it++)
	//{
	//	print(std::to_string(i) + ". " + it->first);
	//	i++;
	//}
}



