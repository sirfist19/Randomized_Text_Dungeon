#ifndef dungeon_h
#define dungeon_h
#include "room.h"
#include "player.h"
#include <vector>
#include <stack>
#include <cstdlib>

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
		room_coord()
		{
			x = 0;
			y = 0;
		}
		void display()
		{
			std::cout << "(" << x << ", " << y << ") ";
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

class room_info 
{ //used for the DFS through the maze
	private:
		room_coord pos;
		room* cur_room;
	public:
		room_info(room_coord _pos, room* _cur_room) {
			pos = _pos;
			cur_room = _cur_room;
		}
		room_coord get_pos()
		{
			return pos;
		}
		room* get_room() {
			return cur_room;
		}
};


class dungeon {
	private:
		std::vector<room*> rooms;
		std::stack<room*> rooms_to_give_exits;//for dungeon generation
		room_descriptions* stock_room_descriptions;
	public:
		dungeon();
		~dungeon() //destructor
		{
			for (unsigned int i = 0; i < rooms.size(); i++)
			{
				delete rooms[i];
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
		room* get_start_room()
		{
			return rooms[0];
		}
		void dfs(std::vector<room_info>& visited, room* cur_room) const;
		void dfs_set_depth() const;
		bool no_two_rooms_have_same_coord(std::vector<room_info> visited) const; //a debugging fxn
		room* find_adj_room(int& index, room* cur_room) const; //finds any adjacent non-connected room
		bool in_visited(std::vector<room_info> visited, int id) const
		{
			for (unsigned int i = 0; i < visited.size(); i++)
			{
				int cur_id = visited[i].get_room()->get_id();
				if (cur_id == id)
					return true;
			}
			return false;
		}
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
		void create_new_exits(room* cur_room, room_descriptions* descriptions);
		int get_opposite_exit(int exit_num);
		void display_debug();
};
#endif 

