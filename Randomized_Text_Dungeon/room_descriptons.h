#ifndef room_descriptions_h
#define room_descriptions_h
#include "chest.h"
#include "static_object.h"

struct room_description {
	std::string name;
	std::string description;
	std::vector<static_object*> terrain;

	room_description(std::string _name, std::string _description, std::vector<static_object*> _terrain)
	{
		name = _name;
		description = _description;
		terrain = _terrain;

		//for (unsigned int i = 0; i < terrain.size(); i++)
		//{
		//	print(terrain[i]->get_name());
		//}
	}
	room_description(std::string _name, std::string _description)//no terrain items
	{
		name = _name;
		description = _description;
	}
	//copy constructor
	room_description(const room_description& in)
	{
		name = in.name;
		description = in.description;
		
		for (unsigned int i = 0; i < in.terrain.size(); i++)
		{
			static_object* cur_terrain = in.terrain[i];
			terrain.push_back(cur_terrain->clone());
		}
	}
	~room_description()
	{
		for (unsigned int i = 0; i < terrain.size(); i++)
		{
			delete terrain[i];
		}
	}
};
class room_descriptions {
private:
	std::vector<room_description*> start_descriptions;
	std::vector<room_description*> near_descriptions;
	std::vector<room_description*> mid_descriptions;
	std::vector<room_description*> far_descriptions;
	std::vector<room_description*> very_far_descriptions;
	std::vector<room_description*> any_descriptions;//descriptions for either near or mid

public:
	std::vector<room_description*> get_start_room_descriptions() {
		return start_descriptions;
	}
	std::vector<room_description*> get_near_room_descriptions() {
		near_descriptions.insert(near_descriptions.begin(), any_descriptions.begin(), any_descriptions.end());
		return near_descriptions;
	}
	std::vector<room_description*> get_mid_room_descriptions() {
		//mid_descriptions.insert(mid_descriptions.begin(), any_descriptions.begin(), any_descriptions.end());
		return mid_descriptions;
	}
	std::vector<room_description*> get_far_room_descriptions() {
		return far_descriptions;
	}
	std::vector<room_description*> get_very_far_room_descriptions() {
		return very_far_descriptions;
	}
	~room_descriptions()
	{
		for (unsigned int i = 0; i < start_descriptions.size(); i++)
		{
			delete start_descriptions[i];
		}
		for (unsigned int i = 0; i < near_descriptions.size(); i++)
		{
			delete near_descriptions[i];
		}
		for (unsigned int i = 0; i < mid_descriptions.size(); i++)
		{
			delete mid_descriptions[i];
		}
		for (unsigned int i = 0; i < far_descriptions.size(); i++)
		{
			delete far_descriptions[i];
		}
		for (unsigned int i = 0; i < very_far_descriptions.size(); i++)
		{
			delete very_far_descriptions[i];
		}
		for (unsigned int i = 0; i < any_descriptions.size(); i++)
		{
			if(any_descriptions[i] != nullptr)
				delete any_descriptions[i];
		}
	}
	room_descriptions()
	{
		//start
		start_descriptions.push_back(
			new room_description(
				"Entrance Room",
				"This room is where you entered the dungeon from and is the only way out. The golden doors with the jade dragon carving remain shut. A strip of light comes through from outside.",
				std::vector<static_object*> {new Dragon_doors()}
			));

		//any (near at the moment)
		any_descriptions.push_back(
			new room_description(
				"Mossy Room",
				"The walls are lined with a thick coat of moss that feels wet to the touch. The air is cold and damp.",
				 std::vector<static_object*> {new Moss()}
			));
		any_descriptions.push_back(
			new room_description(
				"Chamber",
				"You find nothing of note in this room. Stone cobble lines the floor and walls."
			));
		any_descriptions.push_back(
			new room_description(
				"Hallway",
				"A short narrow hallway greets you. The only light filters in from the rooms behind and in front of you."
			));
		any_descriptions.push_back(
			new room_description(
				"Small Room",
				"You enter a tiny, cramped room that isn't even big enough to swing a sword. There's a pedistal in the middle of the room."
				, std::vector<static_object*> {new Pedistal()}
			));

		//near - still close to the entrance, so civilization and not too interesting
		near_descriptions.push_back
		(new room_description(
			"Pit Room", 
			"A giant circular pit lies at the room's center.",
			std::vector<static_object*> {new Pit()}
		));
		near_descriptions.push_back
		(new room_description(
			"Boulder Room",
			"A large spherical boulder lies at the room's center."
			
		));
		near_descriptions.push_back(
			new room_description(
				"Dinning Room",
				"A long wooden dinning table surrounded by wooden chairs is in front of you. The table is set with ceramic plates, knives, forks, spoons, and metal platters as if a large feast was just to start. But it seems no one is around."
			));

		//mid - getting more interesting but still civilization (in the depths of civilization)
		mid_descriptions.push_back
		(new room_description(
			"Library",
			"In front of you lies shelves upon shelves of books going on as far as you can see. The books go so high up that you'd need a tall ladder to reach the top books. Perhaps there is great knowledge to be gained here."

		));
		mid_descriptions.push_back
		(new room_description(
			"Jail",
			"You walk into a room lined with small steel cellrooms. Most of the doors are locked but a few are left wide open. This room feels like people lived many years and died here. A few of the cells have scattered bones all over the floor."
		));
		mid_descriptions.push_back(
			new room_description(
				"Great Hall",
				"You enter a chamber filled with massive columns as far as you can see. The columns are light gray, made out of marble, and are so thick that it makes you wonder what exactly the pillars need to hold up."
			));
		mid_descriptions.push_back(
			new room_description(
				"Statue Hall",
				"A massive circular hall stands before you. Upon a raised platform at the center of the room, lies a statue of a great King sitting in his throne while a Knight, on his knees, pledges fealty to him. The King is illuminated by a bright light coming from the ceiling."
			));
		mid_descriptions.push_back(
			new room_description(
				"Armory",
				"Stands for armor lies all around you."
			));
		mid_descriptions.push_back(
			new room_description(
				"Sparing Room",
				"The room is filled with racks to hold weapons of all types. Swords, maces, training stick, and axes. But there are no weapons here now. Lots of training must have gone on here ages ago."
			));
		mid_descriptions.push_back(
			new room_description(
				"Fountain",
				"A circular fountain lies at the center of the room. Somehow water still is being pumped from the bottom of the fountain to its top. The water gurgles as it trickles down the fountain's four levels."
			));
		//far
		far_descriptions.push_back(
			new room_description(
				"Lake Room",
				"A lake lies ahead of you. The water is completely still, but almost freezing. Swimming won't be an option. Who knows how deep the lake goes..."
			));
		far_descriptions.push_back(
			new room_description(
				"River",
				"A river gushes through at high speed. You will need to step into the river in order to cross."
			));
		far_descriptions.push_back(
			new room_description(
				"Waterfall",
				"Water from a river above falls off a shear cliff tumbling down to the floor. The waterfall splits into three portions. Mist comes off of the waterfall turning into fog."
				, std::vector<static_object*> {new Waterfall()}
			));
		far_descriptions.push_back(
			new room_description(
				"Dragon Skeleton Room",
				"The skeleton of a long dead dragon lies in the center of the room. The bones are black and a hard as steel. The dragon scales have long since been removed to make dragon scale armor."
				
		));
		far_descriptions.push_back(
			new room_description(
				"Overgrown Room",
				"Even walking around is a challenge. At every step you have to cut the undergrowth away. Ferns, lichen, moss have taken this room over. The heat and humidity are almost unbearable."

			));
		//very far - nature 
		very_far_descriptions.push_back(
			new room_description(
				"Lava Room",
				"A small bridge leads over a river of glowing orange steaming magma. You hope the bridge is sturdy because falling would surely result in death."
			));
		very_far_descriptions.push_back(
			new room_description(
				"Cavern",
				"It's pitch black, but you can see that right in front of you is the start of a massive cave. Stalactites and stalagmites line the floor and ceiling."
			));
		very_far_descriptions.push_back(
			new room_description(
				"Magma Bridge",
				"A river of lava flows under a narrow black igneous rock bridge lined with spikes as handrails. You must cross to get to the other side."
			));
		very_far_descriptions.push_back(
			new room_description(
				"Ice Cave",
				"You enter a cave that is made entirely of thick light-blueish ice. You shiver as it's freezing in here."
			));
	}
};

#endif
