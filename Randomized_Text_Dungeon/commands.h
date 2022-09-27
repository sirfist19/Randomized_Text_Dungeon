#ifndef commands_h
#define commands_h
#include <unordered_map>
#include <vector>
#include "string"
#include "helper_fxns.h"
#include "room.h"
#include "dungeon.h"
#include "player.h"
#include "part_of_speech.h"



class commands //a very high up class
				//basically a game manager
{
private:
	Player* player;
	dungeon* Dungeon;

	//singleton Noun, Preposition, and Verb classes
	Noun* noun_class;
	Preposition* preposition_class;
	Verb* verb_class;

	//all possible object names 
	std::vector<std::string> all_object_names;

	//input stuff
	std::vector<std::string> cur_player_input;
	verb_type cur_verb;
	preposition_type cur_preposition;
	noun_type cur_noun;
	object* cur_obj;//the object the player wants to interact with
	obj_location cur_obj_orig_location;

	
public:
	void game_loop(commands* game, bool& game_over, bool& quit_to_title_screen, bool& first_time_enter);
	
	commands(std::string player_name);//default constructor
	~commands();
	bool parseInputVector(bool& game_over, bool& quit_to_title_screen);
	bool pick_command_fxn(bool& game_over, bool& quit_to_title_screen);
	void print_all_commands();
	bool store_input(std::vector<std::string>& cur_player_input, store* cur_store);
	void print_all_verbs();
	void print_all_nouns();
	void input_loop(bool& loop, bool& game_over, bool& quit_to_title_screen);
	void display_cur_room_with_top_bar(room* cur_room);
	void display_cur_room_without_top_bar(room* cur_room);
	void fighting(room* cur_room, bool& game_over, Player* player);
	void fighting_input_loop(bool& game_over, Player* player, Enemy* enemy, bool& ran_away);
	void attack(bool& game_over, Player* player, Enemy* enemy, bool& only_enemy_attacks, bool& is_player_heavy_attack);
	void hello();
	void jump();
	void help();
	void open();
	void take();
	void examine();
	bool use();
	void equip();
	void map(bool& print_all_map);
	void basic_map();
	void equip_weapon(object* obj_to_equip);
	void equip_armor(armor* obj_to_equip);
	void drink();
	void drink_individual_potion(object* obj_to_drink);
	void drop();
	std::string get_player_input_noun() const;
	bool go(int index);
	Player* get_player();
	dungeon* get_dungeon();
	void intro_cut_scene();
};
#endif

