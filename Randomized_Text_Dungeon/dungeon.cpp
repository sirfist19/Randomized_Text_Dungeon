#include "dungeon.h"
#include "enemy.h"
#include <algorithm>
#include <queue>

dungeon::dungeon() 
{
	room_descriptions* stock_room_descriptions = new room_descriptions();
	srand((unsigned int)time(NULL));//sets random numbers

	//for the first room
	room* temp_room = new room(rooms.size() + 1);//sets coord to 0,0
	/*temp_room->add_item(new lesser_healing_potion());
	temp_room->add_item(new lesser_healing_potion());
	temp_room->add_item(new lesser_healing_potion());
	temp_room->add_item(new lesser_healing_potion());
	temp_room->add_item(new lesser_healing_potion());
	temp_room->add_item(new bronze_boots());
	temp_room->add_item(new bronze_boots());
	temp_room->add_item(new bronze_helmet());
	temp_room->add_item(new sword());
	temp_room->add_item(new gold(87));
	temp_room->add_item(new gold(1));*/
	//temp_room->add_item(new compass());

	rooms.push_back(temp_room);
	create_new_exits(temp_room, stock_room_descriptions);

	//create the dungeon as a skeleton model
	while ((rooms.size() < MAX_DUNGEON_SIZE) && (!rooms_to_give_exits.empty()))
	{
		room* cur_room = rooms_to_give_exits.top();
		rooms_to_give_exits.pop();

		create_new_exits(cur_room, stock_room_descriptions);
		std::cout << "Generated dungeon of size " << rooms.size()<<"\n";
	}
	
	//empty the rooms to give exits stack - its not needed anymore
	while (!rooms_to_give_exits.empty())
	{
		rooms_to_give_exits.pop();
	}

	//fill the dungeon with stuff
	bfs_set_depth();
	for (unsigned int i = 0; i < rooms.size(); i++)
	{
		room* cur_room = rooms[i];
		
		cur_room->set_depth_tier();
		cur_room->spawn_enemies();
		cur_room->assign_room_type(cur_room->get_tier(), stock_room_descriptions);
		cur_room->place_chests();
	}
	place_dragon_key();
	place_items();
	place_store();
}
void dungeon::create_new_exits(room* cur_room, room_descriptions* descriptions)
{
	if (cur_room->get_id() == 1) //entrance room
	{
		//first add the exit to the dungeon through the golden dragon door
		int exit_num = random(0, 3);
		int* cur_exits = cur_room->get_exits();
		cur_exits[exit_num] = -2;
		cur_room->set_exits(cur_exits);

		//create the start room and add its coordinate
		room* exit_room = new room(-2);
		room_coord* exit_pos = new room_coord(get_coord(exit_num));
		exit_room->visited_cur_room();//make the exit room show up on the map
		exit_room->set_coord(exit_pos);
		exit_room->set_name("Exit Room");
		rooms.insert(rooms.begin(), exit_room);//but the exit room as the first room

		//set the remaining exits
		cur_room->set_num_exits(random(1,3));
	}
	else
	{
		cur_room->set_num_exits(random(0,3));
	}

	int num_exits = cur_room->get_num_exits();
	int* cur_exits = cur_room->get_exits();

	for (int i = 0; i < num_exits; i++)
	{
		if (rooms.size() < MAX_DUNGEON_SIZE)
		{
			int index;
			int iterations = 0;
			while (true) //finds an index whose exit has no connection
			{
				index = random(0,3);//0 to 3
				if (cur_exits[index] == 0)
					break;
				iterations++;
				if (iterations > 100) //no infinite looping
					return;
			}

			if (DEBUG_MODE)
			{
				this->display_debug();

				//std::cout << "---------Displaying a room---------\n";
				//std::cout << "Current Room: " << cur_room->get_id()<<"\n";
				//std::cout << "Want a room with index " << index <<std::endl;
				//if (adj_room != nullptr)
				//	adj_room->display_room();
				//else
				//	std::cout << "Nullptr\n";
				//std::cout << "-----------------------------------\n";*/
			}
			room* adj_room = find_adj_room(index, cur_room);


			if ((cur_exits[index] == 0) && (adj_room == nullptr))//there is no exit and no adjacent room in the given direction
			{
				//create a new room
				room* temp_room = new room(rooms.size());

				//update cur_room's exit
				cur_exits[index] = temp_room->get_id();
				cur_room->set_exits(cur_exits);

				//update new room's exit
				int new_room_exit = get_opposite_exit(index);
				int* temp_exits = temp_room->get_exits();
				temp_exits[new_room_exit] = cur_room->get_id();
				temp_room->set_exits(temp_exits);

				//update the new room's coord
				room_coord delta_coord = get_coord(index);
				room_coord* cur_room_coord = cur_room->get_coord();
				room_coord* new_room_coord = new room_coord(cur_room_coord->add(*cur_room_coord, delta_coord));
				temp_room->set_coord(new_room_coord);

				//add it to the rooms vector
				rooms.push_back(temp_room);

				//add it to the room_to_give_exits stack
				rooms_to_give_exits.push(temp_room);
			}
			else if ((cur_exits[index] == 0))//there is no exit, but there is an adjacent room in a given direction
			{
				//a random chance to connect to that room
				int chance = random(0,99); //0 to 9
				int adj_room_id = adj_room->get_id();
				if ((chance < CONNECT_ROOMS_CHANCE) && (adj_room_id != -2)) //do not connect if the adj room is the exit room
				{
					//connect the current room
					cur_exits[index] = adj_room_id;

					//connect the adjacent room
					int* adj_room_exits = adj_room->get_exits();
					int adj_index = get_opposite_exit(index);
					adj_room_exits[adj_index] = cur_room->get_id();
					adj_room->set_exits(adj_room_exits);
				}
			}
		}
	}
}
void dungeon::display_all_rooms_coords()
{
	for (unsigned int i = 0; i < sorted_room_coords.size(); i++)
	{
		std::cout << "Room " << sorted_room_coords[i]->id << " with coord ";
		sorted_room_coords[i]->coord->display();
		std::cout << "\n";
	}
}
void dungeon::create_sorted_room_coords_basic()
{
	//create the vector
	for (unsigned int i = 0; i < rooms.size(); i++)
	{
		room_coord* cur_coord = rooms[i]->get_coord();
		int cur_id = rooms[i]->get_id();
		coord_and_id* cur = new coord_and_id(cur_coord, cur_id);
		sorted_room_coords.push_back(cur);
	}

	//sort the vector
	std::sort(sorted_room_coords.begin(), sorted_room_coords.end(), [](coord_and_id* a, coord_and_id* b) {
		// sort by x
		//return a->get_x() > b->get_x();
		// sort by y
		//return a->get_y() > b->get_y();

		//we want to return true if a > b
		int a_x = a->coord->get_x();
		int a_y = a->coord->get_y();
		int b_x = b->coord->get_x();
		int b_y = b->coord->get_y();

		//a = (0, -1) smaller
		//b = (0, 2) larger

		//a = (1, -1) larger
		//b = (0, -1) smaller

		if (b_y < a_y)
			return true;
		else if (b_y > a_y)
			return false;
		else //if they are equal
		{
			if (a_x < b_x)
				return true;
		}
		return false;
	});

	//do a transformation to make the coords easy to process
	int minX = INT_MAX;
	int maxY = INT_MIN;

	//get the minX and maxY
	for (unsigned int i = 0; i < sorted_room_coords.size(); i++)
	{
		int x = sorted_room_coords[i]->coord->get_x();
		int y = sorted_room_coords[i]->coord->get_y();
		if (x < minX)
			minX = x;
		if (y > maxY)
			maxY = y;
	}

	//transform the vector
	for (unsigned int i = 0; i < sorted_room_coords.size(); i++)
	{
		int x = sorted_room_coords[i]->coord->get_x();
		int y = sorted_room_coords[i]->coord->get_y();
		x -= minX;
		y -= maxY;
		y *= -1; //reverse the y-coord axis
		sorted_room_coords[i]->coord->set_coord(x, y);
	}
}
void dungeon::create_sorted_room_coords(bool& print_all_map)
{
	//create the vector
	sorted_room_coords.clear();//clear it first to reset it

	for (unsigned int i = 0; i < rooms.size(); i++)
	{
		if (!print_all_map) //printing only the part the player has been to
		{
			if (rooms[i]->get_visited_status())//only add rooms that have been visited
			{
				room_coord* cur_coord = new room_coord(*rooms[i]->get_coord());
				int cur_id = rooms[i]->get_id();
				coord_and_id* cur = new coord_and_id(cur_coord, cur_id);
				sorted_room_coords.push_back(cur);
			}
		}
		else
		{
			room_coord* cur_coord = new room_coord(*rooms[i]->get_coord());
			int cur_id = rooms[i]->get_id();
			coord_and_id* cur = new coord_and_id(cur_coord, cur_id);
			sorted_room_coords.push_back(cur);
		}
	}

	//multiply by 2 to add in space for connections to be drawn
	for (unsigned int i = 0; i < sorted_room_coords.size(); i++)
	{
		room_coord* coord = sorted_room_coords[i]->coord;
		int new_x = 2 * coord->get_x();
		int new_y = 2 * coord->get_y();
		coord->set_coord(new_x, new_y);
	}

	//add in connections
	for (unsigned int i = 0; i < sorted_room_coords.size(); i++)
	{
		 int id = sorted_room_coords[i]->id;

		 if (id < 0)
			 continue;

		 int x = sorted_room_coords[i]->coord->get_x();
		 int y = sorted_room_coords[i]->coord->get_y();

		 room* cur_room = nullptr;
		 if (id == -2)
			 cur_room = rooms[0];
		 else
			cur_room = rooms[id];

		 int* cur_room_exits = cur_room->get_exits();

		 if ((cur_room_exits[0] > 0) || (cur_room_exits[0] == -2)) //add north exit
		 {
			 room_coord* new_coord = new room_coord(x, y + 1);
			 coord_and_id* posid = new coord_and_id(new_coord, -3);
			 if(!duplicate_coord_and_id(posid))
				sorted_room_coords.push_back(posid);
		 }
		 if ((cur_room_exits[1] > 0) || (cur_room_exits[1] == -2))//add south exit
		 {
			 room_coord* new_coord = new room_coord(x, y - 1);
			 coord_and_id* posid = new coord_and_id(new_coord, -3);
			 if (!duplicate_coord_and_id(posid))
				 sorted_room_coords.push_back(posid);
		 }
		 if ((cur_room_exits[2] > 0) || (cur_room_exits[2] == -2))//add east exit
		 {
			 room_coord* new_coord = new room_coord(x + 1, y);
			 coord_and_id* posid = new coord_and_id(new_coord, -4);
			 if (!duplicate_coord_and_id(posid))
				 sorted_room_coords.push_back(posid);
		 }
		 if ((cur_room_exits[3] > 0) || (cur_room_exits[3] == -2))//add west exit
		 {
			 room_coord* new_coord = new room_coord(x - 1, y);
			 coord_and_id* posid = new coord_and_id(new_coord, -4);
			 if (!duplicate_coord_and_id(posid))
				 sorted_room_coords.push_back(posid);
		 }
	}

	//ANOTHER TRANSFORM
	int minX = INT_MAX;
	int maxY = INT_MIN;

	//get the minX and maxY
	for (unsigned int i = 0; i < sorted_room_coords.size(); i++)
	{
		int x = sorted_room_coords[i]->coord->get_x();
		int y = sorted_room_coords[i]->coord->get_y();
		if (x < minX)
			minX = x;
		if (y > maxY)
			maxY = y;
	}

	//transform the vector
	for (unsigned int i = 0; i < sorted_room_coords.size(); i++)
	{
		int x = sorted_room_coords[i]->coord->get_x();
		int y = sorted_room_coords[i]->coord->get_y();
		x -= minX;
		y -= maxY;
		y *= -1; //reverse the y-coord axis
		sorted_room_coords[i]->coord->set_coord(x, y);
	}

	//add a border to the left of the map
	for (unsigned int i = 0; i < sorted_room_coords.size(); i++)
	{
		int x = sorted_room_coords[i]->coord->get_x() + BORDER_MAP_SIZE;
		int y = sorted_room_coords[i]->coord->get_y();
		sorted_room_coords[i]->coord->set_coord(x, y);
	}

	//sort the vector
	std::sort(sorted_room_coords.begin(), sorted_room_coords.end(), [](coord_and_id* a, coord_and_id* b) {
		// sort by x
		//return a->get_x() > b->get_x();
		// sort by y
		//return a->get_y() > b->get_y();

		//we want to return true if a > b
		int a_x = a->coord->get_x();
		int a_y = a->coord->get_y();
		int b_x = b->coord->get_x();
		int b_y = b->coord->get_y();

		//a = (0, -1) smaller
		//b = (0, 2) larger

		//a = (1, -1) larger
		//b = (0, -1) smaller

		if (b_y > a_y)
			return true;
		else if (b_y < a_y)
			return false;
		else //if they are equal
		{
			if (a_x < b_x)
				return true;
		}
		return false;
	});
}
bool dungeon::duplicate_coord_and_id(coord_and_id* posid)
{
	for (unsigned int i = 0; i < sorted_room_coords.size(); i++)
	{
		if (posid->equal(sorted_room_coords[i]))
		{
			return true;
		}
	}
	return false;
}
void dungeon::bfs_set_depth() const
{
	//search the maze using DFS
	std::vector<room*> visited;
	std::queue<room*> Queue;
	room* cur_room = get_start_room();//cur_room is the start room

	//for the first room
	int cur_depth = 0;
	cur_room->set_depth(cur_depth);
	visited.push_back(cur_room);
	int* cur_exits = cur_room->get_exits();
	cur_depth = 1;//dealing with the rooms next to the first room

	for (int i = 0; i < 4; i++)
	{
		int id = cur_exits[i];
		if ((id > 0) && (!in_visited(visited, id))) {
			room* next_room = rooms[id];
			next_room->set_depth(cur_depth);
			Queue.push(next_room);//push the next room with its coord to the stack
		}
		else if (id == -2)//deal with the exit room
		{
			room* exit_room = rooms[0];
			int exit_depth = -1;
			exit_room->set_depth(exit_depth);
			//don't add to the queue cause no exits
		}
	}

	//go through the stack (set up by the first room)
	while (!Queue.empty())
	{
		cur_room = Queue.front();
		cur_depth = cur_room->get_depth();
		int next_depth = cur_depth + 1;
		Queue.pop();
		visited.push_back(cur_room);

		int* cur_exits = cur_room->get_exits();

		for (int i = 0; i < 4; i++)
		{
			int id = cur_exits[i];
			if ((cur_exits[i] != 0) && (!in_visited(visited, id))) //is an exit and not already visited
			{
				room* next_room = rooms[id];
				next_room->set_depth(next_depth);
				Queue.push(next_room);//push the next room with its coord to the stack
			}
		}
	}
}
void dungeon::dfs_set_depth() const
{
	//search the maze using DFS
	std::vector<room*> visited;
	std::stack<room*> Stack;
	room* cur_room = rooms[0];//cur_room is the start room

	//for the first room
	int cur_depth = 0;
	cur_room->set_depth(cur_depth);
	visited.push_back(cur_room);
	int* cur_exits = cur_room->get_exits();
	cur_depth = 1;//dealing with the rooms next to the first room

	for (int i = 0; i < 4; i++)
	{
		int id = cur_exits[i];
		if ((cur_exits[i] > 0) && (!in_visited(visited, id))) {
			room* next_room = rooms[id - 1];
			next_room->set_depth(cur_depth);
			Stack.push(next_room);//push the next room with its coord to the stack
		}
	}

	//go through the stack (set up by the first room)
	while (!Stack.empty())
	{
		cur_room = Stack.top();
		cur_depth = cur_room->get_depth();
		int next_depth = cur_depth + 1;
		Stack.pop();
		visited.push_back(cur_room);

		int* cur_exits = cur_room->get_exits();

		for (int i = 0; i < 4; i++)
		{
			int id = cur_exits[i];
			if ((cur_exits[i] != 0) && (!in_visited(visited, id))) //is an exit and not already visited
			{
				room* next_room = rooms[id - 1];
				next_room->set_depth(next_depth);
				Stack.push(next_room);//push the next room with its coord to the stack
			}
		}
	}
}
room* dungeon::find_adj_room(int& index, room* cur_room) const//finds any adjacent non-connected room
{
	if (DEBUG_MODE)
	{
		std::cout << "\nIn Room " << cur_room->get_id() << " and have index " << index << "\n";
	}
	room_coord delta_coord = get_coord(index);
	room_coord* cur_coord = cur_room->get_coord();
	room_coord desired_coord = delta_coord.add(*cur_coord, delta_coord);

	//assert to make sure that no two rooms generated on top of each other
	if (!no_two_rooms_have_same_coord())
	{
		std::cout << "Two rooms are on top of each other\n";
		exit(1);
	}
	else if (DEBUG_MODE)
	{
		std::cout << "No two rooms are on top of each other!";
	}

	//goes through all the generated rooms and tries to find a room that matches the desired coords
	for (unsigned int i = 0; i < rooms.size(); i++)
	{
		room_coord* new_room_coord = rooms[i]->get_coord();

		if (desired_coord.is_equal(*new_room_coord, desired_coord))
		{
			if (DEBUG_MODE) {
				std::cout << "Want ";
				desired_coord.display();
				std::cout << "have room " << rooms[i]->get_id() << " with coord";
				new_room_coord->display();
				std::cout << "\n";
			}
			return rooms[i];
		}
	}
	return nullptr;
}
void dungeon::place_dragon_key()
{
	//add dragon key to the deepest room of the dungeon and change that room to a dragon lair with a dragon boss
	dragon_key* dragonKey = new dragon_key();

	//find the deepest room of the dungeon
	room* deepest_room = rooms[0];
	for (unsigned int i = 0; i < rooms.size(); i++)
	{
		if (rooms[i]->get_depth() > deepest_room->get_depth())
		{
			deepest_room = rooms[i];
		}
	}
	deepest_room->add_item(dragonKey);
	std::string dragon_lair_description = "You enter a room filled with red sand that slowly goes up to form a hill. Atop the hill sits a sleeping glowing red dragon. Its tail is curled up as it sleeps. You spot a sparkling object beneath its belly. Could it be the key needed to get out of here?";
	deepest_room->set_info("Dragon's Lair", dragon_lair_description);
	deepest_room->clear_chests();//no chests in the boss room
	Enemy* boss = new Dragon(1);
	std::vector<Enemy*> enemies;
	enemies.push_back(boss);
	deepest_room->set_enemies(enemies);
}
void dungeon::place_items()
{
	//place the compass
	compass* Compass = new compass();

	for (unsigned int i = 0; i < rooms.size(); i++)
	{
		if (rooms[i]->get_depth() == COMPASS_SPAWN_DEPTH)
		{
			rooms[i]->add_item(Compass);
			return;
		}
	}
}
void dungeon::place_store()
{
	room* cur_room = nullptr;
	//select a room to be the store
	for (unsigned int i = 0; i < rooms.size(); i++)
	{
		if (rooms[i]->get_depth() == STORE_SPAWN_DEPTH)
		{
			cur_room = rooms[i];
			break;
		}
	}

	if (cur_room == nullptr)
	{
		print("Could not find a room of STORE_SPAWN_DEPTH in place_store() in dungeon.cpp");
		return;
	}
	//convert this room to a store
	std::string store_description = "A small shop that's just set up here. Not sure who buys this stuff, but the business is still here.";
	cur_room->set_info("Store", store_description);
	cur_room->clear_chests();//no chests 
	std::vector<Enemy*> enemies;//no enemies
	cur_room->set_enemies(enemies);

	cur_room->set_store(new store());
	return;
}
int dungeon::get_deepest_depth()
{
	//find the deepest room of the dungeon
	room* deepest_room = rooms[0];
	for (unsigned int i = 0; i < rooms.size(); i++)
	{
		if (rooms[i]->get_depth() > deepest_room->get_depth())
		{
			deepest_room = rooms[i];
		}
	}
	return deepest_room->get_depth();
}
room_coord dungeon::get_coord(int& index) const//uses the room index, to the north or to the south to get a room_coord
{
	switch (index)
	{
	case 0:
		return room_coord(0, 1);
	case 1:
		return room_coord(0, -1);
	case 2:
		return room_coord(1, 0);
	case 3:
		return room_coord(-1, 0);
	default:
		std::cout << "In function dungeon::get_coord(int& index), the index was out of range. Index: " << index;
		exit(1);
	}
}
int dungeon::get_opposite_exit(int exit_num)
{
	switch (exit_num)
	{
	case 0: return 1;
	case 1: return 0;
	case 2: return 3;
	case 3: return 2;
	default:
		std::cout << "exit_num out of range.";
		exit(1);
	}
}
void dungeon::display_debug() {
	for (unsigned int i = 0; i < rooms.size(); i++)
	{
		rooms[i]->display_room_debug();
	}
}
bool dungeon::no_two_rooms_have_same_coord() const //a debugging fxn
{
	for (unsigned int i = 0; i < rooms.size(); i++)
	{
		room_coord* first = rooms[i]->get_coord();
		for (unsigned int j = i + 1; j < rooms.size(); j++)
		{
			room_coord* second = rooms[j]->get_coord();
			int first_id = rooms[i]->get_id();
			int second_id = rooms[j]->get_id();

			if ((first->is_equal(*first, *second)) && (first_id != second_id))
			{
				std::cout << "\n\nFirst Room:\nID: " << first_id;
				std::cout << "\nCoord: ";
				first->display();

				std::cout << "\n\nSecond Room:\nID: " << second_id;
				std::cout << "\nCoord: ";
				second->display();

				return false;
			}
				
		}
	}
	return true;
}