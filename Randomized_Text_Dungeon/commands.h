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
	go, examine, look, take, drop, quit, clear,
	open, jump, info, hello, inventory, help, list, error
};
enum noun {
	north, south, east, west, pit, _chest, error_, un_assigned
};

class commands //a very high up class
				//basically a game manager
{
private:
	Player* player;
	dungeon* Dungeon;
	noun cur_noun;
	verb cur_verb;
	std::unordered_map<std::string, verb> verb_chart;
	std::unordered_map<std::string, noun> noun_chart;
public:
	void game_loop(commands* game, bool& game_over);
	
	commands(std::string player_name);//default constructor
	~commands();
	bool parseInputVector(std::vector<std::string> in, bool& game_over);
	void print_all_commands();
	void print_all_verbs();
	void print_all_nouns();
	void input_loop(bool& loop, bool& game_over);

	void fighting(room* cur_room, bool& game_over, Player* player);
	void fighting_input_loop(bool& game_over, Player* player, Enemy* enemy);
	void attack(bool& game_over, Player* player, Enemy* enemy);
	//void look(room* cur);
	void hello();
	void jump();
	void help();
	void open();
	void clear_command();
	//void open(object* cur);
	void go(int index);
	Player* get_player();
	dungeon* get_dungeon();
	//void take(room* cur);
};
#endif

