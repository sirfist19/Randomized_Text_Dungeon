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
		main_weapon->display();

		std::cout << "\nItems: ";

		if (inventory.size() == 0)
			std::cout << "\n NO ITEMS IN INVENTORY\n";
		for (unsigned int i = 0; i < inventory.size(); i++)
		{
			inventory[i]->display();
		}
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
		std::cout << "\nPlayer: " + name + "     " + "Room: "<<location->get_name()
			<<"-"<<location->get_id()<<
			"     Score: " + std::to_string(0) + "\n";
		//std::cout << std::endl;
		printEquals();
		std::cout << "\n";
	}
};
#endif
