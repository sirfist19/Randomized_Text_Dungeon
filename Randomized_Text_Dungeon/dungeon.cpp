#include "dungeon.h"
#include "enemy.h"

dungeon::dungeon() 
{
	room_descriptions* stock_room_descriptions = new room_descriptions();
	srand((unsigned int)time(NULL));//sets random numbers

	//for the first room
	room* temp_room = new room(rooms.size() + 1);
	/*temp_room->add_item(new silver_chestplate());
	temp_room->add_item(new silver_boots());
	temp_room->add_item(new bow());
	temp_room->add_item(new dragon_scale_chestplate());
	temp_room->add_item(new dragon_scale_chestplate());
	temp_room->add_item(new bronze_helmet());
	
	temp_room->add_item(new daedric_sword());*/
	//temp_room->add_item(new common_healing_potion());
	//temp_room->add_item(new sword());
	//temp_room->add_item(new bronze_helmet());
	//temp_room->add_item(new dragon_scale_chestplate());
	rooms.push_back(temp_room);
	create_new_exits(temp_room, stock_room_descriptions);

	//std::cout << rooms.size() << max_num_rooms;
	while ((rooms.size() < MAX_DUNGEON_SIZE) && (!rooms_to_give_exits.empty()))
	{
		room* cur_room = rooms_to_give_exits.top();
		rooms_to_give_exits.pop();

		create_new_exits(cur_room, stock_room_descriptions);
	}
	dfs_set_depth();

	for (unsigned int i = 0; i < rooms.size(); i++)
	{
		room* cur_room = rooms[i];
		
		cur_room->set_depth_tier();
		cur_room->spawn_enemies();
		cur_room->assign_room_type(cur_room->get_tier(), stock_room_descriptions);
		cur_room->place_chests();
		/*if (cur_room->get_id() == 2) //testing
		{
			std::vector<Enemy*> enemies;
			enemies.push_back(new Goblin(2));
			cur_room->set_enemies(enemies);
		}*/
	}
	place_dragon_key();
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

		//add dragon key (temporarily)
		//dragon_key* dragonKey = new dragon_key();
		//cur_room->add_item(dragonKey);

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
			while (true)
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
				room* temp_room = new room(rooms.size() + 1);

				//update cur_room's exit
				cur_exits[index] = temp_room->get_id();
				cur_room->set_exits(cur_exits);

				//update new room's exit
				int new_room_exit = get_opposite_exit(index);
				int* temp_exits = temp_room->get_exits();
				temp_exits[new_room_exit] = cur_room->get_id();
				temp_room->set_exits(temp_exits);

				//add it to the rooms vector
				rooms.push_back(temp_room);

				//add it to the room_to_give_exits stack
				rooms_to_give_exits.push(temp_room);
			}
			else if ((cur_exits[index] == 0))//there is no exit, but there is an adjacent room in a given direction
			{
				//a random chance to connect to that room
				int chance = random(0,99); //0 to 9
				if (chance < CONNECT_ROOMS_CHANCE)
				{
					//connect the current room
					cur_exits[index] = adj_room->get_id();

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
void dungeon::dfs(std::vector<room_info>& visited, room* cur_room) const
{
	//search the maze using DFS
	std::stack<room_info> Stack;

	//for the first room
	room_coord cur_coord = room_coord(0, 0);
	room_info cur_room_info = room_info(cur_coord, cur_room);
	visited.push_back(cur_room_info);

	int* cur_exits = cur_room->get_exits();

	for (int i = 0; i < 4; i++)
	{
		int id = cur_exits[i];
		if ((cur_exits[i] > 0) && (!in_visited(visited, id))) {
			room* next_room = rooms[id - 1];
			room_coord delta_coord = get_coord(i);
			//delta_coord.display();
			room_coord next_coord = delta_coord.add(delta_coord, cur_coord);
			room_info next_room_info = room_info(next_coord, next_room);
			Stack.push(next_room_info);//push the next room with its coord to the stack
		}
	}

	//go through the stack (set up by the first room)
	while (!Stack.empty())
	{
		cur_room_info = Stack.top();
		Stack.pop();
		cur_coord = cur_room_info.get_pos();
		visited.push_back(cur_room_info);

		int* cur_exits = cur_room_info.get_room()->get_exits();

		for (int i = 0; i < 4; i++)
		{
			int id = cur_exits[i];
			if ((cur_exits[i] > 0) && (!in_visited(visited, id))) //is an exit and already visited
			{
				room* next_room = rooms[id - 1];
				room_coord delta_coord = get_coord(i);
				room_coord next_coord = delta_coord.add(delta_coord, cur_coord);
				room_info next_room_info = room_info(next_coord, next_room);
				Stack.push(next_room_info);//push the next room with its coord to the stack
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
	room_coord desired_coord = get_coord(index);

	std::vector<room_info> visited;
	dfs(visited, cur_room);//DFS through the whole dungeon, visited is now full of the rooms with their relative coords

	//assert to make sure that no two rooms generated on top of each other
	if (!no_two_rooms_have_same_coord(visited))
	{
		std::cout << "Two rooms are on top of each other\n";
		exit(1);
	}
	else if (DEBUG_MODE)
	{
		std::cout << "No two rooms are on top of each other!";
	}

	//goes through the visited array and tries to find a room that matches the desired coords
	for (unsigned int i = 0; i < visited.size(); i++)
	{
		room_coord cur_coord = visited[i].get_pos();
		if (DEBUG_MODE) {
			std::cout << visited[i].get_room()->get_id() << " with coord ";
			visited[i].get_pos().display();
			std::cout << "\n";
		}

		if (desired_coord.is_equal(cur_coord, desired_coord))
		{
			if (DEBUG_MODE) {
				std::cout << "Want ";
				desired_coord.display();
				std::cout << "have room " << visited[i].get_room()->get_id() << " with coord";
				cur_coord.display();
				std::cout << "\n";
			}
			return visited[i].get_room();
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
bool dungeon::no_two_rooms_have_same_coord(std::vector<room_info> visited) const //a debugging fxn
{
	for (unsigned int i = 0; i < visited.size(); i++)
	{
		room_coord first = visited[i].get_pos();
		for (unsigned int j = i + 1; j < visited.size(); j++)
		{
			room_coord second = visited[j].get_pos();
			int first_id = visited[i].get_room()->get_id();
			int second_id = visited[j].get_room()->get_id();

			if ((first.is_equal(first, second)) && (first_id != second_id))
			{
				std::cout << "\n\nFirst Room:\nID: " << first_id;
				std::cout << "\nCoord: ";
				first.display();

				std::cout << "\n\nSecond Room:\nID: " << second_id;
				std::cout << "\nCoord: ";
				second.display();

				return false;
			}
				
		}
	}
	return true;
}