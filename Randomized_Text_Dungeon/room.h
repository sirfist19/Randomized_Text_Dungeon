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

void print_items(std::vector<object*>& items, std::string before, std::string after);
void print_item(object* item, std::string before, std::string after);
void stack_objects(std::vector<object*>& in);

class room_coord //distance to east and north
{
private:
	int x;
	int y;
public:
	room_coord(int _x, int _y)
	{
		x = _x;
		y = _y;
	}
	~room_coord(){}
	room_coord(const room_coord& in)//copy constructor
	{
		x = in.x;
		y = in.y;
	}
	room_coord()
	{
		x = 0;
		y = 0;
	}
	void set_coord(int& in_x, int& in_y)
	{
		x = in_x;
		y = in_y;
	}
	void display()
	{
		std::cout << "(" << x << ", " << y << ") ";
	}
	int get_x()
	{
		return x;
	}
	int get_y()
	{
		return y;
	}
	room_coord add(room_coord a, room_coord b)
	{
		a.x = a.x + b.x;
		a.y = a.y + b.y;
		return a;
	}
	bool is_equal(room_coord& a, room_coord& b)
	{
		if ((a.x == b.x) && (a.y == b.y))
			return true;
		return false;
	}
};

class room {
	private:

		int id;//room_id
		std::string name;
		std::string description;
		room_coord* location;
		bool visited;//has the player visited here before
		int exits[4];//0 for no exit, other number for room to go to
		int num_exits;
		int depth;//smallest number of rooms from the start room to this room
		depth_tier tier;
		std::vector<Enemy*> enemies;
		std::vector<object*> items;
		chest* Chest;

	public:
		room(int id) //an extremely basic constructor ... more details are constructed later in the dungeon constructor
			:id(id), name("Unnamed Room"), description("It's a cold bare room."), 
			num_exits(0), depth(-1), tier(depth_tier::unassigned), Chest(nullptr), visited(false)
			, location(new room_coord())
		{
			if (id == 1)
				visited = true;
			for (int i = 0; i < 4; i++)
			{
				exits[i] = 0;
			}
		}
		~room()
		{
			for (unsigned int i = 0; i < enemies.size(); i++)
			{
				delete enemies[i];
			}
			for (unsigned int i = 0; i < items.size(); i++)
			{
				delete items[i];
			}
			delete location;
			delete Chest;
		}
		
		//essential fxns
		object* get_matching_object(std::string player_input_noun);
		object* get_matching_object_and_delete(std::string player_input_noun);
		void assign_room_type(depth_tier tier, room_descriptions* descriptions_holder);
		void set_depth_tier();
		void spawn_enemies();
		void place_chests();
		void display_room();
		void display_exit_information() const;
		void display_room_debug() const;
		
		//getters
		std::string get_name()
		{
			return name;
		}
		int get_num_exits()
		{
			return num_exits;
		}
		int* get_exits() {
			return exits;
		}
		int get_id() const
		{
			return id;
		}
		chest* get_chest() const
		{
			return Chest;
		}
		bool get_visited_status() const
		{
			return visited;
		}
		room_coord* get_coord()
		{
			return location;
		}
		std::vector<object*> get_items() const
		{
			return items;
		}
		int get_depth() const
		{
			return depth;
		}
		depth_tier get_tier() const
		{
			return tier;
		}
		std::vector<Enemy*> get_enemies()
		{
			return enemies;
		}
		
		//setters
		void visited_cur_room()
		{
			visited = true;
		}
		void clear_chests()
		{
			//deletes chests in the room if there are any
			Chest = nullptr;
		}
		void set_coord(room_coord* coord)
		{
			location = coord;
		}
		void set_name(std::string in_name)
		{
			name = in_name;
		}
		void add_item(object* in)
		{
			items.push_back(in);
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
		void set_enemies(std::vector<Enemy*> in)
		{
			enemies = in;
		}

		void set_info(std::string in_name, std::string in_description)
		{
			name = in_name;
			description = in_description;
		}
		void set_num_exits(int num)
		{
			num_exits = num;
		}
		void set_exits(int* in_exits) {
			for (int i = 0; i < 4; i++)
			{
				exits[i] = in_exits[i];
			}
		}
};
#endif // !dungeon_h

