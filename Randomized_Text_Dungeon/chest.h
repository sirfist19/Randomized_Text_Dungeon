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
	void display_chest_contents()
	{
		for (unsigned int i = 0; i < contents.size(); i++)
		{
			contents[i]->display();
		}
	}
	void display()
	{
		std::cout << "Chest\n";
		std::cout << "\tDescription: ";
		print(description);
	}
};

#endif
