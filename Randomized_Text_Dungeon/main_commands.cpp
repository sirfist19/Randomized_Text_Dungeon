#include <string>
#include "commands.h"

//all the commands themselves in alphabetical order
void commands::clear_command()
{
	clear_();
}
void commands::drink()
{
	object* obj_to_drink = player->get_matching_object(player_input_noun);

	if (cur_noun == noun::all)
	{
		std::vector<healing_potion*> inv_potions = player->get_inventory_healing_potions();
		if (inv_potions.empty())
		{
			print("You don't have any potions to drink.");
			return;
		}
		if (player->get_health()->health_is_full())
		{
			print("It won't have any effect.");
		}
		int i = 0;
		while ((!player->get_health()->health_is_full()) && (!inv_potions.empty()))
		{
			//drink individual potions
			healing_potion* cur = player->get_smallest_healing_potion();
			drink_individual_potion(cur);
			inv_potions = player->get_inventory_healing_potions();
		}
		return;
	}
	if (player_input_noun == "")
	{
		print("You can't drink nothing.");
		return;
	}
	if (obj_to_drink == nullptr)
	{
		print("That item is not in your inventory.");
		return;
	}
	drink_individual_potion(obj_to_drink);
}
void commands::drink_individual_potion(object* obj_to_drink)
{
	std::string identifier = obj_to_drink->identify();

	if (identifier == "healing potion")
	{
		healing_potion* cur = (healing_potion*)obj_to_drink;
		int heal_amt = cur->get_heal_amt();
		health_bar* player_health = player->get_health();
		int cur_health = player_health->get_health();

		if (player_health->health_is_full())
		{
			print("It will have no effect.");
			return;
		}
		player->delete_item_from_inventory(obj_to_drink, 1);
		player_health->heal(heal_amt);
		int healed_amt = player_health->get_health() - cur_health;
		std::cout << "You were healed by " << healed_amt << " health.\n";
	}
	else
	{
		print("You cannot drink that.");
		return;
	}
}
void commands::drop()
{
	object* obj_to_drop = player->get_matching_object(player_input_noun);

	if (player_input_noun == "")
	{
		print("You can't drop nothing.");
		return;
	}
	if ((obj_to_drop != nullptr) && (obj_to_drop->get_name() == "Fists"))
	{
		print("You can't drop your fists! What do you want to do ... cut your arms off or something?!");
		return;
	}
	if ((obj_to_drop == nullptr) && (cur_noun != noun::all))
	{
		print("That item is not in your inventory.");
		return;
	}
	//std::string identifier = obj_to_equip->identify();
	room* cur_room = player->get_cur_room();
	std::string before_string = "You dropped ";
	std::string after_string = " onto the floor.\n";

	if (cur_noun != noun::all)
	{
		int amt = obj_to_drop->get_amt();
		int amt_to_drop = 1;
		if (amt > 1)
		{
			std::cout << "There are " << amt << " of those in your inventory. How many do you want to drop?";
			std::string answer = "0";

			while (!is_number_in_range(answer, 1, amt))
			{
				answer = input();
				if (!is_number_in_range(answer, 1, amt))
				{
					print("You don't have that many of those. Type the amount again.");
				}
			}
			amt_to_drop = std::stoi(answer);
		}

		object* copy_obj_to_drop = obj_to_drop->clone();//copy the object to the room (while keeping the polymorphism typing!)
		copy_obj_to_drop->set_amt(amt_to_drop);//no duplicating items so set the correct amount that are dropping on the floor
		cur_room->add_item(copy_obj_to_drop);
		player->delete_item_from_inventory(obj_to_drop, amt_to_drop);

		print_item(copy_obj_to_drop, before_string, after_string);
	}
	else //cur_noun == noun::all (doesn't drop equipped items)
	{
		std::vector<object*> player_items = player->get_inventory();
		if (player_items.empty())
		{
			print("You have no items to drop.");
			return;
		}

		print_items(player_items, before_string, after_string);
		
		for (unsigned int i = 0; i < player_items.size(); i++)
		{
			cur_room->add_item(player_items[i]);
			player->delete_item_from_inventory_all(player_items[i]);
		}
	}
}
void commands::equip()
{
	object* obj_to_equip = player->get_matching_object(player_input_noun);

	if (player_input_noun == "")
	{
		print("You can't equip nothing.");
		return;
	}
	if ((obj_to_equip == nullptr) && (cur_noun != noun::all))
	{
		print("That item is not in your inventory.");
		return;
	}

	if (cur_noun == noun::all)
	{
		std::vector<object*> inventory = player->get_inventory();
		bool nothing_to_equip = true;
		unsigned int i = 0;
		while(i < inventory.size())
		{
			if (inventory.empty())
				break;
			std::string identifier = inventory[i]->identify();

			//equip if it is a weapon that is different from the current one and does more damage
			if ((identifier == "weapon") 
				&& (inventory[i]->get_name() != player->get_weapon()->get_name()) 
				&& (((weapon*)inventory[i])->get_light_damage() > player->get_weapon()->get_light_damage()))
			{
				equip_weapon(inventory[i]);
				nothing_to_equip = false;
				inventory = player->get_inventory();
				continue;
			}

			//equiping armor if there is no armor of that piece or if the specific piece is of a different type (the piece needs to have more defense)
			if (identifier == "helmet")
			{
				if ((player->get_helmet() == nullptr)
					||
					(
						(inventory[i]->get_name() != player->get_helmet()->get_name())
						&& (((helmet*)inventory[i])->get_defense() > player->get_helmet()->get_defense()))
					)
				{
					equip_armor((armor*)inventory[i]);
					nothing_to_equip = false;
					inventory = player->get_inventory();
					continue;
				}
			}
			if (identifier == "chestplate")
			{
				if ((player->get_chestplate() == nullptr)
					||
					(
						(inventory[i]->get_name() != player->get_chestplate()->get_name())
						&& (((chestplate*)inventory[i])->get_defense() > player->get_chestplate()->get_defense()))
					)
				{
					equip_armor((armor*)inventory[i]);
					nothing_to_equip = false;
					inventory = player->get_inventory();
					continue;
				}
			}
			if (identifier == "boots")
			{
				if ((player->get_boots() == nullptr)
					||
					(
						(inventory[i]->get_name() != player->get_boots()->get_name())
						&& (((boots*)inventory[i])->get_defense() > player->get_boots()->get_defense()))
					)
				{
					equip_armor((armor*)inventory[i]);
					nothing_to_equip = false;
					inventory = player->get_inventory();
					continue;
				}
			}
			i++;
		}
		if (nothing_to_equip)
			print("There's nothing to equip.");
		return;
	}
	std::string identifier = obj_to_equip->identify();

	if (identifier == "gold")
	{
		print("You cannot equip gold.");
		return;
	}
	if (identifier == "chest")
	{
		print("You cannot equip chests.");
		return;
	}
	if (identifier == "potion" || identifier == "healing potion")
	{
		print("You cannot equip potions.");
		return;
	}
	if (identifier == "weapon")
	{
		equip_weapon(obj_to_equip);
	}
	if ((identifier == "helmet") || (identifier == "chestplate") || (identifier == "boots"))
	{
		equip_armor((armor*)obj_to_equip);
	}
}
void commands::equip_armor(armor* obj_to_equip)
{
	std::string identifier = obj_to_equip->identify();
	armor* clone_obj_to_equip = obj_to_equip->clone_armor();
	armor* old_armor_piece = nullptr;

	if (identifier == "helmet")
	{
		old_armor_piece = player->get_helmet();
		player->set_helmet((helmet*)clone_obj_to_equip);
	}
	else if (identifier == "chestplate")
	{
		old_armor_piece = player->get_chestplate();
		player->set_chestplate((chestplate*)clone_obj_to_equip);
	}
	else if (identifier == "boots")
	{
		old_armor_piece = player->get_boots();
		player->set_boots((boots*)clone_obj_to_equip);
	}
	player->delete_item_from_inventory(obj_to_equip, 1);
	if (old_armor_piece != nullptr)
	{
		player->add_item_to_inventory(old_armor_piece);
		std::cout << "You equipped " << clone_obj_to_equip->get_name() << " and "<<old_armor_piece->get_name()<<" was placed in your inventory.\n";
	}
	else
	{
		std::cout << "You equipped " << clone_obj_to_equip->get_name() << ".\n";
	}
	player->compute_stats();
}
void commands::equip_weapon(object* obj_to_equip)
{
	std::string obj_to_equip_name = obj_to_equip->get_name();
	weapon* clone_obj_to_equip = ((weapon*)obj_to_equip)->clone_weapon();
	weapon* old_weapon = player->get_weapon();
	player->set_weapon(clone_obj_to_equip);
	player->delete_item_from_inventory(obj_to_equip, 1);

	if (old_weapon->get_name() != "Fists")
	{
		player->add_item_to_inventory(old_weapon);
		std::cout << "You equipped " << obj_to_equip_name <<
			" and " << old_weapon->get_name() << " has been placed into you inventory.\n";
	}
	else //if the player only had fists before
	{
		std::cout << "You equipped " << obj_to_equip_name << ".\n";
	}
}
void commands::examine()
{
	//if the object is in the player's inventory or equipped or in the room then...
	//print the name and description of the object
	room* cur_room = player->get_cur_room();
	chest* cur_chest = cur_room->get_chest();
	std::string chest_name = "";
	if(cur_chest != nullptr)
		chest_name = cur_chest->get_name();
	turn_to_lower_case(chest_name);
	object* match = nullptr; //the final match

	object* inventory_match = player->get_matching_object(player_input_noun);//from the player inventory (not equipped items yet)
	object* room_match = cur_room->get_matching_object(player_input_noun);
	object* chest_match = nullptr;
	if (cur_chest != nullptr)
	{
		chest_match = cur_chest->get_matching_object(player_input_noun);
	}
	
	if ((player_input_noun == "chest") || (player_input_noun == chest_name))
	{
		match = cur_chest;
	}
	if ((player_input_noun == "pit") && (cur_room->get_name() == "Pit Room"))
	{
		print("Pit");
		print("It's a pitch black giant gaping pit. If it's not bottomless, then it goes down a long ways.");
		return;
	}
	if ((player_input_noun == "pedistal") && (cur_room->get_name() == "Small Room"))
	{
		print("Pedistal");
		print("The pedistal is made out of a black volcanic stone. There is an inscription on it but you cannot read it.");
		return;
	}

	if (inventory_match != nullptr)
	{
		match = inventory_match;
	}
	else if (room_match != nullptr)
	{
		match = room_match;
	}
	else if (chest_match != nullptr)
	{
		match = chest_match;
	}
	
	if (match == nullptr)
	{
		print("There's nothing to examine.");
	}
	else
	{
		print(match->get_name() + ":");
		print(match->get_description());
	}
}
void commands::basic_map()
{
	std::vector<coord_and_id*> coords = Dungeon->get_sorted_room_coords();
	//clear_();
	//player->printTopBar();
	//player->get_cur_room()->display_room();

	//now draw the map
	print("DUNGEON MAP:\n");
	std::vector<std::string> lines;
	int cursor_x = 0;
	int cursor_y = 0;

	std::string cur = "";
	int x, y, id;

	for (unsigned int i = 0; i < coords.size(); i++)
	{
		x = coords[i]->coord->get_x();
		y = coords[i]->coord->get_y();
		id = coords[i]->id;
		room* cur_room = Dungeon->get_room(id - 1);

		if ((cursor_y == y) && (cursor_x == x))//we are on the same line
		{
			//add the room num
			if (cur_room->get_visited_status())
			{
				if (player->get_cur_room_id() == id)
					cur += "@";
				else
					cur += "o";
			}
			else
			{
				cur += " ";
			}
			
			//cur += std::to_string(coords[i]->id);//adds the number of the room to the map
			cursor_x++;
		}
		else if ((cursor_y == y) && (cursor_x != x))
		{
			cursor_x++;
			cur += " ";
			i--;//repeat the same coord
		}
		else if (cursor_y != y)
		{
			//push the previous line and start a new one
			lines.push_back(cur);
			cur = "";
			cursor_y++;
			cursor_x = 0;
			i--;//do the same coord again
		}
	}
	lines.push_back(cur);

	//draw the lines vector to draw the map
	for (unsigned int i = 0; i < lines.size(); i++)
	{
		std::cout << lines[i] << "\n";
	}
}
void commands::map(bool& print_all_map)
{
	Dungeon->create_sorted_room_coords(print_all_map);//set the coords again for the map adding the one that was just visited
	std::vector<coord_and_id*> coords = Dungeon->get_sorted_room_coords();

	//add in the connection characters to the coords vector
	//clear_();
	//player->printTopBar();
	//player->get_cur_room()->display_room();

	//now draw the map
	print("\t\t\t\t\t\tDUNGEON MAP:");
	print("\t\t\t\t\t\t\t\t\t\t\tKey: ");
	print("\t\t\t\t\t\t\t\t\t@ - player's current location, B - boss room");
	print("\t\t\t\t\t\t\t\t\tS - start room, X - dungeon exit, $ - store");
	print("\t\t\t\t\t\t\t\t\tE - live enemy still in room, W - wooden chest");
	print("\t\t\t\t\t\t\t\t\tG - gold chest, D - dragon chest, C - compass");

	std::vector<std::string> lines;
	int cursor_x = 0;
	int cursor_y = 0;

	std::string cur = "";
	int x, y, id;

	for (unsigned int i = 0; i < coords.size(); i++)
	{
		x = coords[i]->coord->get_x();
		y = coords[i]->coord->get_y();
		id = coords[i]->id;
		room* cur_room = nullptr;

		if ((id != -3) && (id != -4) && (id != -2))
			cur_room = Dungeon->get_room(id);
		if (id == -2)
		{
			cur_room = Dungeon->get_room(0);
		}

		if ((cursor_y == y) && (cursor_x == x))//we are on the same line
		{
			if (cur_room == nullptr) //if it is a connection
			{
				if(id == -3)
					cur += "|";
				if (id == -4)
					cur += "-";
			}
			else 
			{
				if((player_input_noun == "depth") || (player_input_noun == "full depth")) //the depth map
					cur += std::to_string(cur_room->get_depth());
				else 
				{
					int amt_enemies = cur_room->get_enemies().size();
					bool has_enemies = false;
					if (amt_enemies != 0)
						has_enemies = true;

					chest* cur_chest = cur_room->get_chest();
					std::string chest_name = "";
					if(cur_chest != nullptr)
						chest_name = cur_chest->get_name();

					compass* Compass = (compass*)cur_room->get_matching_object("compass");

					if (player->get_cur_room_id() == id)
						cur += "@";
					else if (id == 1)//start room
						cur += "S";
					else if (id == -2)
						cur += "X";
					else if (cur_room->get_name() == "Dragon's Lair")
						cur += "B";
					else if (Compass != nullptr)
						cur += "C";
					else if (amt_enemies > 0)
						cur += "E";
					else if ((cur_room->get_chest() != nullptr) && (chest_name == "Wooden Chest"))
						cur += "W";
					else if ((cur_room->get_chest() != nullptr) && (chest_name == "Gold Chest"))
						cur += "G";
					else if ((cur_room->get_chest() != nullptr) && (chest_name == "Dragon Chest"))
						cur += "D";
					else
						cur += "o";
				}
				
			}

			//cur += std::to_string(coords[i]->id);//adds the number of the room to the map
			cursor_x++;
		}
		else if ((cursor_y == y) && (cursor_x != x))
		{
			cursor_x++;
			cur += " ";
			i--;//repeat the same coord
		}
		else if (cursor_y != y)
		{
			//push the previous line and start a new one
			lines.push_back(cur);
			cur = "";
			cursor_y++;
			cursor_x = 0;
			i--;//do the same coord again
		}
	}
	lines.push_back(cur);

	//draw the lines vector to draw the map
	for (unsigned int i = 0; i < lines.size(); i++)
	{
		std::cout << lines[i] << "\n";
	}
}
bool commands::go(int index) //returning true doesn't reprint the room info and top bar, returning false does
{
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
		return true;
	}
	else if (new_room_id == -2)
	{
		print("The golden jade dragon doors still are locked. You will need to find and use a special key to open them.");
		return true;
	}
	else
	{
		int new_index = new_room_id;
		//std::cout << "New index:" <<new_index;
		room* player_next_room = Dungeon->get_room(new_index);

		if (player_next_room == nullptr)
		{
			std::cout << "It's a nullptr again!";
		}

		player_next_room->visited_cur_room();
		player->set_location(player_next_room);
		
		std::cout << "Going " << dir_string << "...\n";
		wait(5);
		clear_command();
		return false;//redisplays the room
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
	if (player_input_noun == "")
	{
		print("BASIC COMMANDS: ");
		print("1. go - Allows the player to move through the dungeon. Need to type a direction (Ex: north or east) as the object.");
		print("2. quit - Quits the game to the title screen. Restarts all progress.");
		print("3. inventory - Shows your health, current weapon, and items.");
		print("4. map - Displays a dungeon map of the areas you have already visited.");
		print("5. open - Opens chests.");
		print("6. take - Take objects off the ground or from chests.");
		print("7. equip - Equips items from the inventory into use.");
		print("8. examine - Gives more information on an object in the room, in a chest, or in your inventory.");
		print();
		print("- To get a full list of commands type 'list'.");
		print("- To find out more information about a specific command, type 'help' followed by the command you want to learn more about. Ex: help equip");
		print();

		print("SYNTAX: ");
		print("When typing a command always use the syntax VERB then an OBJECT.");
		std::cout << "    NOTE: In certain commands objects are optional.\n\n";
		print("Example 1: 'examine key'");
		std::cout << "             VERB   OBJ\n";
		print("Example 2: 'look'");
		std::cout << "            VERB\n\n";
	}
	else if (player_input_noun == "hello")
	{
		print("COMMAND: hello");
		print("Needs Object: No");
		print("If you want some company, then use this one. Talk to the narator. ");
	}
	else if (player_input_noun == "go")
	{
		print("COMMAND: go");
		print("Needs Object: Yes");
		print("Allows the player to move through the dungeon. Need to type a direction (Ex: north (n), south (s), east (e) or west (w)) as the object.");
		print("Ex: go north");
		print("Ex: go n");
	}
	else if (player_input_noun == "quit")
	{
		print("COMMAND: quit");
		print("Needs Object: No");
		print("Quits the game to the title screen. Restarts all progress.");
	}
	else if (player_input_noun == "map")
	{
		print("COMMAND: map");
		print("Needs Object: No");
		print("Displays a dungeon map of the areas you have already visited.");
		print("The @ represents the current position of the player. S is the start room, E is the exit to the dungeon, and B is the boss room.");
	}
	else if (player_input_noun == "examine")
	{
		print("COMMAND: examine");
		print("Needs Object: Yes");
		print("Takes all as an object: No");
		print("Gives more information on an object in the room, in a chest, or in your inventory.");
		print("Ex: examine chest");
		print("Ex: examine axe");
	}
	else if (player_input_noun == "inventory")
	{
		print("COMMAND: inventory");
		print("Needs Object: No");
		print("Shows your health, current weapon, and items.");
		print("\nOther relevant commands for using your inventory are:");
		print("\t-equip");
		print("\t-drink");
		print("\t-use");
		print("\t-examine");
	}
	else if (player_input_noun == "open")
	{
		print("COMMAND: open");
		print("Needs Object: Yes");
		print("Opens chests allowing the items inside the chests to be displayed and for the player to take the items inside. When a chest is already open, this will redisplay the chest's current items.");
		print("\nEx: open chest");
	}
	else if ((player_input_noun == "take") || (player_input_noun == "pickup"))
	{
		print("COMMAND: take/pickup");
		print("Needs Object: Yes");
		print("Takes all as an object: Yes");
		print("Takes items from already open chests or from the floor of the current room and places the items into the player's inventory. Using 'take all' picks up all items in the room and in already open chests in one command. To take one item at a time use the item you want to pick up's name as the object of this command.");
		print("\nEx: take all");
		print("Ex: take sword");
	}
	else if (player_input_noun == "drop")
	{
		print("COMMAND: drop");
		print("Needs Object: Yes");
		print("Takes all as an object: Yes");
		print("Drops items from the player's inventory onto the current room's floor. Using 'drop all' drops all items in the player's inventory. To drop one item at a time use the item you want to pick drop's name as the object of this command. If there is more than one object of that type in the player's inventory, you will be prompted to type the amount you want to drop.");
		print("\nEx: drop all");
		print("Ex: drop lesser healing potion");
	}
	else if (player_input_noun == "equip")
	{
		print("COMMAND: equip");
		print("Needs Object: Yes");
		print("Takes all as an object: Yes");
		print("Moves a weapon or armor from the player's inventory to the player's main weapon or main armor. The previous player's main weapon or armor is placed into the player's inventory. Using 'equip all' equips all of the weapons and armor in your inventory that do more damage or have more defense than what the player currently has. In essence, your equipped gear is maximized for damage and defense.");
		print("\nEx: equip all");
		print("Ex: equip bow");
	}
	else if (player_input_noun == "drink")
	{
		print("COMMAND: drink");
		print("Needs Object: Yes");
		print("Takes all as an object: Yes");
		print("Consumes a potion in the player's inventory and gives the player the effect. If the command 'drink all' is used, then the smallest healing potions will be drunk first healing the player to full health (if you have enough potions).");
		print("\nEx: drink greater healing potion");
		print("\nEx: drink all");
	}
	else if (player_input_noun == "jump")
	{
		print("COMMAND: jump");
		print("Needs Object: No (but an object may be used)");
		print("The player jumps into the air. If an object is used then the player jumps into or on that object.");
	}
	else if (player_input_noun == "list")
	{
		print("COMMAND: list");
		print("Needs Object: No");
		print("Prints out a list of all valid verbs and objects. Item names are also valid objects.");
	}
	else if (player_input_noun == "clear")
	{
		print("COMMAND: clear");
		print("Needs Object: No");
		print("Clears the screen.");
	}
	else if (player_input_noun == "use")
	{
		print("COMMAND: use");
		print("Needs Object: Yes");
		print("Takes all as an object: No");
		print("Uses keys. If a potion is the object, then the player drink the potion (see drink command).");
		print("\nEx: use key");
		print("Ex: use common healing potion");
	}
	else if (player_input_noun == "help")
	{
		print("COMMAND: help");
		print("Needs Object: No (but may be used)");
		print("Takes all as an object: No (but may take one)");
		print("You should already know what this one does because you are using it! Teaches the player how to play the game by showing them new commands. To find out more information about a specific command, type 'help' followed by the command you want to learn more about.");
		print("\nEx: help");
		print("Ex: help go");
	}
	else
	{
		print("Object not recognized. The object of the help command needs to be a command such as 'go' or 'clear'.");
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
void commands::open()
{
	chest* cur_room_chest = player->get_cur_room()->get_chest();

	if (cur_noun == noun::_none)
	{
		std::cout << "You can't open nothing. Type an object for the open command.\n";
	}
	else if (cur_noun != noun::_chest)
	{
		std::cout << "Object not recognized! Please try again.\n";
	}
	else if ((cur_room_chest != nullptr) && (cur_noun == noun::_chest))
	{
		cur_room_chest->open();
	}
}
void commands::take() //take objects either from the room's items or an open chest in the room
{
	//the noun is invalid
	if (cur_noun == noun::_none)
	{
		std::cout << "You can't take nothing. Type an object for the take command.\n";
		return;
	}

	//variables
	room* cur_room = player->get_cur_room();
	chest* cur_room_chest = cur_room->get_chest();
	std::vector<std::string> chest_content_names;
	open_method open_status;
	std::vector<object*> floor_contents = cur_room->get_items();

	object* obj_to_take_room = cur_room->get_matching_object_and_delete(player_input_noun);
	object* obj_to_take_chest = nullptr;
	if (cur_room_chest != nullptr)
	{
		obj_to_take_chest = cur_room_chest->get_matching_object_and_delete(player_input_noun);
		open_status = cur_room_chest->get_open_status();
		chest_content_names = cur_room_chest->get_all_content_names();
	}


	//if no object is recognized
	if ((obj_to_take_chest == nullptr) && (obj_to_take_room == nullptr) && (cur_noun != noun::all))
	{
		print("That object is not in this room.");
		return;
	}
	//if there are no items in the room and all is used
	bool no_items_to_take = ((cur_noun == noun::all) && (cur_room_chest == nullptr) && (floor_contents.size() == 0))
		|| ((cur_noun == noun::all) && (floor_contents.size() == 0) && (cur_room_chest != nullptr) && (cur_room_chest->is_empty()))
		|| ((cur_noun == noun::all) && (floor_contents.size() == 0) && (cur_room_chest != nullptr) && (cur_room_chest->get_open_status() != open_method::already_open));
	if (no_items_to_take)
	{
		print("There are no items in the room to take.");
		return;
	}

	std::string before_str = "You took ";
	std::string after_chest_str = " from the chest.\n";
	std::string after_floor_str = " from the floor.\n";

	//TAKING FROM THE ROOM FLOOR
	//taking only one item or one type of item 
	if ((obj_to_take_room != nullptr) && (cur_noun != noun::all))
	{
		//print what the player took
		print_item(obj_to_take_room, before_str, after_floor_str);
		

		//add the item to the player's inventory
		player->add_item_to_inventory(obj_to_take_room);
	}
	//taking all items from the floor
	else if ((cur_noun == noun::all) && (floor_contents.size() > 0))
	{
		print_items(floor_contents, before_str, after_floor_str);
		cur_room->clear_items();
		player->add_items_to_inventory(floor_contents);
	}

	//TAKING FROM A CHEST
	//taking only one item or one type of item
	if ((obj_to_take_chest != nullptr) && (open_status == open_method::already_open) && (cur_noun != noun::all))
	{
		//print what the player took
		std::string name = obj_to_take_chest->get_name();
		int amt = obj_to_take_chest->get_amt();
		print_item(obj_to_take_chest, before_str, after_chest_str);

		//add the item to the player's inventory
		player->add_item_to_inventory(obj_to_take_chest);
	}

	//there is a chest and it is open and the noun is all
	else if ((cur_room_chest != nullptr) && (open_status == open_method::already_open) && (cur_noun == noun::all))
	{
		//taking all chest contents
		std::vector<object*> chest_contents = cur_room_chest->get_all_contents();
		if (chest_contents.size() > 0)
		{
			
			print_items(chest_contents, before_str, after_chest_str);
			cur_room_chest->clear_chest_contents();
			player->add_items_to_inventory(chest_contents);
		}
	}

	//stack the player's inventory
	std::vector<object*> inventory = player->get_inventory();
	stack_objects(inventory);
	player->set_inventory(inventory);
}
void commands::use()
{
	object* obj_to_use = player->get_matching_object(player_input_noun);

	if (player_input_noun == "")
	{
		print("You can't equip nothing.");
		return;
	}
	if (obj_to_use == nullptr)
	{
		print("That item is not in your inventory.");
		return;
	}

	std::string identifier = obj_to_use->identify();
	if (identifier == "key")
	{
		obj_to_use->use(player->get_cur_room()->get_exits());
	}
	else if (identifier == "compass")
	{
		room* boss_room = Dungeon->get_boss_room();
		bool visited = boss_room->get_visited_status();

		if (!visited)
		{
			boss_room->visited_cur_room();
			print("You used the compass. With a shining light, the location of the boss room appeared on your map!");
			player->delete_item_from_inventory(obj_to_use, 1);
			wait(5);
			print("\nSeconds later, the compass disappeared right out of your hand. It must have server its purpose.");
		}
		else {
			print("The compass doesn't work because you already discovered the boss room.");
		}
	}
	else if ((identifier == "potion") || (identifier == "healing potion"))
	{
		//if a potion is used just drink it
		drink();
	}
	else
	{
		obj_to_use->use();
	}
}