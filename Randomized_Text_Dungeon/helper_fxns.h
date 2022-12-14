#ifndef helper_fxns_h
#define helper_fxns_h
#include <string>
#include <iostream>
#include <vector>

void print(std::string in);
void print_no_newline(std::string in);
void print();

bool str_input_accepted(std::string& to_check, std::string* inputs, int size, bool& valid_input);
bool str_input_accepted(std::vector<std::string>& to_check, std::string* inputs, int size, bool& valid_input);
bool str_input_accepted(std::string& to_check, std::vector<std::string>& input, bool& valid_input);
std::string input();
void Quit();
void invalid_input();
void nl(int amount);
void wait(long seconds);
void clear_();
void printEquals();
void printUnderscore();
void game_Over(bool& game_over, bool& quit_to_title_screen);
bool is_number_in_range(const std::string& in, const int& start, const int& end);
int random(int start, int end);
std::vector<std::string> slice_vector(const std::vector<std::string>& in, const int& start_index, const int& end_index);
std::vector<std::string> prompt();
void turn_to_lower_case(std::string& user_input);
void turn_to_lower_case(std::vector<std::string>& user_input);
std::vector<std::string> basic_parse(std::string& user_input);
std::string get_player_name();
std::string welcome_screen();
void winning_screen();
std::string place_parsed_back_together(std::vector<std::string> in);

#endif
