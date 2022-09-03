#ifndef room_h
#define room_h
#include <iostream>
#include <string>
#include "helper_fxns.h"
#include "enemy.h"
#include "room_descriptons.h"
#include "chest.h"

enum depth_tier {
	near, mid, far, very_far, unassigned, start, any
};


class room {
	private:

		int id;//room_id
		std::string name;
		std::string description;
		int exits[4];//0 for no exit, other number for room to go to
		int num_exits;
		int depth;//smallest number of rooms from the start room to this room
		depth_tier tier;
		std::vector<Enemy*> enemies;
		std::vector<object*> items;
		chest* Chest;

	public:
		room(int id) //an extremely basic constructor ... more details are constructed later in the dungeon constructor
			:id(id), name("Unnamed Room"), description("It's a cold bare room."), num_exits(0), depth(-1), tier(depth_tier::unassigned), Chest(nullptr)
		{
			for (int i = 0; i < 4; i++)
			{
				exits[i] = 0;
			}
		}
		void place_chests()
		{
			int spawn_chest = random(0, 99);
			switch (tier)
			{
				case depth_tier::near:
					if(spawn_chest < NEAR_SPAWN_WOOD_CHEST)
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
		~room()
		{
			for (unsigned int i = 0; i < enemies.size(); i++)
			{
				delete enemies[i];
			}
		}
		chest* get_chest() const
		{
			return Chest;
		}
		std::vector<object*> get_items() const 
		{
			return items;
		}
		void add_item(object* in)
		{
			items.push_back(in);
		}
		int get_depth() const
		{
			return depth;
		}
		depth_tier get_tier() const
		{
			return tier;
		}
		void set_depth(int& in_depth)
		{
			if((depth == -1)&&(in_depth >= 0)) //if depth is not set yet
				depth = in_depth;
			else //if depth is already set
			{
				if ((in_depth > 0) && (in_depth < depth))//in_depth is less than the current depth
				{
					depth = in_depth;
				}
			}
		}
		void clear_items()
		{
			items.clear();
		}
		object* get_matching_object(std::string player_input_noun)
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
		void assign_room_type(depth_tier tier, room_descriptions* descriptions_holder)
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
					name = start_descriptions[0]->name;
					description = start_descriptions[0]->description;
					break;
				case depth_tier::near:
					max_range = near_descriptions.size() - 1;
					num = random(1, max_range);
					name = near_descriptions[num]->name;
					description = near_descriptions[num]->description;
					break;
				case depth_tier::mid:
					max_range = mid_descriptions.size() - 1;
					num = random(1, max_range);
					name = mid_descriptions[num]->name;
					description = mid_descriptions[num]->description;
					break;
				case depth_tier::far:
					max_range = far_descriptions.size() - 1;
					num = random(1, max_range);
					name = far_descriptions[num]->name;
					description = far_descriptions[num]->description;
					break;
				case depth_tier::very_far:
					max_range = very_far_descriptions.size() - 1;
					num = random(1, max_range);
					name = very_far_descriptions[num]->name;
					description = very_far_descriptions[num]->description;
					break;
			
				default:
					break;
			}
		}
		void set_depth_tier() 
		{
			if (DEBUG_MODE)
				std::cout << "Currently the depth is " << depth<<std::endl;
			if (depth == 0)
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
		void spawn_enemies()
		{
			//randomly spawn enemies
			int spawn_enemy_num = random(0, 99);
			int cur_num = 0;

			switch (tier)
			{
			case depth_tier::near:
				if(spawn_enemy_num < cur_num + NEAR_SPAWN_RAT)
					enemies.push_back(new Rat());
				else if (spawn_enemy_num < cur_num + NEAR_SPAWN_SLIME)
					enemies.push_back(new Slime());
				break;
			case depth_tier::mid:
				if (spawn_enemy_num < cur_num + MID_SPAWN_RAT)
					enemies.push_back(new Rat());
				else if (spawn_enemy_num < cur_num + MID_SPAWN_SLIME)
					enemies.push_back(new Slime());
				else if (spawn_enemy_num < cur_num + MID_SPAWN_SKELETON)
					enemies.push_back(new Skeleton());
				else if (spawn_enemy_num < cur_num + MID_SPAWN_GOBLIN)
					enemies.push_back(new Goblin());
				break;
			case depth_tier::far:
				if (spawn_enemy_num < cur_num + FAR_SPAWN_RAT)
					enemies.push_back(new Rat());
				else if (spawn_enemy_num < cur_num + FAR_SPAWN_SLIME)
					enemies.push_back(new Slime());
				else if (spawn_enemy_num < cur_num + FAR_SPAWN_SKELETON)
					enemies.push_back(new Skeleton());
				else if (spawn_enemy_num < cur_num + FAR_SPAWN_GOBLIN)
					enemies.push_back(new Goblin());
				else if (spawn_enemy_num < cur_num + FAR_SPAWN_ORC)
					enemies.push_back(new Orc());
				break;
			case depth_tier::very_far:
				if (spawn_enemy_num < cur_num + VERY_FAR_SPAWN_RAT)
					enemies.push_back(new Rat());
				else if (spawn_enemy_num < cur_num + VERY_FAR_SPAWN_PHANTOM_KNIGHT)
					enemies.push_back(new Phantom_Knight());
				else if (spawn_enemy_num < cur_num + VERY_FAR_SPAWN_ORC)
					enemies.push_back(new Orc());
				break;
			default:
				break;
			}
		}
		std::vector<Enemy*> get_enemies()
		{
			return enemies;
		}
		void set_enemies(std::vector<Enemy*> in)
		{
			enemies = in;
		}
		std::string get_name()
		{
			return name;
		}
		void set_num_exits(int num)
		{
			num_exits = num;
		}
		int get_num_exits()
		{
			return num_exits;
		}
		int* get_exits() {
			return exits;
		}
		void set_exits(int* in_exits) {
			for (int i = 0; i < 4; i++)
			{
				exits[i] = in_exits[i];
			}
		}
		int get_id() const
		{
			return id;
		}
		void display_room()
		{
			print(description);
			std::cout << "\n";
			if ((Chest != nullptr) || (items.size() != 0))
			{
				std::cout << "On the floor you find ";
				if (Chest != nullptr)
				{
					
					std::string chest_name = "a " + Chest->get_name();
					turn_to_lower_case(chest_name);
					std::cout << chest_name;
					if (items.size() != 0)
					{
						std::cout << ", ";
					}
				}
				if (items.size() != 0)
				{
					for (unsigned int i = 0; i < items.size(); i++)
					{
						int amt = items[i]->get_amt();
						if (i != items.size() - 1)
						{
							if (amt == 1)
								std::cout << "a " << items[i]->get_name() << ", ";
							else
								std::cout << amt << " " << items[i]->get_name() << "s, ";
						}
						else if (Chest != nullptr)
						{
							if (amt == 1)
								std::cout << "and a " << items[i]->get_name();
							else
								std::cout << "and " << amt << " " << items[i]->get_name();
						}
						else
						{
							if (amt == 1)
								std::cout << " a" << items[i]->get_name();
							else
								std::cout << amt << " " << items[i]->get_name();
						}
							
					}
				}
				std::cout << ".\n";
			}
			display_exit_information();

			if (id == 1)
			{
				std::cout << "\n";
				print("TIP: Don't know what to type. Type 'help' for more information.");
			}
		}
		
		void display_exit_information() const
		{
			int num = 0;
			for (int i = 0; i < 4; i++)
			{
				if (exits[i] != 0)
					num++;
			}
			
			//std::cout << exits[0] << exits[1] << exits[2] << exits[3];

			//std::cout << "\n";//add a newline before exit infomation is printed 
			if (num == 1)
			{
				std::cout << "There is an exit to the ";
				if (exits[0] != 0)
					std::cout << "North";
				if (exits[1] != 0)
					std::cout << "South";
				if (exits[2] != 0)
					std::cout << "East";
				if (exits[3] != 0)
					std::cout << "West";
			}
			else if (num == 2)
			{
				std::cout << "There are exits to the ";
				if (exits[3] != 0)
				{
					if (exits[0] != 0)
						std::cout << "North";
					if (exits[1] != 0)
						std::cout << "South";
					if (exits[2] != 0)
						std::cout << "East";
					if (exits[3] != 0)
						std::cout << " and West";
				}
				else if ((exits[3] == 0) && (exits[2] != 0))
				{
					if (exits[0] != 0)
						std::cout << "North";
					if (exits[1] != 0)
						std::cout << "South";
					if (exits[2] != 0)
						std::cout << " and East";
				}
				else if ((exits[3] == 0) && (exits[2] == 0))
				{
					if (exits[0] != 0)
						std::cout << "North";
					if (exits[1] != 0)
						std::cout << " and South";
				}
			}
			else if (num == 3)
			{
				std::cout << "There are exits to the ";
				if (exits[3] != 0)
				{
					if (exits[0] != 0)
					{
						if (exits[0] != 0)
							std::cout << "North";
						if (exits[1] != 0)
							std::cout << ", South ";
						if (exits[2] != 0)
							std::cout << ", East ";
						if (exits[3] != 0)
							std::cout << "and West";
					}
					else //if north is 0
					{
						if (exits[1] != 0)
							std::cout << "South";
						if (exits[2] != 0)
							std::cout << ", East ";
						if (exits[3] != 0)
							std::cout << "and West";
					}
				}
				else
				{
					if (exits[0] != 0)
						std::cout << "North";
					if (exits[1] != 0)
						std::cout << ", South ";
					if (exits[2] != 0)
						std::cout << "and East";
				}
			}
			else if (num == 4)
			{
				std::cout << "There are exits to the ";
				if (exits[0] != 0)
					std::cout << "North";
				if (exits[1] != 0)
					std::cout << ", South";
				if (exits[2] != 0)
					std::cout << ", East";
				if (exits[3] != 0)
					std::cout << " and West";
			}
			else
			{
				std::cout << "Num exits out of range: " << num_exits;
			}
			std::cout << ".\n";
		}
		void display_room_debug() const
		{
			printUnderscore();
			std::cout << "Room " <<id<<" - "<<name<< std::endl;
			std::cout << "Depth: " << depth<<std::endl;
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
			std::cout << "\n\nCHESTS: ";
			if (Chest != nullptr)
			{
				std::cout << "\nName: "<<Chest->get_name()<<"\n";
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
			}
			std::cout << "\n\nEXITS:\n";
			std::cout << "North: " << exits[0]<<std::endl;
			std::cout << "South: " << exits[1] << std::endl;
			std::cout << "East: " << exits[2] << std::endl;
			std::cout << "West: " << exits[3] << std::endl<< std::endl;
		}
};
#endif // !dungeon_h

