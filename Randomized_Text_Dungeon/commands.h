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
	open, jump, hello, inventory, help, list, equip, map, error
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

	//input stuff
	std::vector<std::string> cur_player_input;
	noun cur_noun;
	verb cur_verb;
	std::string player_input_noun;
	std::unordered_map<std::string, verb> verb_chart;
	std::unordered_map<std::string, noun> noun_chart;
public:
	void game_loop(commands* game, bool& game_over, bool& quit_to_title_screen, bool& first_time_enter);
	
	commands(std::string player_name);//default constructor
	~commands();
	bool parseInputVector(bool& game_over, bool& quit_to_title_screen);
	void print_all_commands();
	void print_all_verbs();
	void print_all_nouns();
	void input_loop(bool& loop, bool& game_over, bool& quit_to_title_screen);

	void fighting(room* cur_room, bool& game_over, Player* player);
	void fighting_input_loop(bool& game_over, Player* player, Enemy* enemy, bool& ran_away);
	void attack(bool& game_over, Player* player, Enemy* enemy, bool& only_enemy_attacks, bool& is_player_heavy_attack);
	void hello();
	void stack_objects(std::vector<object*>& in);
	void jump();
	void help();
	void open();
	void take();
	void use();
	void equip();
	void map(bool& print_all_map);
	void basic_map();
	void equip_weapon(object* obj_to_equip);
	void equip_armor(armor* obj_to_equip);
	void drink();
	void drop();
	void clear_command();
	std::string get_player_input_noun() const;
	bool go(int index);
	Player* get_player();
	dungeon* get_dungeon();
	void intro_cut_scene();
};
#endif

