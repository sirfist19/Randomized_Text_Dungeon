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

enum Status_Effect_Type {
	BURN,
	POISON_,
	FROZEN,
	PARALYSIS,
	NONE_
};

class Status_Effect {
private: 
	Status_Effect_Type type;
	int turns_since_inflicted;

public:
	Status_Effect(Status_Effect_Type type) 
	: type(type), turns_since_inflicted(0)
	{

	}
	void increase_turns_since_inflicted()
	{
		turns_since_inflicted++;
	}
	int get_turns_since_inflicted()
	{
		return turns_since_inflicted;
	}
	void reset_status_effect() 
	{
		type = Status_Effect_Type::NONE_;
		turns_since_inflicted = 0;
	}
	bool attempt_unfreeze() {
		int chance = turns_since_inflicted * random(1,35);
		if (chance > UNFREEZE_CHANCE) 
			return true;
		return false;
	}
	bool attempt_paralysis_move() {
		int chance = random(0,100);
		if (chance < PARALYSIS_NOT_MOVE_CHANCE) 
			return true;
		return false;
	}
	std::string get_display_str()
	{
		switch(type) {
			case Status_Effect_Type::BURN:
				return "Burn";
			case Status_Effect_Type::POISON_:
				return "Poisoned";
			case Status_Effect_Type::PARALYSIS:
				return "Paralyzed";
			case Status_Effect_Type::FROZEN:
				return "Frozen";
			case Status_Effect_Type::NONE_:
				return "None";
			default:
				return "Unrecognized status type";
		}
	}
	Status_Effect_Type get_status_effect_type()
	{
		return type;
	}
	bool already_has_status_effect()
	{
		return type != Status_Effect_Type::NONE_;
	}
};

class Alive_Entity {
protected:
	std::string name;
	health_bar* health;
	bool alive;

	int level;
	int defense;

	// armor
	helmet* Helmet;
	chestplate* Chestplate;
	boots* Boots;

	// weapons
	weapon* main_weapon;
	
	// status condition
	Status_Effect* status_effect;

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
	Status_Effect* get_status_effect()
	{
		return status_effect;
	}
	void set_status(Status_Effect_Type type) {
		status_effect = new Status_Effect(type);
	}
	void set_weapon(object* new_weapon)
	{
		if(new_weapon->identify() == "weapon")
			main_weapon = (weapon*) new_weapon;
	}
	void display_armor() const
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
		, status_effect(new Status_Effect(Status_Effect_Type::NONE_))
	{

	}
	Alive_Entity(std::string name, weapon* main_weapon, int health, int level, helmet* Helmet, chestplate* Chestplate, boots* Boots)
		: name(name), alive(true), health(new health_bar(health)), main_weapon(main_weapon)
		, defense(0), Helmet(Helmet), Chestplate(Chestplate), Boots(Boots), level(level)
		, status_effect(new Status_Effect(Status_Effect_Type::NONE_))
	{
		compute_stats();//calculate the defense based on new armor
	}
	Alive_Entity() :name(""), alive(false), health(new health_bar(10))
		, main_weapon(nullptr), status_effect(new Status_Effect(Status_Effect_Type::NONE_))
	{

	}
	virtual ~Alive_Entity()
	{
		delete health;
		delete main_weapon;
		delete status_effect;
		if(Helmet != nullptr)
			delete Helmet;

		if(Chestplate != nullptr)
			delete Chestplate;

		if(Boots != nullptr)
			delete Boots;
	}
};
#endif


