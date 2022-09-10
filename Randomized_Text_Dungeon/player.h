#ifndef player_h
#define player_h
#include "alive_entity.h"
#include "room.h"

struct level_info
{
	int level;
	int hp;
	int xp_needed;
	level_info(int in_level, int in_hp, int in_xp_needed) : hp(in_hp), level(in_level), xp_needed(in_xp_needed)
	{

	}
};
class Player : public Alive_Entity
{
private:
	room* location;
	std::vector<object*> inventory;
	std::vector<level_info*> leveling_info;
	int cur_exp;
	int exp_to_next_level;

public:
	Player(std::string name, room* location) : Alive_Entity(name, new fists(), 5, 1), location(location), cur_exp(0), exp_to_next_level(20)
	{
		leveling_info.push_back(new level_info(1, 5, 0));//the level, then player's health at that level, then needed xp
		leveling_info.push_back(new level_info(2, 10, 20));
		leveling_info.push_back(new level_info(3, 15, 60));
		leveling_info.push_back(new level_info(4, 22, 130));
		leveling_info.push_back(new level_info(5, 30, 250));
		leveling_info.push_back(new level_info(6, 40, 450));
		leveling_info.push_back(new level_info(7, 50, 700));
		leveling_info.push_back(new level_info(8, 65, 1000));
		leveling_info.push_back(new level_info(9, 80, 1400));
		leveling_info.push_back(new level_info(10, 100, 2000));
	}
	virtual ~Player()
	{
		for (unsigned int i = 0; i < inventory.size(); i++)
		{
			delete inventory[i];
		}
		for (unsigned int i = 0; i < leveling_info.size(); i++)
		{
			delete leveling_info[i];
		}
	}
	virtual void level_up(const int& desired_level)
	{
		level_info* desired_level_info = leveling_info[desired_level - 1];
		level = desired_level;
		int cur_max_health = health->get_max_health();
		int desired_max_health = desired_level_info->hp;
		int change_in_health = desired_max_health - cur_max_health;
		int old_health = health->get_health();
		health->increase_max_health(change_in_health);
		health->heal_to_full();
		int new_health = health->get_health();
		std::cout << "You leveled up to level " << desired_level << " and gained " << change_in_health << " max health points.\n";
		std::cout << "You were healed by "<<new_health - old_health<<" health points to full health.\n";
	}
	int get_exp() const
	{
		return cur_exp;
	}
	void increase_exp(const int& amt)
	{
		if (amt <= 0)
			return;
		cur_exp += amt;

		int desired_level = 0;
		//find the correct player level for the new amount of exp
		for (unsigned int i = 0; i < leveling_info.size(); i++)
		{
			if (cur_exp >= leveling_info[i]->xp_needed)
				desired_level++;
		}
		if (level < desired_level)
		{
			level_up(desired_level);
		}

		//update exp to next level's val
		int next_level = level + 1;
		if (next_level < 10)
			exp_to_next_level = leveling_info[next_level - 1]->xp_needed - cur_exp;//minus one due to the indexing of leveling_info
		else
			exp_to_next_level = 0;
	}
	virtual void f() {}
	void display_inventory()
	{
		//printEquals();
		std::cout << name << "'s Inventory\n";
		std::cout << "Level: " << level <<"\n";
		std::cout << "\tExp till next level: " << exp_to_next_level << "\n";
		std::cout << "\tCurrent Exp: " << cur_exp << "\n\n";

		health->display_health_bar();
		std::cout << "Defense: " << defense << std::endl;
		display_armor();
		std::cout << "Weapon: ";
		main_weapon->display_chest();
		
		std::cout << "\nGeneral Items: ";

		if (inventory.size() == 0)
			std::cout << "\n NO ITEMS IN INVENTORY";
		std::cout << "\n";
		for (unsigned int i = 0; i < inventory.size(); i++)
		{
			std::cout << "\tx";
			//if(inventory[i]->get_amt() != 1)
			std::cout << inventory[i]->get_amt() << " - ";
			inventory[i]->display_chest();
		}
	}
	void add_item_to_inventory(object* obj)
	{
		inventory.push_back(obj);
	}
	void set_inventory(std::vector<object*> in)
	{
		inventory = in;
	}
	std::vector<object*> get_inventory() const
	{
		return inventory;
	}
	helmet* get_helmet() const
	{
		return Helmet;
	}
	void set_helmet(helmet* in)
	{
		Helmet = in;
	}
	chestplate* get_chestplate() const
	{
		return Chestplate;
	}
	void set_chestplate(chestplate* in)
	{
		Chestplate = in;
	}
	void set_boots(boots* in)
	{
		Boots = in;
	}
	boots* get_boots() const
	{
		return Boots;
	}
	void delete_item_from_inventory(object* obj, int amt_used) //the obj is the object to be deleted
	{
		//to remove the object from the player's inventory move the last vector item to it's place and 
		//then pop_back()
		for (unsigned int i = 0; i < inventory.size(); i++)
		{
			if (obj == inventory[i])
			{
				if (obj->get_amt() > 1)
				{
					obj->decrease_amt(amt_used);
					if (obj->get_amt() == 0)
					{
						inventory[i] = inventory[inventory.size() - 1];
						inventory.pop_back();
					}
				}
				else //amt is only 1 so delete the item
				{
					inventory[i] = inventory[inventory.size() - 1];
					inventory.pop_back();
				}
				
			}
		}
	}
	void delete_item_from_inventory_all(object* obj) //the obj is the object to be deleted
	{
		//to remove the object from the player's inventory move the last vector item to it's place and 
		//then pop_back()
		for (unsigned int i = 0; i < inventory.size(); i++)
		{
			if (obj == inventory[i])
			{
				inventory[i] = inventory[inventory.size() - 1];
				inventory.pop_back();
			}
		}
	}
	void add_items_to_inventory(std::vector<object*> objs)
	{
		for (unsigned int i = 0; i < objs.size(); i++)
		{
			inventory.push_back(objs[i]);
		}
	}
	object* get_matching_object(std::string player_input_noun) 
	{
		for (unsigned int i = 0; i < inventory.size(); i++)
		{
			std::string cur_name = inventory[i]->get_name();
			turn_to_lower_case(cur_name);

			if (player_input_noun == cur_name)
			{
				object* result = inventory[i];
				return result;
			}
		}
		return nullptr;
	}
	virtual void display_attack_info() 
	{
		std::cout << "Player: " << name << "    Level: "<<level<<"\n";
		health->display_health_bar();
		std::cout << "Defense: " << defense << std::endl;
		std::cout << "Weapon: " << main_weapon->get_name()<<"\n";
	}
	room* get_cur_room() {
		return location;
	}
	virtual void kill()//kills the entity
	{
		health->kill(); 
		//game_Over();
	}
	void set_location(room* new_location)
	{
		location = new_location;
	}
	int get_cur_room_id()
	{
		return location->get_id();
	}
	std::string get_name()
	{
		return name;
	}
	void printTopBar()
	{
		printEquals();
		std::cout << "\nPlayer: " + name + "     " + "Room: " << location->get_name()
			<< "-" << location->get_id();
		std::cout << "     Health: " << health->get_health() << "/" << health->get_max_health();
		std::cout<<"\n";
		printEquals();
		std::cout << "\n";
	}
};
#endif
