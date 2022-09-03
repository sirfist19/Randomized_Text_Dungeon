#ifndef player_h
#define player_h
#include "alive_entity.h"
#include "room.h"

class Player : public Alive_Entity
{
private:
	room* location;
	std::vector<object*> inventory;
public:
	Player(std::string name, room* location) : Alive_Entity(name, new fists(), 10), location(location) //new fists()
	{

	}
	virtual void f() {}
	void display_inventory()
	{
		//printEquals();
		std::cout << name << "'s Inventory\n";
		health->display_health_bar();
		std::cout << "Weapon: ";
		main_weapon->display_chest();

		std::cout << "\nItems: ";

		if (inventory.size() == 0)
			std::cout << "\n NO ITEMS IN INVENTORY";
		std::cout << "\n";
		for (unsigned int i = 0; i < inventory.size(); i++)
		{
			std::cout << "\tx";
			//if(inventory[i]->get_amt() != 1)
			std::cout << inventory[i]->get_amt()<<" - ";
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
		std::cout << "Player: " << name << "\n";
		health->display_health_bar();
		std::cout << "Weapon: " << main_weapon->get_name()<<"\n";
	}
	room* get_cur_room() {
		return location;
	}
	virtual void kill()//kills the entity
	{
		health->kill(); 
		game_Over();
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
