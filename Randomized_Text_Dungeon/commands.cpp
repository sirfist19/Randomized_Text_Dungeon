#include <string>
#include "commands.h"

void commands::game_loop(commands* game, bool& game_over)
{
	//check to make sure the player is still alive
	game_over = !(player->is_alive());
	if (game_over)
		return;

	bool loop = true;
	player->printTopBar();
	room* cur_room = player->get_cur_room();
	cur_room->display_room();

	fighting(cur_room, game_over, player);
	input_loop(loop, game_over);
}
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
	std::cout << player->get_name() << " attacks with " << player_weapon->get_name() << "\n";
	int player_damage = player_weapon->get_damage();
	enemy->get_health()->damage(player_damage);

	if (!enemy->is_alive())
	{
		std::cout << "\nYou killed the " << enemy->get_name() << "!\n\n";
		return;
	}
		
	//enemy attacks
	weapon* enemy_weapon = enemy->get_weapon();
	std::cout << enemy->get_name() << " attacks with " << enemy_weapon->get_name() << "\n\n";
	int enemy_damage = enemy_weapon->get_damage();
	player->get_health()->damage(enemy_damage);

	game_over = !(player->is_alive());
}
void commands::input_loop(bool& loop, bool& game_over)
{
	if (game_over)
		return;

	while (loop)
	{
		std::vector<std::string> usr_text = prompt();
		printUnderscore();
		loop = parseInputVector(usr_text, game_over);

		if (loop)
			printUnderscore();
	}
}
bool commands::parseInputVector(std::vector<std::string> in, bool& game_over)
{
	//set cur_noun and cur_verb based on user input
	//if not found in the maps then return false and print 
	//command string not found

	//returning false reprints the top bar and room info
	if (in.size() <= 0)
	{
		print("You cannot enter nothing. That's not a command. Please try again.");
		return true;
	}

	std::unordered_map<std::string, verb>::iterator gotVerb
		= verb_chart.find(in[0]);
	std::unordered_map<std::string, noun>::iterator gotNoun;
	if (in.size() > 1)
	{
		gotNoun = noun_chart.find(in[1]);
	}
	else //noun not typed
		cur_noun = noun::un_assigned;

	
	if (gotVerb == verb_chart.end())
	{
		std::cout << "Command not recognized! Please try again.\n";
		return true;
	}
	if ((in.size() > 1) && (gotNoun == noun_chart.end())) //if a noun was typed but not recognized
	{
		std::cout << "Object not recognized! Please try again.\n";
		return true;
	}

	cur_verb = gotVerb->second;
	if(in.size() > 1)
		cur_noun = gotNoun->second;

	if (DEBUG_MODE)
	{
		std::cout<<"Input Size: "<<in.size();
		std::cout << "Current Verb: " << cur_verb;
		std::cout << "\nCurrent Noun: " << cur_noun;
	}
	switch (cur_verb)
	{
	case verb::go:
		if (cur_noun == noun::north)
		{
			go(0);
			return false;
		}
		else if (cur_noun == noun::south)
		{
			go(1);
			return false;
		}
		else if (cur_noun == noun::east)
		{
			go(2);
			return false;
		}
		else if (cur_noun == noun::west)
		{
			go(3);
			return false;
		}
		else
			std::cout << "Direction of the go command not recognized.\n";
		break;
	case verb::clear:
		clear_();
		return false;
		break;
		//case verb::look:
		//	look(Player->get_cur_room());
		//	break;
	case verb::jump:
		jump();
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
	case verb::help:
		help();
		break;
	case verb::list:
		print_all_commands();
		break;
		//case verb::take:
		//	debug_print("In taking case");
		//	take(player->get_cur_room());
		//	break;
	default:
		print("Command recognized but not implemented");
		break;
	}
	return true;
}
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
	verb_chart["look"] = verb::look;
	verb_chart["take"] = verb::take;
	verb_chart["pickup"] = verb::take;
	verb_chart["quit"] = verb::quit;
	verb_chart["clear"] = verb::clear;
	verb_chart["open"] = verb::open;
	verb_chart["jump"] = verb::jump;
	verb_chart["info"] = verb::info;
	verb_chart["inventory"] = verb::inventory;
	verb_chart["hello"] = verb::hello;
	verb_chart["help"] = verb::help;
	verb_chart["list"] = verb::list;

	noun_chart["north"] = noun::north;
	noun_chart["south"] = noun::south;
	noun_chart["east"] = noun::east;
	noun_chart["west"] = noun::west;
	noun_chart["pit"] = noun::pit;
	noun_chart["chest"] = noun::_chest;
	
}
Player* commands::get_player()
{
	return player;
}
dungeon* commands::get_dungeon()
{
	return Dungeon;
}
commands::~commands()
{
	delete player;
}
void commands::open()
{
	chest* cur_room_chest = player->get_cur_room()->get_chest();

	if (cur_noun == noun::un_assigned)
	{
		std::cout << "You can't open nothing. Type an object for the open command.\n";
	}
	else if (cur_noun != noun::_chest)
	{
		std::cout<< "Object not recognized! Please try again.\n";
	}
	else if ((cur_room_chest != nullptr)&&(cur_noun == noun::_chest)) 
	{
		cur_room_chest->open();
	}
}
void commands::jump()
{
	std::string cur_room_name = player->get_cur_room()->get_name();
	
	if ((cur_room_name == "Pit Room") && (cur_noun == noun::pit))
	{
		print("You jump into the pit. After about 30 seconds of falling you realize it's a bottomless pit and you'll be falling for quite some time. Eventually you hit the bottom long after you reached terminal velocity. Your death happens instantaneously.");
		player->kill();
	}
	else {
		std::cout << "You jump high into the air, but nothing happens.\n";
	}
}
void commands::hello()
{
	std::cout << "Hello there! How do you do?";
	nl(2);
	wait(2);
	std::cout << "Oh wait, I'm not supposed to respond to questions. I'm the omniscient narrator after all; I could give something\naway ... ";
	wait(15);
	std::cout << "by accident.";
	wait(3);
	std::cout << " That would be bad.";
	nl(2);
	std::cout << "Get back to the task at hand! Enter another response...";
	nl(1);
}
void commands::help()
{
	using std::cout;
	using std::endl;
	print("When typing a command always use the syntax VERB then an OBJECT.");
	cout << "    NOTE: In certain commands objects are optional.\n\n";
	print("Example 1: 'examine key'");
	cout << "             VERB   OBJ\n";
	print("Example 2: 'look'");
	cout << "            VERB\n\n";
	print("To get a full list of commands type 'list'");


	/*cout << "1.look" << endl;
	cout << "2.quit" << endl;
	cout << "3.inventorygo" << endl;
	cout << "4.examine" << endl;
	cout << "5.take" << endl;
	cout << "6.inventory" << endl;
	cout << "7.drop" << endl;
	cout << "8.info" << endl;
	cout << "9.clear" << endl;
	cout << "10.open" << endl;
	cout << "11.jump" << endl;*/
}
void commands::print_all_commands()
{
	print("Note: Objects written in upper case letters are room specific and will not be included on this list. These names are still valid in commands.");
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
void commands::go(int index)
{
	//std::cout << "Going somewhere\n";
	room* player_cur_room = player->get_cur_room();
	int* cur_exits = player_cur_room->get_exits();

	int new_room_id = cur_exits[index];
	std::string dir_string = "";

	switch (index)
	{
		case 0: 
			dir_string = "north";
			break;
		case 1:
			dir_string = "south";
			break;
		case 2: 
			dir_string = "east";
			break;
		case 3:
			dir_string = "west";
			break;
	}
	if (new_room_id == 0)
	{
		std::cout << "There is no exit in that direction.\n";
	}
	else
	{
		int new_index = new_room_id - 1;
		//std::cout << "New index:" <<new_index;
		room* player_next_room = Dungeon->get_room(new_index);

		if (player_next_room == nullptr)
		{
			std::cout << "It's a nullptr again!";
		}

		
		player->set_location(player_next_room);
		std::cout << "Going " << dir_string << "...\n";
		wait(5);
		clear_command();
	}
}
//void commands::open(object* cur)
//{
//	debug_print("Opening");
//}
void commands::clear_command()
{
	clear_();
}
/*void commands::look(room* cur)
{
	print(cur->get_look_description());
}
void commands::take(room* cur)
{
	cur->set_look_descript(cur->get_dropped_objects());//reset the look description
}*/