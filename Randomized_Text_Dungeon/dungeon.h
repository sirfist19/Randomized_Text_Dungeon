#ifndef dungeon_h
#define dungeon_h
#include "room.h"
#include "player.h"
#include <vector>
#include <stack>
#include <unordered_map>
#include <cstdlib>

struct coord_and_id 
{
	room_coord* coord;
	int id;//for rooms is the id, -2 for the exit, -3 for north and south connections, -4 for east and west connections 

	coord_and_id(room_coord* in_coord, int in_id) : coord(in_coord), id(in_id) {}
	~coord_and_id()
	{
		delete coord;
	}
	bool equal(coord_and_id* other)
	{
		if ((other->id == id) && (coord->is_equal(*coord, *other->coord)))
			return true;
		return false;
	}
};

class dungeon 
{
	private:
		std::vector<room*> rooms;
		std::stack<room*> rooms_to_give_exits;//for dungeon generation
		room_descriptions* stock_room_descriptions;
		std::unordered_map<room_coord, int, room_coord_hasher> occupied_coords;
		std::vector<coord_and_id*> sorted_room_coords;//for map making
	public:
		dungeon();
		~dungeon() //destructor
		{
			for (unsigned int i = 0; i < rooms.size(); i++)
			{
				delete rooms[i];
			}
			for (unsigned int i = 0; i < sorted_room_coords.size(); i++)
			{
				delete sorted_room_coords[i];
			}
			delete stock_room_descriptions;

			//std::cout << rooms_to_give_exits.size();
			while(!rooms_to_give_exits.empty())//should be empty but if it is not delete everything
			{
				rooms_to_give_exits.pop();
			}
		}
		int get_size()
		{
			return rooms.size();
		}
		room* get_room(int index)
		{
			//std::cout << "Index: " << index<<std::endl;
			//rooms[index]->display_room();
			return rooms[index];
		}
		std::vector<coord_and_id*> get_sorted_room_coords() const
		{
			return sorted_room_coords;
		}
		room* get_start_room() const
		{
			for (unsigned int i = 0; i < rooms.size(); i++)
			{
				if (rooms[i]->get_id() == 1)
					return rooms[i];
			}
			return nullptr;
		}
		room* get_boss_room() const
		{
			for (unsigned int i = 0; i < rooms.size(); i++)
			{
				if (rooms[i]->get_name() == "Dragon's Lair")
					return rooms[i];
			}
			return nullptr;
		}
		room* get_store_room() const
		{
			for (unsigned int i = 0; i < rooms.size(); i++)
			{
				if (rooms[i]->get_store() != nullptr)
					return rooms[i];
			}
			return nullptr;
		}
		//void dfs() const;
		void create_sorted_room_coords(bool& print_all_map);
		void create_sorted_room_coords_basic();
		bool duplicate_coord_and_id(coord_and_id* posid);
		void dfs_set_depth() const;
		void bfs_set_depth() const;
		bool no_two_rooms_have_same_coord() const; //a debugging fxn
		room* find_adj_room(int& index, room* cur_room) const; //finds any adjacent non-connected room
		bool in_visited(std::vector<room*> visited, int id) const
		{
			for (unsigned int i = 0; i < visited.size(); i++)
			{
				int cur_id = visited[i]->get_id();
				if (cur_id == id)
					return true;
			}
			return false;
		}
		room_coord get_coord(int& index) const;
		void place_dragon_key();
		void place_items();
		void place_store();
		int get_deepest_depth();
		void display_all_rooms_coords();
		void create_new_exits(room* cur_room, room_descriptions* descriptions);
		int get_opposite_exit(int exit_num);
		void display_debug();
		void display_dungeon_info();
};
#endif 

