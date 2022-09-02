#ifndef chest_h
#define chest_h
#include "weapon.h"
#include "helper_fxns.h"
#include <vector>

enum open_method {
	already_open, openable, key
};

class chest : public object
{
protected:
	std::vector<object*> contents;
	open_method locked;
public:
	chest() : object("Chest", "A wooden chest with steel lining."), locked(open_method::openable)
	{
		int amt_of_gold = random(0, 20);
		contents.push_back(new gold(amt_of_gold));
		contents.push_back(new dagger());
	}
	void open()
	{
		switch (locked)
		{
		case open_method::openable:
			print("With a creaking sound, the chest opens.");
			display_chest_contents();
			locked = open_method::already_open;
			break;
		case open_method::already_open:
			print("The chest is already open.");
			display_chest_contents();
			break;
		case open_method::key:
			print("The chest is locked. You'll need a key to open it.");
			break;
		default:
			break;
		}
	}
	std::vector<std::string> get_all_content_names() const 
	{
		std::vector<std::string> names;
		for (unsigned int i = 0; i < contents.size(); i++)
		{
			std::string cur_name = contents[i]->get_name();
			names.push_back(cur_name);
		}
		turn_to_lower_case(names);
		return names;
	}
	object* get_matching_object(std::string player_input_noun)  
	{
		if (locked == open_method::already_open)
		{
			for (unsigned int i = 0; i < contents.size(); i++)
			{
				std::string cur_name = contents[i]->get_name();
				turn_to_lower_case(cur_name);
				//std::cout << "Cur Chest obj name: " << cur_name<<"\n";

				if (player_input_noun == cur_name)
				{
					object* result = contents[i];

					//to remove the object from the chest move the last vector item to it's place and 
					//then pop_back()
					contents[i] = contents[contents.size() - 1];
					contents.pop_back();

					return result;
				}
			}
		}
		return nullptr;
	}
	std::vector<object*> get_all_contents()
	{
		return contents;
	}
	open_method get_open_status()
	{
		return locked;
	}
	void clear_chest_contents()
	{
		contents.clear();
	}
	void display_chest_contents()
	{
		print("\nContents:");
		for (unsigned int i = 0; i < contents.size(); i++)
		{
			std::cout << "\t-";
			contents[i]->display_chest();
		}
	}
	void display_chest()
	{
		print(name);
	}
	void display()
	{
		std::cout << "Chest\n";
		std::cout << "\tDescription: ";
		print(description);
	}
};

#endif
