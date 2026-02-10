#include "helper_fxns.h"
#include "constants.h"
#include <vector>
#include <map>

void print(std::string in) 
{
	print_no_newline(in);
	std::cout << std::endl;
}
void print_no_newline(std::string in) {
	//std::cout << in << std::endl;

	std::vector<std::string> text = basic_parse(in);
	int cur = 0;

	for (unsigned int i = 0; i < text.size(); i++)
	{
		if (text[i] == "\n")
		{
			std::cout << "\n";
			cur = 0;
			
			continue;
		}
		int temp_total = text[i].size() + cur;
		if (temp_total <= MAX_CHAR_PER_LINE)
		{
			//print the text and increment cur
			std::cout << text[i];
			if (temp_total == MAX_CHAR_PER_LINE)
			{
				std::cout << std::endl;
				temp_total = 0;
			}
			else if (i != text.size() - 1)
			{
				std::cout << " ";
				temp_total++;
			}
			cur = temp_total;
		}
		else//if it goes over
		{
			//start a new line, reset cur, and print
			std::cout << "\n" << text[i] << " ";
			cur = text[i].size() + 1;
		}
	}
}

int random(int start, int end)//picks a random number from the start to the end number
								//inclusive on both start and end
								//ex: random(0, 5) picks a random number from 0 to 5
{
	int range = (end - start) + 1;
	int a = -1;
	a = rand() % range + start;
	return a;
}
void game_Over(bool& game_over, bool& quit_to_title_screen)
{
	printEquals();
	std::cout << "\nGAME OVER!\n";
	std::cout << "You died. Better luck next time.\n";
	printEquals();

	print("\nDo you want to restart or quit the game?");
	print("1. Restart");
	print("2. Quit");

	std::string in = "";
	while (in == "")
	{
		in = input();
		if ((in == "1") || (in == "restart"))
		{
			game_over = false;
			quit_to_title_screen = true;
			printUnderscore();
			print("Restarting the game...");
			wait(5);
			clear_();
		}
		else if ((in == "2") || (in == "quit"))
		{
			game_over = true;
			quit_to_title_screen = true;
			printUnderscore();
			print("Bye bye! Thanks for playing!");
			printUnderscore();
		}
		else
		{
			printUnderscore();
			print("Invalid input. Please try again.");
			printUnderscore();
			in = "";
		}
	}
	
	
	//exit(0);
}
bool is_number_in_range(const std::string& in, const int& start, const int& end)//inclusive of both start and end
{
	for (unsigned int i = 0; i < in.size(); i++)
	{
		if (!isdigit(in[i])) {
			print("That's not a number.");
			return false;//if not a number don't check it
		}
	}
	int num = std::stoi(in);
	for (int i = start; i < end + 1; i++)
	{
		if (num == i)
			return true;
	}
	return false;
}
void print() {
	std::cout << std::endl;
}

