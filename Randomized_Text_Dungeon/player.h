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
		health->heal(change_in_health);
		int new_health = health->get_health();
		std::cout << "You leveled up to level " << desired_level << " and gained " << change_in_health << " max health points.\n";
		//std::cout << "You were healed by "<<new_health - old_health<<" health points to full health.\n";
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
	std::vector<weapon*> get_inventory_weapons() const
	{
		std::vector<weapon*> weapons;
		for (unsigned int i = 0; i < inventory.size(); i++)
		{
			if (inventory[i]->identify() == "weapon")
			{
				weapons.push_back((weapon*)inventory[i]);
			}
		}
		return weapons;
	}
	std::vector<armor*> get_inventory_armor() const
	{
		std::vector<armor*> inv_armor;
		for (unsigned int i = 0; i < inventory.size(); i++)
		{
			std::string cur_id = inventory[i]->identify();
			if ((cur_id == "armor") || (cur_id == "helmet") || (cur_id == "chestplate") || (cur_id == "boots"))
			{
				inv_armor.push_back((armor*)inventory[i]);
			}
		}
		return inv_armor;
	}
	std::vector<healing_potion*> get_inventory_healing_potions() const
	{
		std::vector<healing_potion*> inv_potions;
		for (unsigned int i = 0; i < inventory.size(); i++)
		{
			std::string cur_id = inventory[i]->identify();
			if (cur_id == "healing potion")
			{
				inv_potions.push_back((healing_potion*)inventory[i]);
			}
		}
		return inv_potions;
	}
	std::vector<object*> get_misc_objs() const
	{
		std::vector<object*> inv_misc;
		for (unsigned int i = 0; i < inventory.size(); i++)
		{
			std::string cur_id = inventory[i]->identify();

			//not equal to anything else
			if ((cur_id != "healing potion") && (cur_id != "armor") 
				&& (cur_id != "helmet") && (cur_id != "chestplate") 
				&& (cur_id != "boots") && (cur_id != "weapon") 
				&& (cur_id != "gold"))
			{
				inv_misc.push_back(inventory[i]);
			}
		}
		return inv_misc;
	}
	healing_potion* get_smallest_healing_potion()
	{
		std::vector<healing_potion*> inv_potions = get_inventory_healing_potions();

		//find the smallest potion
		healing_potion* smallest_potion = (healing_potion*)inv_potions[0];
		for (unsigned int j = 1; j < inv_potions.size(); j++)
		{
			healing_potion* cur = (healing_potion*)inv_potions[j];
			if (cur->get_heal_amt() < smallest_potion->get_heal_amt())
			{
				smallest_potion = cur;
			}
		}
		return smallest_potion;
	}
	int get_amt_gold() const
	{
		int gold_amt = 0;
		for (unsigned int i = 0; i < inventory.size(); i++)
		{
			std::string cur_id = inventory[i]->identify();
			if (cur_id == "gold")
			{
				gold_amt = inventory[i]->get_amt();
			}
		}
		return gold_amt;
	}
	void display_inventory() const
	{
		//sort the inventory items into different categories
		int gold_amt = get_amt_gold();
		std::vector<weapon*> inventory_weapons = get_inventory_weapons();
		std::vector<armor*> inventory_armor = get_inventory_armor();
		std::vector<healing_potion*> inventory_healing_potions = get_inventory_healing_potions();
		std::vector<object*> inventory_misc_objs = get_misc_objs();

		std::cout << name << "'s Inventory\n";
		std::cout << "Level: " << level <<"\n";
		std::cout << "\tExp till next level: " << exp_to_next_level << "\n";
		std::cout << "\tCurrent Exp: " << cur_exp << "\n\n";

		health->display_health_bar();
		std::cout << "Gold: " << gold_amt << "\n";
		std::cout << "Weapon: ";
		main_weapon->display_chest();
		std::cout << "Defense: " << defense << std::endl;
		display_armor();
		
		std::cout << "\nGeneral Items:\n";

		if (inventory.size() == 0)
		{
			std::cout << " NO ITEMS IN INVENTORY\n";
			return;
		}

		//print out all of the inventory unorganized
		//std::cout << "\n";
		//for (unsigned int i = 0; i < inventory.size(); i++)
		//{
		//	std::cout << "\tx";
		//	std::cout << inventory[i]->get_amt() << " - ";
		//	inventory[i]->display_chest();
		//}
		//std::cout << "\n\n\n";
		
		if (!inventory_weapons.empty())
			std::cout << "\tWeapons:\n";
		for (unsigned int i = 0; i < inventory_weapons.size(); i++)
		{
			std::cout << "\t\tx";
			std::cout << inventory_weapons[i]->get_amt() << " - ";
			inventory_weapons[i]->display_chest();
		}
		
		if (!inventory_armor.empty())
			std::cout << "\tArmor:\n";
		for (unsigned int i = 0; i < inventory_armor.size(); i++)
		{
			std::cout << "\t\tx";
			std::cout << inventory_armor[i]->get_amt() << " - ";
			inventory_armor[i]->display_chest();
		}
		
		if (!inventory_healing_potions.empty())
			std::cout << "\tPotions:\n";
		for (unsigned int i = 0; i < inventory_healing_potions.size(); i++)
		{
			std::cout << "\t\tx";
			std::cout << inventory_healing_potions[i]->get_amt() << " - ";
			inventory_healing_potions[i]->display_chest();
		}

		if (!inventory_misc_objs.empty())
			std::cout << "\tMiscellaneous Items:\n";
		for (unsigned int i = 0; i < inventory_misc_objs.size(); i++)
		{
			std::cout << "\t\tx";
			std::cout << inventory_misc_objs[i]->get_amt() << " - ";
			inventory_misc_objs[i]->display_chest();
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

		if ((obj == main_weapon) && (main_weapon->get_name() != "Fists"))
		{
			delete obj;
			main_weapon = new fists();
			return;
		}
		if (obj == Helmet)
		{
			delete Helmet;
			Helmet = nullptr;
			return;
		}
		if (obj == Chestplate)
		{
			delete Chestplate;
			Chestplate = nullptr;
			return;
		}
		if (obj == Boots)
		{
			delete Boots;
			Boots = nullptr;
			return;
		}
		for (unsigned int i = 0; i < inventory.size(); i++)
		{
			if (obj == inventory[i])
			{
				if (obj->get_amt() > 1)
				{
					obj->decrease_amt(amt_used);
					if (obj->get_amt() == 0)
					{
						delete inventory[i];
						inventory[i] = inventory[inventory.size() - 1];
						inventory.pop_back();
					}
				}
				else //amt is only 1 so delete the item
				{
					delete inventory[i];
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
		std::string weapon_name = main_weapon->get_name();
		turn_to_lower_case(weapon_name);
		std::string helmet_name = "";
		std::string chestplate_name = "";
		std::string boots_name = "";

		if (Helmet != nullptr) 
		{
			helmet_name = Helmet->get_name();
			turn_to_lower_case(helmet_name);
		}
		if (Chestplate != nullptr)
		{
			chestplate_name = Chestplate->get_name();
			turn_to_lower_case(chestplate_name);
		}
		if (Boots != nullptr)
		{
			boots_name = Boots->get_name();
			turn_to_lower_case(boots_name);
		}
	
		if (player_input_noun == weapon_name)
		{
			return main_weapon;
		}
		if ((Helmet != nullptr) && (player_input_noun == helmet_name))
		{
			return Helmet;
		}
		if ((Chestplate != nullptr) && (player_input_noun == chestplate_name))
		{
			return Chestplate;
		}
		if ((Boots != nullptr) && (player_input_noun == boots_name))
		{
			return Boots;
		}

		for (unsigned int i = 0; i < inventory.size(); i++)
		{
			std::string cur_name = inventory[i]->get_name();
			turn_to_lower_case(cur_name);

			if (player_input_noun == cur_name)
			{
				return inventory[i];
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
