#ifndef alive_entity_h
#define alive_entity_h
#include "weapon.h"
#include "armor.h"

class health_bar
{
private:
	int health;
	int max_health;

public:
	health_bar(int _max_health)
	{
		health = _max_health;
		max_health = _max_health;
	}
	int get_health()
	{
		return health;
	}
	int get_max_health()
	{
		return max_health;
	}
	void display_health_bar()
	{
		std::cout << "Health: " << health << "/" << max_health << std::endl;
	}
	virtual void kill()//kills the entity
	{
		health = 0;
	}
	void damage(int& amt)
	{
		health -= amt;

		if (health <= 0)
		{
			health = 0;
		}
	}
	bool health_is_full()
	{
		if (health == max_health)
			return true;
		return false;
	}
	void heal(int& amt) {
		health += amt;

		if (health > max_health)
		{
			health = max_health;
		}
	}
	void heal_to_full()
	{
		health = max_health;
	}
	void increase_max_health(int& amt)
	{
		max_health += amt;
	}
};
class Alive_Entity {
protected:
	std::string name;
	health_bar* health;
	int level;
	int defense;
	helmet* Helmet;
	chestplate* Chestplate;
	boots* Boots;

	weapon* main_weapon;
	bool alive;

public:
	bool is_alive()
	{
		if (health->get_health() <= 0)
			alive = false;
		return alive;
	}
	
	std::string get_name() const
	{
		return name;
	}
	int get_level() const
	{
		return level;
	}
	virtual void level_up()
	{
		level++;
	}
	health_bar* get_health() const
	{
		return health;
	}
	int get_defense() const
	{
		return defense;
	}
	weapon* get_weapon() const 
	{
		return main_weapon;
	}
	helmet* get_helmet() const
	{
		return Helmet;
	}
	chestplate* get_chestplate() const
	{
		return Chestplate;
	}
	boots* get_boots() const
	{
		return Boots;
	}
	void set_weapon(object* new_weapon)
	{
		if(new_weapon->identify() == "weapon")
			main_weapon = (weapon*) new_weapon;
	}
	void display_armor()
	{
		std::cout << "Armor: \n";
		std::cout << "\tHelmet: ";
		if (Helmet == nullptr)
			std::cout << " NONE\n";
		else
			Helmet->display_chest();
		std::cout << "\tChestplate: ";
		if (Chestplate == nullptr)
			std::cout << " NONE\n";
		else
			Chestplate->display_chest();
		std::cout << "\tBoots: ";
		if (Boots == nullptr)
			std::cout << " NONE\n";
		else
			Boots->display_chest();
		std::cout << std::endl;
	}
	virtual void compute_stats()
	{
		//calculate defense based on current armor
		int helmet_def = 0;
		int chestplate_def = 0;
		int boots_def = 0;
		if(Helmet != nullptr)
			helmet_def = Helmet->get_defense();
		if(Chestplate != nullptr)
			chestplate_def = Chestplate->get_defense();
		if(Boots != nullptr)
			boots_def = Boots->get_defense();

		defense = helmet_def + chestplate_def + boots_def;
	}
	virtual void f() = 0;
	Alive_Entity(std::string name, weapon* main_weapon, int health, int level) //constructor for no armor
		: name(name), alive(true), health(new health_bar(health)), main_weapon(main_weapon)
		, defense(0), Helmet(nullptr), Chestplate(nullptr), Boots(nullptr), level(level)
	{

	}
	Alive_Entity(std::string name, weapon* main_weapon, int health, int level, helmet* Helmet, chestplate* Chestplate, boots* Boots)
		: name(name), alive(true), health(new health_bar(health)), main_weapon(main_weapon)
		, defense(0), Helmet(Helmet), Chestplate(Chestplate), Boots(Boots), level(level)
	{
		compute_stats();//calculate the defense based on new armor
	}
	Alive_Entity() :name(""), alive(false), health(new health_bar(10)), main_weapon(nullptr)
	{

	}
	virtual ~Alive_Entity()
	{
		delete health;
		delete main_weapon;
		if(Helmet != nullptr)
			delete Helmet;

		if(Chestplate != nullptr)
			delete Chestplate;

		if(Boots != nullptr)
			delete Boots;
	}
};
#endif


