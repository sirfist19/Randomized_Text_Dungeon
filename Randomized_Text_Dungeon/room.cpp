#include "room.h"

//general functions at the room level
void print_items(std::vector<object*>& items, std::string before, std::string after)
{
	std::string res = before;
	for (unsigned int i = 0; i < items.size(); i++)
	{
		std::string name = items[i]->get_name();
		std::string identifier = items[i]->identify();
		int amt = items[i]->get_amt();

		std::string before_string = "";
		std::string after_string = "";

		if ((i == items.size() - 1) && (i != 0))
			before_string += "and ";

		if (amt > 1) //many items
		{
			if (identifier == "boots")
			{
				before_string += (std::to_string(amt) + " pairs of ");
			}
			else if (identifier == "gold") 
			{
				before_string += (std::to_string(amt) + " ");
				//no plural
			}
			else
			{
				before_string += (std::to_string(amt) + " ");
				after_string += "s";
			}
		}
		else //only one item
		{
			if (identifier == "boots")
			{
				before_string += "a pair of ";
			}
			else if (identifier == "gold")
			{
				before_string += "1 ";
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
						before_string += "an ";
					default:
						before_string += "a ";
				}
			}
		}

		if ((items.size() != 1) && (i != items.size() - 1))
			after_string += ", ";

		res += (before_string + name + after_string);
	}
	res += after;//add on the after string argument made in the previous function that called this one
	
	//actually print the items
	print_no_newline(res);
}
void print_item(object* item, std::string before, std::string after) //a wrapper for print_items for one item
{
	std::vector<object*> items;
	if(item != nullptr)
		items.push_back(item);
	print_items(items, before, after);
}
//stack objects on the floor or in the player's inventory
void stack_objects(std::vector<object*>& in)
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
//room fxns
object* room::get_matching_object_and_delete(std::string player_input_noun)
{
	for (unsigned int i = 0; i < items.size(); i++)
	{
		std::string cur_name = items[i]->get_name();
		turn_to_lower_case(cur_name);

		if (player_input_noun == cur_name)
		{
			object* result = items[i];

			//to remove the object from the chest move the last vector item to it's place and 
			//then pop_back()
			items[i] = items[items.size() - 1];
			items.pop_back();
			return result;
		}
	}
	return nullptr;
}
object* room::get_matching_object(std::string player_input_noun)
{
	for (unsigned int i = 0; i < items.size(); i++)
	{
		std::string cur_name = items[i]->get_name();
		turn_to_lower_case(cur_name);

		if (player_input_noun == cur_name)
		{
			return items[i];
		}
	}
	return nullptr;
}
void room::assign_room_type(depth_tier tier, room_descriptions* descriptions_holder)
{
	std::vector<room_description*> start_descriptions =
		descriptions_holder->get_start_room_descriptions();
	std::vector<room_description*> near_descriptions =
		descriptions_holder->get_near_room_descriptions();
	std::vector<room_description*> mid_descriptions =
		descriptions_holder->get_mid_room_descriptions();
	std::vector<room_description*> far_descriptions =
		descriptions_holder->get_far_room_descriptions();
	std::vector<room_description*> very_far_descriptions =
		descriptions_holder->get_very_far_room_descriptions();
	int max_range;
	int num;

	switch (tier)
	{
	case depth_tier::start:
		if (depth == 0)
		{
			name = start_descriptions[0]->name;
			description = start_descriptions[0]->description;
			break;
		}
		//if has depth 0 then have near room description
	case depth_tier::near:
		max_range = near_descriptions.size() - 1;
		num = random(0, max_range);
		name = near_descriptions[num]->name;
		description = near_descriptions[num]->description;
		break;
	case depth_tier::mid:
		max_range = mid_descriptions.size() - 1;
		num = random(0, max_range);
		name = mid_descriptions[num]->name;
		description = mid_descriptions[num]->description;
		break;
	case depth_tier::far:
		max_range = far_descriptions.size() - 1;
		num = random(0, max_range);
		name = far_descriptions[num]->name;
		description = far_descriptions[num]->description;
		break;
	case depth_tier::very_far:
		max_range = very_far_descriptions.size() - 1;
		num = random(0, max_range);
		name = very_far_descriptions[num]->name;
		description = very_far_descriptions[num]->description;
		break;

	default:
		break;
	}
}
void room::place_chests()
{
	int spawn_chest = random(0, 99);
	switch (tier)
	{
	case depth_tier::start:
		if((depth != 0) && (spawn_chest < START_SPAWN_WOOD_CHEST))
			Chest = new wooden_chest(open_method::openable);
	case depth_tier::near:
		if ((depth != 0) &&(spawn_chest < NEAR_SPAWN_WOOD_CHEST))
			Chest = new wooden_chest(open_method::openable);
		break;
	case depth_tier::mid:
		if (spawn_chest < MID_SPAWN_WOOD_CHEST)
			Chest = new wooden_chest(open_method::openable);
		else if (spawn_chest < MID_SPAWN_GOLD_CHEST + MID_SPAWN_WOOD_CHEST)
			Chest = new gold_chest(open_method::openable);
		break;
	case depth_tier::far:
		if (spawn_chest < FAR_SPAWN_GOLD_CHEST)
			Chest = new gold_chest(open_method::openable);
		break;
	case depth_tier::very_far:
		if (spawn_chest < VERY_FAR_SPAWN_DRAGON_CHEST)
			Chest = new dragon_chest(open_method::openable);
		else if (spawn_chest < VERY_FAR_SPAWN_GOLD_CHEST + VERY_FAR_SPAWN_DRAGON_CHEST)
			Chest = new gold_chest(open_method::openable);
		break;
	default:
		break;
	}
}
void room::spawn_enemies()
{
	//randomly spawn enemies
	int spawn_enemy_num = random(0, 99);
	int cur_num = 0;

	switch (tier)
	{
	case depth_tier::near:
		if (spawn_enemy_num < cur_num + NEAR_SPAWN_RAT)
			enemies.push_back(new Rat(1));
		else if (spawn_enemy_num < cur_num + NEAR_SPAWN_SLIME)
			enemies.push_back(new Slime(1));
		break;
	case depth_tier::mid:
		if (spawn_enemy_num < cur_num + MID_SPAWN_RAT)
			enemies.push_back(new Rat(2));
		else if (spawn_enemy_num < cur_num + MID_SPAWN_SLIME)
			enemies.push_back(new Slime(2));
		else if (spawn_enemy_num < cur_num + MID_SPAWN_SKELETON)
			enemies.push_back(new Skeleton(1));
		else if (spawn_enemy_num < cur_num + MID_SPAWN_GOBLIN)
			enemies.push_back(new Goblin(1));
		break;
	case depth_tier::far:
		if (spawn_enemy_num < cur_num + FAR_SPAWN_RAT)
			enemies.push_back(new Rat(3));
		else if (spawn_enemy_num < cur_num + FAR_SPAWN_SLIME)
			enemies.push_back(new Slime(3));
		else if (spawn_enemy_num < cur_num + FAR_SPAWN_SKELETON)
			enemies.push_back(new Skeleton(2));
		else if (spawn_enemy_num < cur_num + FAR_SPAWN_GOBLIN)
			enemies.push_back(new Goblin(2));
		else if (spawn_enemy_num < cur_num + FAR_SPAWN_ORC)
			enemies.push_back(new Orc(1));
		break;
	case depth_tier::very_far:
		if (spawn_enemy_num < cur_num + VERY_FAR_SPAWN_RAT)
			enemies.push_back(new Rat(4));
		else if (spawn_enemy_num < cur_num + VERY_FAR_SPAWN_PHANTOM_KNIGHT)
			enemies.push_back(new Phantom_Knight(1));
		else if (spawn_enemy_num < cur_num + VERY_FAR_SPAWN_ORC)
			enemies.push_back(new Orc(2));
		break;
	default:
		break;
	}
}
void room::set_depth_tier()
{
	if (DEBUG_MODE)
		std::cout << "Currently the depth is " << depth << std::endl;
	if ((depth >= MIN_START_AREA) && (depth <= MAX_START_AREA))
		tier = depth_tier::start;
	else if ((depth >= MIN_NEAR) && (depth <= MAX_NEAR))
		tier = depth_tier::near;
	else if ((depth >= MIN_MID) && (depth <= MAX_MID))
		tier = depth_tier::mid;
	else if ((depth >= MIN_FAR) && (depth <= MAX_FAR))
		tier = depth_tier::far;
	else if (depth >= MIN_VERY_FAR)
		tier = depth_tier::very_far;
	else
		tier = depth_tier::unassigned;
}
std::string room::get_depth_tier_string()
{
	switch (tier)
	{
	case depth_tier::start:
	case depth_tier::near:
		return "Reception Area";
	case depth_tier::mid:
		return "Civilization's End";
	case depth_tier::far:
		return "Dungeon Depths";
	case depth_tier::very_far:
		return "Dungeon Deep";
	default:
		return "error";
	}
}
void room::display_room()
{
	stack_objects(items);
	print(description);

	std::cout << "\n";
	std::string before_string = "";
	std::string after_string = ".\n\n";
	if ((Chest != nullptr) || (items.size() != 0))
	{
		before_string += "On the floor you find ";
		if (Chest != nullptr)
		{
			std::string chest_name = ("a " + Chest->get_name());
			turn_to_lower_case(chest_name);
			before_string += chest_name;
			if (items.size() != 0)
			{
				before_string += ", ";
			}
		}
		print_items(items, before_string, after_string);
	}
	display_exit_information();

	if (id == 1)
	{
		std::cout << "\n";
		print("TIP: Don't know what to type? Type 'help' for more information.");
	}
}
void room::display_exit_information() const
{
	int num = 0;
	for (int i = 0; i < 4; i++)
	{
		if (exits[i] > 0)
			num++;
		else if (exits[i] == -2)
		{
			std::cout << "The locked gold jade doors that lead to the exit of the dungeon lies to your ";
			switch (i)
			{
			case 0:
				std::cout << "North.\n";
				break;
			case 1:
				std::cout << "South.\n";
				break;
			case 2:
				std::cout << "East.\n";
				break;
			case 3:
				std::cout << "West.\n";
				break;
			default:
				std::cout << i << "room.h  - display_exit_information index error.\n";
				break;
			}
		}
	}

	if (num == 1)
	{
		std::cout << "There is an exit to the ";
		if (exits[0] > 0)
			std::cout << "North";
		if (exits[1] > 0)
			std::cout << "South";
		if (exits[2] > 0)
			std::cout << "East";
		if (exits[3] > 0)
			std::cout << "West";
	}
	else if (num == 2)
	{
		std::cout << "There are exits to the ";
		if (exits[3] > 0)
		{
			if (exits[0] > 0)
				std::cout << "North";
			if (exits[1] > 0)
				std::cout << "South";
			if (exits[2] > 0)
				std::cout << "East";
			if (exits[3] > 0)
				std::cout << " and West";
		}
		else if ((exits[3] <= 0) && (exits[2] > 0))
		{
			if (exits[0] > 0)
				std::cout << "North";
			if (exits[1] > 0)
				std::cout << "South";
			if (exits[2] > 0)
				std::cout << " and East";
		}
		else if ((exits[3] <= 0) && (exits[2] <= 0))
		{
			if (exits[0] > 0)
				std::cout << "North";
			if (exits[1] > 0)
				std::cout << " and South";
		}
	}
	else if (num == 3)
	{
		std::cout << "There are exits to the ";
		if (exits[3] > 0)
		{
			if (exits[0] > 0)
			{
				if (exits[0] > 0)
					std::cout << "North";
				if (exits[1] > 0)
					std::cout << ", South ";
				if (exits[2] > 0)
					std::cout << ", East ";
				if (exits[3] > 0)
					std::cout << "and West";
			}
			else //if north is 0
			{
				if (exits[1] > 0)
					std::cout << "South";
				if (exits[2] > 0)
					std::cout << ", East ";
				if (exits[3] > 0)
					std::cout << "and West";
			}
		}
		else
		{
			if (exits[0] > 0)
				std::cout << "North";
			if (exits[1] > 0)
				std::cout << ", South ";
			if (exits[2] > 0)
				std::cout << "and East";
		}
	}
	else if (num == 4)
	{
		std::cout << "There are exits to the ";
		if (exits[0] > 0)
			std::cout << "North";
		if (exits[1] > 0)
			std::cout << ", South";
		if (exits[2] > 0)
			std::cout << ", East";
		if (exits[3] > 0)
			std::cout << " and West";
	}
	else
	{
		std::cout << "Num exits out of range: " << num_exits;
	}
	std::cout << ".\n";
}
void room::display_room_debug() const
{
	printUnderscore();
	std::cout << "Room " << id << " - " << name << std::endl;
	std::cout << "Coord: ";
	location->display();

	std::cout << "\nDepth: " << depth << std::endl;
	std::cout << "Depth Tier: ";
	switch (tier)
	{
	case depth_tier::start:
		std::cout << "start";
		break;
	case depth_tier::near:
		std::cout << "near";
		break;
	case depth_tier::mid:
		std::cout << "mid";
		break;
	case depth_tier::far:
		std::cout << "far";
		break;
	case depth_tier::very_far:
		std::cout << "very far";
		break;
	case depth_tier::unassigned:
		std::cout << "unassigned";
		break;
	default:
		break;
	}
	std::cout << "\n\nITEMS: ";
	if (items.empty())
		std::cout << "NONE";
	for (unsigned int i = 0; i < items.size(); i++)
	{
		std::cout << items[i]->get_name();
	}
	std::cout << "\n\nCHESTS: ";
	if (Chest != nullptr)
	{
		std::cout << "\nName: " << Chest->get_name() << "\n";
		Chest->display_chest_contents();
	}
	else
	{
		std::cout << "NONE";
	}
	std::cout << "\nENEMIES: ";
	if (enemies.empty())
		std::cout << "NONE";
	for (unsigned int i = 0; i < enemies.size(); i++)
	{
		std::cout << "\n";
		enemies[i]->display_attack_info();
		std::cout << "Exp to drop: " << enemies[i]->get_exp();
	}
	std::cout << "\n\nEXITS:\n";
	std::cout << "North: " << exits[0] << std::endl;
	std::cout << "South: " << exits[1] << std::endl;
	std::cout << "East: " << exits[2] << std::endl;
	std::cout << "West: " << exits[3] << std::endl << std::endl;
}
