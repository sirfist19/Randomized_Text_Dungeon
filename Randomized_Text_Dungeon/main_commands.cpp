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

	if (player_input_noun == "")
	{
		print("You can't equip nothing.");
		return;
	}
	if (obj_to_drink == nullptr)
	{
		print("That item is not in your inventory.");
		return;
	}
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
	std::string player_input_noun = get_player_input_noun();
	object* obj_to_drop = player->get_matching_object(player_input_noun);

	if (player_input_noun == "")
	{
		print("You can't drop nothing.");
		return;
	}
	if ((obj_to_drop == nullptr) && (cur_noun != noun::all))
	{
		print("That item is not in your inventory.");
		return;
	}
	//std::string identifier = obj_to_equip->identify();
	room* cur_room = player->get_cur_room();

	if (cur_noun != noun::all)
	{
		int amt = obj_to_drop->get_amt();
		int amt_to_drop = 1;
		if (amt > 1)
		{
			std::cout << "Ther are " << amt << " of those in your inventory. How many do you want to drop?";
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

		std::cout << "You dropped " << amt_to_drop << " " << obj_to_drop->get_name() << " onto the floor.\n";
	}
	else //cur_noun == noun::all
	{
		std::vector<object*> player_items = player->get_inventory();
		if (player_items.empty())
		{
			print("You have no items to drop.");
			return;
		}

		std::string item_names = "";
		for (unsigned int i = 0; i < player_items.size(); i++)
		{
			item_names += player_items[i]->get_name();

			if (i != player_items.size() - 1)
				item_names += ", ";

			cur_room->add_item(player_items[i]);
			player->delete_item_from_inventory_all(player_items[i]);
		}
		std::cout << "You dropped " << item_names << " onto the floor.\n";
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
			inventory = player->get_inventory();
			if (inventory.empty())
				break;
			std::string identifier = inventory[i]->identify();

			//equip if it is a weapon that is different from the current one and does more damage
			if ((identifier == "weapon") 
				&& (inventory[i]->get_name() != player->get_weapon()->get_name()) 
				&& (((weapon*)inventory[i])->get_damage() > player->get_weapon()->get_damage()))
			{
				equip_weapon(inventory[i]);
				nothing_to_equip = false;
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
	armor* old_armor_piece = nullptr;

	if (identifier == "helmet")
	{
		old_armor_piece =player->get_helmet();
		player->set_helmet((helmet*)obj_to_equip);
	}
	else if (identifier == "chestplate")
	{
		old_armor_piece = player->get_chestplate();
		player->set_chestplate((chestplate*)obj_to_equip);
	}
	else if (identifier == "boots")
	{
		old_armor_piece = player->get_boots();
		player->set_boots((boots*)obj_to_equip);
	}
	player->delete_item_from_inventory(obj_to_equip, 1);
	if (old_armor_piece != nullptr)
	{
		player->add_item_to_inventory(old_armor_piece);
		std::cout << "You equipped " << obj_to_equip->get_name() << " and "<<old_armor_piece->get_name()<<" was placed in your inventory.\n";
	}
	else
	{
		std::cout << "You equipped " << obj_to_equip->get_name() << ".\n";
	}
	player->compute_stats();
}
void commands::equip_weapon(object* obj_to_equip)
{
	weapon* old_weapon = player->get_weapon();
	player->set_weapon(obj_to_equip);
	player->delete_item_from_inventory(obj_to_equip, 1);

	if (old_weapon->get_name() != "Fists")
	{
		player->add_item_to_inventory(old_weapon);
		std::cout << "You equipped " << obj_to_equip->get_name() <<
			" and " << old_weapon->get_name() << " has been placed into you inventory.\n";
	}
	else //if the player only had fists before
	{
		std::cout << "You equipped " << obj_to_equip->get_name() << ".\n";
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
		print("2. quit - Simply quits the game.");
		print("3. inventory - Shows your health, current weapon, and items.");
		print("4. open - Opens chests.");
		print("5. take - Take objects off the ground or from chests.");
		print("6. equip - Equips items from the inventory into use.");
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
		print("Allows the player to move through the dungeon. Need to type a direction (Ex: north, south, east or west) as the object.");
		print("Ex: go north");
	}
	else if (player_input_noun == "quit")
	{
		print("COMMAND: quit");
		print("Needs Object: No");
		print("Quits the game.");
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
		print("Takes all as an object: Not yet");
		print("Consumes a potion in the player's inventory and gives the player the effect.");
		print("\nEx: drink greater healing potion");
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

	object* obj_to_take_room = cur_room->get_matching_object(player_input_noun);
	object* obj_to_take_chest = nullptr;
	if (cur_room_chest != nullptr)
	{
		obj_to_take_chest = cur_room_chest->get_matching_object(player_input_noun);
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

	//TAKING FROM THE ROOM FLOOR
	//taking only one item or one type of item 
	if ((obj_to_take_room != nullptr) && (cur_noun != noun::all))
	{
		//print what the player took
		std::string name = obj_to_take_room->get_name();
		int amt = obj_to_take_room->get_amt();
		std::cout << "You took ";

		if (amt > 1)
		{
			std::cout << amt << " ";
		}
		else
		{
			switch (name[0])
			{
			case 'a':
			case 'e':
			case 'i':
			case 'o':
			case 'u':
				std::cout << "an ";
			default:
				std::cout << "a ";
			}
		}

		std::cout << name << " from the floor.\n";

		//add the item to the player's inventory
		player->add_item_to_inventory(obj_to_take_room);
	}
	//taking all items from the floor
	else if ((cur_noun == noun::all) && (floor_contents.size() > 0))
	{

		std::cout << "You took ";

		for (unsigned int i = 0; i < floor_contents.size(); i++)
		{
			std::string name = floor_contents[i]->get_name();
			int amt = floor_contents[i]->get_amt();

			if ((i == floor_contents.size() - 1) && (i != 0))
				std::cout << "and ";

			if (amt > 1)
			{
				std::cout << amt << " ";
			}
			else
			{
				switch (name[0])
				{
				case 'a':
				case 'e':
				case 'i':
				case 'o':
				case 'u':
					std::cout << "an ";
				default:
					std::cout << "a ";
				}
			}

			if ((floor_contents.size() == 1) || (i == floor_contents.size() - 1))
				std::cout << name;
			else
				std::cout << name << ", ";
		}
		std::cout << " from the floor.\n";
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
		std::cout << "You took ";

		if (amt > 1)
		{
			std::cout << amt << " ";
		}
		else
		{
			switch (name[0])
			{
			case 'a':
			case 'e':
			case 'i':
			case 'o':
			case 'u':
				std::cout << "an ";
			default:
				std::cout << "a ";
			}
		}
		std::cout << name << " from the chest.\n";

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
			std::cout << "You took ";

			for (unsigned int i = 0; i < chest_contents.size(); i++)
			{
				std::string name = chest_contents[i]->get_name();
				int amt = chest_contents[i]->get_amt();

				if ((i == floor_contents.size() - 1) && (i != 0))
					std::cout << "and ";

				if (amt > 1)
				{
					std::cout << amt << " ";
				}
				else
				{
					switch (name[0])
					{
					case 'a':
					case 'e':
					case 'i':
					case 'o':
					case 'u':
						std::cout << "an ";
					default:
						std::cout << "a ";
					}
				}

				if ((chest_contents.size() == 1) || (i == chest_contents.size() - 1))
					std::cout << name;
				else
					std::cout << name << ", ";
			}
			std::cout << " from the chest.\n";
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