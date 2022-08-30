#ifndef helper_fxns_h
#define helper_fxns_h
#include <string>
#include <iostream>
#include <vector>

void print(std::string in);
void print();

bool str_input_accepted(std::string& to_check, std::string* inputs, int size, bool& valid_input);
std::string input();
void Quit();
void invalid_input();
void nl(int amount);
void wait(long seconds);
void clear_();
void printEquals();
void printUnderscore();
void game_Over();
int random(int start, int end);
std::vector<std::string> prompt();
std::vector<std::string> basic_parse(std::string& user_input);
std::string get_player_name();
std::string welcome_screen();

#endif
