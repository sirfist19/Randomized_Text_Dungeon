#ifndef commands_h
#define commands_h
#include <unordered_map>
#include <vector>
#include "string"
#include "helper_fxns.h"
#include "room.h"
#include "dungeon.h"
#include "player.h"

enum verb {
	go, examine, use, take, drop, drink, quit, clear,
	open, jump, info, hello, inventory, help, list, equip, error
};
enum noun {
	north, south, east, west, pit, _chest, all, error_, un_assigned, _none
};

class commands //a very high up class
				//basically a game manager
{
private:
	Player* player;
	dungeon* Dungeon;
	std::vector<std::string> cur_player_input;
	noun cur_noun;
	verb cur_verb;
	std::string player_input_noun;
	std::unordered_map<std::string, verb> verb_chart;
	std::unordered_map<std::string, noun> noun_chart;
public:
	void game_loop(commands* game, bool& game_over, bool& first_time_enter);
	
	commands(std::string player_name);//default constructor
	~commands();
	bool parseInputVector(bool& game_over);
	void print_all_commands();
	void print_all_verbs();
	void print_all_nouns();
	void input_loop(bool& loop, bool& game_over);

	void fighting(room* cur_room, bool& game_over, Player* player);
	void fighting_input_loop(bool& game_over, Player* player, Enemy* enemy);
	void attack(bool& game_over, Player* player, Enemy* enemy);
	void hello();
	void stack_objects(std::vector<object*>& in);
	void jump();
	void help();
	void open();
	void take();
	void use();
	void equip();
	void equip_weapon(object* obj_to_equip);
	void drink();
	void drop();
	void clear_command();
	std::string get_player_input_noun() const;
	void go(int index);
	Player* get_player();
	dungeon* get_dungeon();
	void intro_cut_scene();
};
#endif