bool str_input_accepted(std::string& to_check, std::string* inputs, int size, bool& valid_input) {
	for (int i = 0; i < size; i++) {
		if (to_check == inputs[i]) {
			valid_input = true;
			return true;
		}
	}
	return false;
}
bool str_input_accepted(std::string& to_check, std::vector<std::string>& input, bool& valid_input) {
	for (unsigned int i = 0; i < input.size(); i++) {
		if (to_check == input[i]) {
			valid_input = true;
			return true;
		}
	}
	return false;
}
bool str_input_accepted(std::vector<std::string>& to_check, std::string* inputs, int size, bool& valid_input) {
	
	std::string res = "";
	for (unsigned int i = 0; i < to_check.size(); i++)
	{
		res += to_check[i];
		if (i != to_check.size() - 1)
			res += " ";
	}
	for (int i = 0; i < size; i++) {
		if (res == inputs[i]) {
			valid_input = true;
			return true;
		}
	}
	return false;
}
std::string input()
{
	print();
	std::cout << ">";
	std::string in;
	std::getline(std::cin, in);
	turn_to_lower_case(in);
	return in;
}
void Quit()
{
	print("You quit the game. Bye bye for now.");
	exit(0);
}
void invalid_input() {
	print("Invalid input");
}
void nl(int amount)//newline fxn
{
	for (int i = 0; i < amount; i++)
	{
		std::cout << std::endl;
	}
}
void wait(long seconds)
{
	//Sleep(seconds );
	long topBounds = 75000000;
	int b = 1;
	for (long y = 0; y < (seconds * topBounds); y++)//250 million times of repeating nothing
	{
		b++;
		b--;
		//the value of repeating the loop 225000000 times is about 1 second
	}
}
void clear_()
{
	if(!DISABLE_CLEARING_SCREEN) {
		#ifdef _WIN32
    		// Clear screen for Windows
    		system("cls");
		#elif defined(__APPLE__) || defined(__linux__)
    		// Clear screen for Mac and Linux
    		system("clear");
		#else
    	// Unsupported operating system
    	std::cout << "Clearing screen not supported on this operating system." << std::endl;
		#endif
	}
}
void printEquals()
{
	for (int i = 0; i < MAX_CHAR_PER_LINE; i++)
	{
		std::cout << "=";
	}
	//std::cout << "\n";
}
void printUnderscore()
{
	for (int i = 0; i < MAX_CHAR_PER_LINE; i++)
	{
		std::cout << "_";
	}
	std::cout << "\n";
}
std::vector<std::string> prompt()
{
	std::cout << ">";
	std::string temp = "";
	std::getline(std::cin, temp);
	std::vector<std::string> res;
	if (temp != "") {
		res = basic_parse(temp);
		turn_to_lower_case(res);
	}
		
	return res;
}
std::vector<std::string> slice_vector(const std::vector<std::string>& in, const int& start_index, const int& end_index) 
{
	std::vector<std::string>::const_iterator begin =
		in.begin() + start_index;
	std::vector<std::string>::const_iterator last =
		in.begin() + end_index + 1;
	std::vector<std::string> verb_vector_to_find(begin, last);
	return verb_vector_to_find;
}
std::string place_parsed_back_together(std::vector<std::string> in)
{
	std::string res = "";
	for (unsigned int i = 0; i < in.size(); i++)
	{
		res += in[i];
		if (i != in.size() - 1)
			res += " ";
	}
	return res;
}
std::vector<std::string> basic_parse(std::string& user_input)
{
	std::string cur_word = "";
	std::vector<std::string> res;
	for (unsigned int i = 0; i < user_input.size(); i++)
	{
		if (user_input[i] == ' ')
		{
			if (cur_word != "")
			{
				res.push_back(cur_word);
				cur_word = "";
			}
		}
		else if (i == user_input.size() - 1)
		{
			cur_word += user_input[i];
			res.push_back(cur_word);
		}
		else
		{
			cur_word += user_input[i];
		}
	}
	return res;
}
void turn_to_lower_case(std::string& user_input)
{
	for (unsigned int i = 0; i < user_input.size(); i++)
	{
		user_input[i] = tolower(user_input[i]);
	}
}
void turn_to_lower_case(std::vector<std::string>& user_input)
{
	for (unsigned int i = 0; i < user_input.size(); i++)
	{
		for(unsigned int j = 0; j < user_input[i].size(); j++)
			user_input[i][j] = tolower(user_input[i][j]);
	}
}
std::string get_player_name()
{
	std::cout << "\n";
	std::cout << ">";
	std::string name;
	std::getline(std::cin, name);
	std::cout << "\nAre you sure that " << name << " is correct?\n";
	print("1. Yes");
	print("2. No");
	return name;
}
std::string encrypt_message(std::string message)
{
	std::map<char, char> key;
	
	key['a'] = 'j';
	key['b'] = 'e';
	key['c'] = 's';
	key['d'] = 't';
	key['e'] = 'l';
	key['f'] = 'p';
	key['g'] = 'o';
	key['h'] = 'u';
	key['i'] = 'z';
	key['j'] = 'y';
	key['k'] = 'x';
	key['l'] = 'a';
	key['m'] = 'c';
	key['n'] = 'b';
	key['o'] = 'i';
	key['p'] = 'r';
	key['q'] = 'w';
	key['r'] = 'v';
	key['s'] = 'q';
	key['t'] = 'd';
	key['u'] = 'f';
	key['v'] = 'h';
	key['w'] = 'k';
	key['x'] = 'm';
	key['y'] = 'n';
	key['z'] = 'q';
	
	

	return "";
}

std::string welcome_screen()
{
	printEquals();
	print();
	print("Welcome to Randomized Text Dungeon!");
	print("\t- An adventure game by Aidan Cini");
	print();
	print("This is a randomized text dungeon. The rooms, enemies, and items are randomized so every time you play, the dungeon will be different. Playing in fullscreen mode is recommended. It's more immersive and looks nicer.\n");
	print("You are an explorer that has just found the dungeon that you have been searching for for 14 years. A massive entrance lined with columns stands leads into the depths below. Enter if you dare.");
	printEquals();
	print();
	print("\n1. Enter");
	print("2. Leave");
	std::string yes_inputs[] = { "1", "start", "enter" };
	std::string no_inputs[] = { "2", "quit", "leave", "q" };

	bool valid_input = false;
	while (valid_input == false) {
		std::string welcome_input = input();

		if (str_input_accepted(welcome_input, no_inputs, 4, valid_input)) {
			Quit();
		}
		else if (str_input_accepted(welcome_input, yes_inputs, 3, valid_input)) {
			//returns to main to generate the dungeon
		}
		else {
			invalid_input();
		}
	}

	//getting player name section
	std::string name = "";
	std::string yes_input[] = { "1", "yes" };
	std::string no_input[] = { "2", "no" };

	valid_input = false;
	print("What is your name? I don't seem to remember...");
	while (valid_input == false) {
		name = get_player_name();
		std::string answer = input();

		if (str_input_accepted(answer, no_input, 2, valid_input)) {
			print("Then type your name again. Not sure how you couldn't remember your own name though.");
			valid_input = false;
		}
		else if ((str_input_accepted(answer, yes_input, 2, valid_input)) && (name.size() > 15))
		{
			print("That name is too long. There is a maximum name length of 15 characters. Please enter a different name.");
			valid_input = false;
		}
		else if (str_input_accepted(answer, yes_input, 2, valid_input)) {
			std::cout << "Well then " << name << " welcome to the dungeon! And best of luck.\n\n";
		}
		else {
			invalid_input();
		}
	}

	wait(4);
	clear_();
	print("Generating the dungeon. Please wait.");
	return name;
}
void winning_screen()
{
	clear_();
	printEquals();
	print("You insert the Dragon Key into the keyhole of the golden doors. As soon as you insert the key a wind rises up around you. You slowly turn the key to the left and hear the sound of the mechanism opening. The carving of the dragon on the door begins to glow and with a click the doors begin to open.");
	print("\nYou up the steps that lead down to the dungeon and enter the forest you came to right before entering. You look up at the blue sky.");
	print("\nYou escaped the dungeon! YOU WIN! CONGRATULATIONS!");
	printEquals();
	exit(0);
}

