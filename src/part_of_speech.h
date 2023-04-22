#ifndef part_of_speech_h
#define part_of_speech_h
#include "player.h"

enum obj_location
{
	loc_room_floor, loc_inventory, loc_inside_terrain, loc_is_terrain, loc_is_chest, loc_in_chest, loc_none
};
enum verb_type {
	verb_none, go, examine, use, take, drop, drink, quit, clear, look,
	open, jump, hello, inventory, help, list, equip, map
};
enum preposition_type {
	preposition_none, in, through, on, off, up, down, above, below, under, over
};
enum noun_type {
	noun_none, north, south, east, west, all, full, depth, full_depth
};


class Verb 
{
private:
	std::unordered_map<std::string, verb_type> verb_type_chart;
public:
	Verb()
	{
		//verbs
		verb_type_chart["go"] = verb_type::go;
		verb_type_chart["n"] = verb_type::go;
		verb_type_chart["north"] = verb_type::go;
		verb_type_chart["s"] = verb_type::go;
		verb_type_chart["south"] = verb_type::go;
		verb_type_chart["e"] = verb_type::go;
		verb_type_chart["east"] = verb_type::go;
		verb_type_chart["w"] = verb_type::go;
		verb_type_chart["west"] = verb_type::go;

		verb_type_chart["examine"] = verb_type::examine;
		verb_type_chart["look at"] = verb_type::examine;
		verb_type_chart["use"] = verb_type::use;
		verb_type_chart["take"] = verb_type::take;
		verb_type_chart["pickup"] = verb_type::take;
		verb_type_chart["pick up"] = verb_type::take;
		verb_type_chart["equip"] = verb_type::equip;
		verb_type_chart["quit"] = verb_type::quit;
		verb_type_chart["q"] = verb_type::quit;
		verb_type_chart["clear"] = verb_type::clear;
		verb_type_chart["open"] = verb_type::open;
		verb_type_chart["jump"] = verb_type::jump;
		verb_type_chart["inventory"] = verb_type::inventory;
		verb_type_chart["i"] = verb_type::inventory;

		verb_type_chart["hello"] = verb_type::hello;
		verb_type_chart["help"] = verb_type::help;
		verb_type_chart["list"] = verb_type::list;
		verb_type_chart["drink"] = verb_type::drink;
		verb_type_chart["drop"] = verb_type::drop;
		verb_type_chart["map"] = verb_type::map;
		verb_type_chart["look"] = verb_type::look;
	}
	verb_type get_verb_type(std::vector<std::string> words)
	{
		//return the verb type found for the entire passed in string
		std::string total_string = place_parsed_back_together(words);
		
		std::unordered_map<std::string, verb_type>::iterator gotVerb = 
			verb_type_chart.find(total_string);

		if (gotVerb == verb_type_chart.end())
		{
			//std::cout << "Command not recognized! Please try again.\n";
			//return true;
			return verb_type::verb_none;
		}
		return gotVerb->second;
	}
};
class Preposition
{
private:
	std::unordered_map<std::string, preposition_type> preposition_type_chart;
public:
	Preposition()
	{
		//prepositions
		preposition_type_chart["into"] = preposition_type::in;
		preposition_type_chart["in"] = preposition_type::in;
		preposition_type_chart["through"] = preposition_type::through;
		preposition_type_chart["on"] = preposition_type::on;
		preposition_type_chart["off"] = preposition_type::off;
		preposition_type_chart["up"] = preposition_type::up;
		preposition_type_chart["down"] = preposition_type::down;
		preposition_type_chart["above"] = preposition_type::above;
		preposition_type_chart["below"] = preposition_type::below;
		preposition_type_chart["under"] = preposition_type::under;
		preposition_type_chart["over"] = preposition_type::over;
	}
	preposition_type get_preposition_type(std::vector<std::string> words)
	{
		//return the preposition type found for the entire passed in string
		std::string total_string = place_parsed_back_together(words);

		std::unordered_map<std::string, preposition_type>::iterator got_preposition =
			preposition_type_chart.find(total_string);

		if (got_preposition == preposition_type_chart.end())
		{
			//std::cout << "Command not recognized! Please try again.\n";
			//return true;
			return preposition_type::preposition_none;
		}
		return got_preposition->second;
	}
};


