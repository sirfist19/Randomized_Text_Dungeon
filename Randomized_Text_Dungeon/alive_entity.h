#ifndef alive_entity_h
#define alive_entity_h
#include "weapon.h"

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
	void increase_max_health(int& amt)
	{
		max_health += amt;
	}
};
class Alive_Entity {
protected:
	std::string name;
	//room* location;
	health_bar* health;
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
	health_bar* get_health() const
	{
		return health;
	}
	weapon* get_weapon() const 
	{
		return main_weapon;
	}
	void set_weapon(object* new_weapon)
	{
		if(new_weapon->identify() == "weapon")
			main_weapon = (weapon*) new_weapon;
	}
	virtual void f() = 0;
	Alive_Entity(std::string name, weapon* main_weapon, int health) : name(name), alive(true), health(new health_bar(health)), main_weapon(main_weapon)
	{

	}
	Alive_Entity() :name(""), alive(false), health(new health_bar(10)), main_weapon(nullptr)
	{

	}
	~Alive_Entity()
	{
		delete health;
		delete main_weapon;
	}
};
#endif


