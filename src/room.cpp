#include "room.h"
#include "db/rooms_db.h"
#include "static_object.h"
#include "static_object_factory.h"

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
				//game_out << "Combining " << in[i]->get_name() << " with " << in[j]->get_name();
				in[i]->combine(in[j]);//in[j] is now invalid

				//remove in[j]
				in[j] = in[in.size() - 1];//set in[j] to the last element of the vector
				in.pop_back();
			}
		}
	}
}

//room fxns
void room::remove_room_item(object* obj_to_remove)
{
	for (unsigned int i = 0; i < items.size(); i++)
	{
		if (obj_to_remove == items[i])
		{
			items[i] = items[items.size() - 1];
			items.pop_back();
			return;
		}
	}
}
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
object* room::get_matching_terrain_object(std::string player_input_noun)
{
	for (unsigned int i = 0; i < static_items.size(); i++)
	{
		std::string cur_name = static_items[i]->get_name();
		turn_to_lower_case(cur_name);

		if (player_input_noun == cur_name)
		{
			return static_items[i];
		}
	}
	return nullptr;
}
bool room::can_be_a_hallway()
{
	//count the number of zeros
	int zero_count = 0;
	for (int i = 0; i < 4; i++)
	{
		if (exits[i] == 0)
		{
			zero_count++;
		}
	}
	if (zero_count == 2)
	{
		return true;
	}
	return false;
}
/*
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
	room_description* cur_description = nullptr;
	bool valid = false; 
	switch (tier)
	{
	case depth_tier::start:
		if (depth == 0)
		{
			cur_description = new room_description(*start_descriptions[0]);
			name = cur_description->name;
			description = cur_description->description;
			static_items = cur_description->terrain;
			cur_description = nullptr;
			break;
		}
		//if has depth 0 then have near room description
	case depth_tier::near:
		while (!valid) {
			max_range = near_descriptions.size() - 1;
			num = random(0, max_range);
			cur_description = new room_description(*near_descriptions[num]);

			if ((cur_description->name == "Hallway") && (can_be_a_hallway()))//check for hallways
				valid = true;
			else if (cur_description->name != "Hallway")
				valid = true;
		}
		name = cur_description->name;
		description = cur_description->description;
		static_items = cur_description->terrain;
		cur_description = nullptr;
		break;
	case depth_tier::mid:
		while (!valid) {
			max_range = mid_descriptions.size() - 1;
			num = random(0, max_range);
			cur_description = new room_description(*mid_descriptions[num]);

			if ((cur_description->name == "Hallway") && (can_be_a_hallway()))//check for hallways
				valid = true;
			else if (cur_description->name != "Hallway")
				valid = true;
		}
		name = cur_description->name;
		description = cur_description->description;
		static_items = cur_description->terrain;
		cur_description = nullptr;
		break;
	case depth_tier::far:
		max_range = far_descriptions.size() - 1;
		num = random(0, max_range);
		cur_description = new room_description(*far_descriptions[num]);
		name = cur_description->name;
		description = cur_description->description;
		static_items = cur_description->terrain;
		cur_description = nullptr;
		break;
	case depth_tier::very_far:
		max_range = very_far_descriptions.size() - 1;
		num = random(0, max_range);
		cur_description = new room_description(*very_far_descriptions[num]);
		name = cur_description->name;
		description = cur_description->description;
		static_items = cur_description->terrain;
		cur_description = nullptr;
		break;

	default:
		break;
	}
}
*/


// helper to map enum -> string used in JSON
static std::string tier_to_string(depth_tier t) {
    switch (t) {
        case depth_tier::start:    return "start";
        case depth_tier::near:     return "near";
        case depth_tier::mid:      return "mid";
        case depth_tier::far:      return "far";
        case depth_tier::very_far: return "very_far";
        default:                   return "any";
    }
}

// helper: pick random element from vector (assumes non-empty)
static const std::string& pick_random_id(const std::vector<std::string>& ids) {
    int max_range = (int)ids.size() - 1;
    int idx = random(0, max_range);
    return ids[idx];
}