class Noun
{
private:
	std::unordered_map<std::string, noun_type> noun_type_chart;
public:
	Noun()
	{
		//nouns
		noun_type_chart["north"] = noun_type::north;
		noun_type_chart["n"] = noun_type::north;
		noun_type_chart["south"] = noun_type::south;
		noun_type_chart["s"] = noun_type::south;
		noun_type_chart["east"] = noun_type::east;
		noun_type_chart["e"] = noun_type::east;
		noun_type_chart["west"] = noun_type::west;
		noun_type_chart["w"] = noun_type::west;
		noun_type_chart["all"] = noun_type::all;
		noun_type_chart["full"] = noun_type::full;
		noun_type_chart["depth"] = noun_type::depth;
		noun_type_chart["full_depth"] = noun_type::full_depth;
		noun_type_chart["depth_full"] = noun_type::full_depth;
	}
	noun_type get_noun_type(std::vector<std::string> words)
	{
		//return the noun type found for the entire passed in string
		std::string total_string = place_parsed_back_together(words);

		std::unordered_map<std::string, noun_type>::iterator got_noun =
			noun_type_chart.find(total_string);

		if (got_noun == noun_type_chart.end())
		{
			//std::cout << "Command not recognized! Please try again.\n";
			//return true;
			return noun_type::noun_none;
		}
		return got_noun->second;
	}
	object* get_matching_object(std::vector<std::string> words, Player* player, obj_location& obj_orig_location)
	{
		std::string total_string = place_parsed_back_together(words);
		//find the matching object in general
		//the object could be in the player's inventory, in the room, in a chest
		room* cur_room = player->get_cur_room();
		chest* cur_chest = cur_room->get_chest();
		std::string chest_name = "";
		if (cur_chest != nullptr)
			chest_name = cur_chest->get_name();
		turn_to_lower_case(chest_name);
		object* match = nullptr; //the final match

		object* inventory_match = player->get_matching_object(total_string);//from the player inventory (not equipped items yet)
		object* room_match = cur_room->get_matching_object(total_string);

		std::vector<static_object*> cur_room_terrain = cur_room->get_terrain_objects();

		//loop through all the terrain and see if you can find a match for an object inside the terrain
		object* terrain_item_match = nullptr;
		for (unsigned int i = 0; i < cur_room_terrain.size(); i++)
		{
			static_object* cur_terrain = cur_room_terrain[i];
			object* temp_match = cur_terrain->get_matching_revealed_item(total_string);

			if (temp_match != nullptr)
			{
				terrain_item_match = temp_match;
				break;
			}
		}
		
		object* room_terrain_match = cur_room->get_matching_terrain_object(total_string);
		object* chest_match = nullptr;
		if (cur_chest != nullptr)
		{
			chest_match = cur_chest->get_matching_object(total_string);
		}
		if ((cur_chest != nullptr) && 
			((total_string == "chest") || (total_string == chest_name)))
		{
			match = cur_chest;
			obj_orig_location = obj_location::loc_is_chest;
		}

		if (room_match != nullptr)
		{
			match = room_match;
			obj_orig_location = obj_location::loc_room_floor;
		}
		else if (room_terrain_match != nullptr)
		{
			match = room_terrain_match;
			obj_orig_location = obj_location::loc_is_terrain;
		}
		else if (terrain_item_match != nullptr)
		{
			match = terrain_item_match;
			obj_orig_location = obj_location::loc_inside_terrain;
		}
		else if (chest_match != nullptr)
		{
			match = chest_match;
			obj_orig_location = obj_location::loc_in_chest;
		}
		else if (inventory_match != nullptr)
		{
			match = inventory_match;
			obj_orig_location = obj_location::loc_inventory;
		}
	
		return match;
	}
};

#endif
