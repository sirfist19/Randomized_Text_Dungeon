#ifndef chest_h
#define chest_h
#include "weapon.h"
#include "potion.h"
#include "helper_fxns.h"
#include "constants.h"
#include <vector>

enum open_method {
	already_open, openable, key
};

class chest : public object //an abc for a general chest
{
protected:
	std::vector<object*> contents;
	open_method locked;
public:
	chest(std::string name, std::string description, open_method locked) : object(name, description), locked(locked)
	{
		
	}
	~chest()
	{
		for (unsigned int i = 0; i < contents.size(); i++)
		{
			delete contents[i];
		}
	}
	virtual object* clone()
	{
		std::cout << "ERROR: Attempting to clone a chest. This is not implemented.";
		return nullptr;
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
	std::string identify()
	{
		return "chest";
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
	bool is_empty() {
		if (contents.size() == 0)
			return true;
		return false;
	}
	open_method get_open_status()
	{
		return locked;
	}
	void clear_chest_contents()
	{
		contents.clear();
	}
	virtual weapon* pick_random_common_weapon()
	{
		weapon_list* list = new weapon_list();
		std::vector<weapon*> common_list = list->get_common_list();

		int rand_num = random(0, common_list.size() - 1);
		weapon* template_weapon = common_list[rand_num];
		weapon* chosen_weapon = template_weapon->clone_weapon();//copies the template weapon
		delete list;
		return chosen_weapon;
	}
	virtual weapon* pick_random_rare_weapon()
	{
		weapon_list* list = new weapon_list();
		std::vector<weapon*> rare_list = list->get_rare_list();

		int rand_num = random(0, rare_list.size() - 1);
		weapon* template_weapon = rare_list[rand_num];
		weapon* chosen_weapon = template_weapon->clone_weapon();//copies the template weapon
		delete list;
		return chosen_weapon;
	}
	virtual weapon* pick_random_legendary_weapon()
	{
		weapon_list* list = new weapon_list();
		std::vector<weapon*> legendary_list = list->get_legendary_list();

		int rand_num = random(0, legendary_list.size() - 1);
		weapon* template_weapon = legendary_list[rand_num];
		weapon* chosen_weapon = template_weapon->clone_weapon();//copies the template weapon
		delete list;
		return chosen_weapon;
	}
	void display_chest_contents()
	{
		print("\nContents:");
		if (contents.size() == 0)
			print("\tEMPTY");
		for (unsigned int i = 0; i < contents.size(); i++)
		{
			std::cout << "\t-";
			if (contents[i]->get_amt() != 1)
				std::cout << contents[i]->get_amt() << " ";
			else if (contents[i]->identify() == "gold")
				std::cout << "1 ";
			contents[i]->display_chest();
		}
	}
	void display_chest()= 0;
	void display() = 0;
};
class wooden_chest : public chest //lowest tier of chest
{
public:
	wooden_chest(open_method locked) : chest("Wooden Chest", "A wooden chest with a steel lining.", locked)
	{
		while (contents.empty())//to ensure no empty chest is generated
		{
			//gold
			int spawn_gold = random(1, 100);
			if (spawn_gold < WOOD_CHEST_SPAWN_GOLD_RATE)
			{
				int amt_of_gold = random(WOOD_CHEST_MIN_GOLD, WOOD_CHEST_MAX_GOLD);
				contents.push_back(new gold(amt_of_gold));
			}

			//healing potions
			int spawn_lesser_healing = random(1, 100);
			if (spawn_lesser_healing < WOOD_CHEST_LESSER_HEALING_SPAWN_RATE)
			{
				contents.push_back(new lesser_healing_potion());
			}

			//weapons
			int spawn_weapon = random(1, 100);
			if (spawn_weapon < CHEST_WEAPON_SPAWN_RATE)
			{
				contents.push_back(pick_random_common_weapon());
			}
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

class gold_chest : public chest //mid tier of chest
{
public:
	gold_chest(open_method locked) : chest("Gold Chest", "A golden chest that sparkles with light. It has a silver lining.", locked)
	{
		while (contents.empty())//to ensure no empty chest is generated
		{
			//gold
			int spawn_gold = random(1, 100);
			if (spawn_gold < GOLD_CHEST_SPAWN_GOLD_RATE)
			{
				int amt_of_gold = random(GOLD_CHEST_MIN_GOLD, GOLD_CHEST_MAX_GOLD);
				contents.push_back(new gold(amt_of_gold));
			}

			//healing potions
			int spawn_healing = random(1, 100);

			int cur = GOLD_CHEST_LESSER_HEALING_SPAWN_RATE;
			if (spawn_healing < cur)
			{
				contents.push_back(new lesser_healing_potion());
			}
			cur += GOLD_CHEST_COMMON_HEALING_SPAWN_RATE;
			if (spawn_healing < cur)
			{
				contents.push_back(new common_healing_potion());
			}

			//weapons
			int spawn_weapon = random(1, 100);
			if (spawn_weapon < CHEST_WEAPON_SPAWN_RATE)
			{
				contents.push_back(pick_random_rare_weapon());
			}
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

class dragon_chest : public chest //highest tier of chest
{
public:
	dragon_chest(open_method locked) : chest("Dragon Chest", "A dark red chest with a black lining made out of dragon bone, blood, and fire.", locked)
	{
		while (contents.empty())//to ensure no empty chest is generated
		{
			//gold
			int spawn_gold = random(1, 100);
			if (spawn_gold < DRAGON_CHEST_SPAWN_GOLD_RATE)
			{
				int amt_of_gold = random(DRAGON_CHEST_MIN_GOLD, DRAGON_CHEST_MAX_GOLD);
				contents.push_back(new gold(amt_of_gold));
			}

			//healing potions
			int spawn_healing = random(1, 100);

			int cur = DRAGON_CHEST_LESSER_HEALING_SPAWN_RATE;
			if (spawn_healing < cur)
			{
				contents.push_back(new lesser_healing_potion());
			}
			cur += DRAGON_CHEST_COMMON_HEALING_SPAWN_RATE;
			if (spawn_healing < cur)
			{
				contents.push_back(new common_healing_potion());
			}
			cur += DRAGON_CHEST_GREATER_HEALING_SPAWN_RATE;
			if (spawn_healing < cur)
			{
				contents.push_back(new greater_healing_potion());
			}

			//weapons
			int spawn_weapon = random(1, 100);
			if (spawn_weapon < CHEST_WEAPON_SPAWN_RATE)
			{
				contents.push_back(pick_random_legendary_weapon());
			}
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