void room::assign_room_type(depth_tier tier)
{
	
    // 1) Entrance room special-case
    if (tier == depth_tier::start && depth == 0) {
        const RoomDef& def = RoomsDB::get("room.entrance"); // choose whatever id you used
        name = def.name;
        description = def.description;
        return;
    }

    // 2) Build pool of candidate room IDs
    // You can decide whether "any" rooms should be eligible in near/mid/far pools.
    // Right now your JSON probably has explicit tiers, so just use the tier pool.
    const std::string tierStr = tier_to_string(tier);

    std::vector<std::string> candidates = RoomsDB::ids_for_tier(tierStr);

    // Optional: also allow "any" tier rooms everywhere except start
    {
        std::vector<std::string> anyIds = RoomsDB::ids_for_tier("any");
        candidates.insert(candidates.end(), anyIds.begin(), anyIds.end());
    }

    if (candidates.empty()) {
        // Fail fast with a helpful message
        throw std::runtime_error("No room candidates found for tier: " + tierStr);
    }

    // 3) Pick a valid room, preserving your hallway constraint
    bool valid = false;
    const RoomDef* chosen = nullptr;

    while (!valid) {
        const std::string& id = pick_random_id(candidates);
        const RoomDef& def = RoomsDB::get(id);

        if (def.name == "Hallway") {
            valid = can_be_a_hallway();
        } else {
            valid = true;
        }

        if (valid) chosen = &def;
    }

    // 4) Assign
    name = chosen->name;
	description = chosen->description;

	static_items.clear();
	for (const std::string& obj_id : chosen->object_ids)
	{
		static_items.push_back(create_static_object(obj_id));
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
		game_out << "Currently the depth is " << depth << std::endl;
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

	std::string terrain_items_string = "";
	for (unsigned int i = 0; i < static_items.size(); i++)
	{
		terrain_items_string += static_items[i]->get_revealed_items_description();
	}
	
	game_out << "\n";

	if (terrain_items_string != "")
	{
		print_no_newline(terrain_items_string);
		game_out << " ";
	}

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
		game_out << "\n";
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
			game_out << "The locked gold jade doors that lead to the exit of the dungeon lies to your ";
			switch (i)
			{
			case 0:
				game_out << "North.\n";
				break;
			case 1:
				game_out << "South.\n";
				break;
			case 2:
				game_out << "East.\n";
				break;
			case 3:
				game_out << "West.\n";
				break;
			default:
				game_out << i << "room.h  - display_exit_information index error.\n";
				break;
			}
		}
	}

	if (num == 1)
	{
		game_out << "There is an exit to the ";
		if (exits[0] > 0)
			game_out << "North";
		if (exits[1] > 0)
			game_out << "South";
		if (exits[2] > 0)
			game_out << "East";
		if (exits[3] > 0)
			game_out << "West";
	}
	else if (num == 2)
	{
		game_out << "There are exits to the ";
		if (exits[3] > 0)
		{
			if (exits[0] > 0)
				game_out << "North";
			if (exits[1] > 0)
				game_out << "South";
			if (exits[2] > 0)
				game_out << "East";
			if (exits[3] > 0)
				game_out << " and West";
		}
		else if ((exits[3] <= 0) && (exits[2] > 0))
		{
			if (exits[0] > 0)
				game_out << "North";
			if (exits[1] > 0)
				game_out << "South";
			if (exits[2] > 0)
				game_out << " and East";
		}
		else if ((exits[3] <= 0) && (exits[2] <= 0))
		{
			if (exits[0] > 0)
				game_out << "North";
			if (exits[1] > 0)
				game_out << " and South";
		}
	}
	else if (num == 3)
	{
		game_out << "There are exits to the ";
		if (exits[3] > 0)
		{
			if (exits[0] > 0)
			{
				if (exits[0] > 0)
					game_out << "North";
				if (exits[1] > 0)
					game_out << ", South ";
				if (exits[2] > 0)
					game_out << ", East ";
				if (exits[3] > 0)
					game_out << "and West";
			}
			else //if north is 0
			{
				if (exits[1] > 0)
					game_out << "South";
				if (exits[2] > 0)
					game_out << ", East ";
				if (exits[3] > 0)
					game_out << "and West";
			}
		}
		else
		{
			if (exits[0] > 0)
				game_out << "North";
			if (exits[1] > 0)
				game_out << ", South ";
			if (exits[2] > 0)
				game_out << "and East";
		}
	}
	else if (num == 4)
	{
		game_out << "There are exits to the ";
		if (exits[0] > 0)
			game_out << "North";
		if (exits[1] > 0)
			game_out << ", South";
		if (exits[2] > 0)
			game_out << ", East";
		if (exits[3] > 0)
			game_out << " and West";
	}
	else
	{
		game_out << "Num exits out of range: " << num_exits;
	}
	game_out << ".\n";
}
void room::display_room_debug() const
{
	printUnderscore();
	game_out << "Room " << id << " - " << name << std::endl;
	game_out << "Coord: ";
	location->display();

	game_out << "\nDepth: " << depth << std::endl;
	game_out << "Depth Tier: ";
	switch (tier)
	{
	case depth_tier::start:
		game_out << "start";
		break;
	case depth_tier::near:
		game_out << "near";
		break;
	case depth_tier::mid:
		game_out << "mid";
		break;
	case depth_tier::far:
		game_out << "far";
		break;
	case depth_tier::very_far:
		game_out << "very far";
		break;
	case depth_tier::unassigned:
		game_out << "unassigned";
		break;
	default:
		break;
	}
	game_out << "\n\nITEMS: ";
	if (items.empty())
		game_out << "NONE";
	for (unsigned int i = 0; i < items.size(); i++)
	{
		game_out << items[i]->get_name();
	}
	game_out << "\n\nTERRAIN: ";
	if (static_items.empty())
		game_out << "NONE";
	for (unsigned int i = 0; i < static_items.size(); i++)
	{
		game_out << static_items[i]->get_name();
	}
	game_out << "\n\nCHESTS: ";
	if (Chest != nullptr)
	{
		game_out << "\nName: " << Chest->get_name() << "\n";
		Chest->display_chest_contents();
	}
	else
	{
		game_out << "NONE";
	}
	game_out << "\nENEMIES: ";
	if (enemies.empty())
		game_out << "NONE";
	for (unsigned int i = 0; i < enemies.size(); i++)
	{
		game_out << "\n";
		enemies[i]->display_attack_info();
		game_out << "Exp to drop: " << enemies[i]->get_exp();
	}
	game_out << "\n\nEXITS:\n";
	game_out << "North: " << exits[0] << std::endl;
	game_out << "South: " << exits[1] << std::endl;
	game_out << "East: " << exits[2] << std::endl;
	game_out << "West: " << exits[3] << std::endl << std::endl;
}
bool room::has_terrain(object* in)
{
	for (unsigned int i = 0; i < static_items.size(); i++)
	{
		if (in == static_items[i])
			return true;
	}
	return false;
}
