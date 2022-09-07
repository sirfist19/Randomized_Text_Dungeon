#ifndef enemy_h
#define enemy_h
#include "alive_entity.h"

enum enemy_tier {
	easy, medium, hard, very_hard
};
class Enemy : public Alive_Entity
{
protected:
	enemy_tier tier;
public:
	Enemy(std::string name, weapon* main_weapon, int health, int level, enemy_tier tier) : Alive_Entity(name, main_weapon, health, level), tier(tier)//for basic enemy
	{

	}
	Enemy(std::string name, weapon* main_weapon, int health, int level, helmet* Helmet, chestplate* Chestplate, boots* Boots, enemy_tier tier) 
		: Alive_Entity(name, main_weapon, health, level, Helmet, Chestplate, Boots), tier(tier)//for basic enemy with armor
	{

	}
	
	virtual std::string name_by_level(const int& level) = 0;
	virtual int health_by_level(const int& level) = 0;
	virtual helmet* helmet_by_level(const int& level) = 0;
	virtual chestplate* chestplate_by_level(const int& level) = 0;
	virtual boots* boots_by_level(const int& level) = 0;
	virtual weapon* weapon_by_level(const int& level) = 0;

	virtual void display_attack_info() {
		std::cout << "Enemy: " << name << "   Level: "<<level<<"\n";
		health->display_health_bar();
		std::cout << "Weapon: " << main_weapon->get_name() << "\n";
		std::cout << "Defense: " << defense << std::endl;
		std::cout << "Armor:\n";
		if (Helmet != nullptr)
		{
			std::cout << "\t- ";
			Helmet->display_chest();
		}
		if (Chestplate != nullptr)
		{
			std::cout << "\t- ";
			Chestplate->display_chest();
		}
		if (Boots != nullptr)
		{
			std::cout << "\t- ";
			Boots->display_chest();
		}
		if (defense == 0)
			std::cout << "NONE\n";
		std::cout << "\n";
	}
};

//easy
class Rat : public Enemy
{
public:
	Rat(int level) : Enemy(name_by_level(level), weapon_by_level(level), health_by_level(level), level, 
		helmet_by_level(level), chestplate_by_level(level), boots_by_level(level), enemy_tier::easy) {}
	virtual std::string name_by_level(const int& level)
	{
		switch (level)
		{
		case 1:
		case 2:
			return "Rat";
			break;
		case 3: 
			return "Giant Buff Rat";
			break;
		case 4:
			return "Giant Buff Armored Rat";
			break;
		default: 
			return "ERROR RAT LEVEL OUT OF RANGE";
		}
	}
	virtual int health_by_level(const int& level) 
	{
		int health;
		switch (level)
		{
		case 1:
			health = random(1, 3);
			break;
		case 2: 
			health = random(5, 7);
			break;
		case 3:
			health = random(8, 12);
			break;
		case 4:
			health = random(15, 20);
			break;
		}
		return health;
	}
	virtual helmet* helmet_by_level(const int& level)
	{
		helmet* helm = nullptr;
		int has_helm = random(0, 99);
		switch (level)
		{
		case 3:
			if (has_helm < 50)
			{
				helm = new chainmail_helmet();
			}
			break;
		case 4:
			if (has_helm < 50)
			{
				helm = new bronze_helmet();
			}
			break;
		}
		return helm;
	}
	virtual chestplate* chestplate_by_level(const int& level)
	{
		chestplate* chest = nullptr;
		int has_chest = random(0, 99);
		switch (level)
		{
		case 3:
			if (has_chest < 50)
			{
				chest = new chainmail_chestplate();
			}
			break;
		case 4:
			if (has_chest < 50)
			{
				chest = new bronze_chestplate();
			}
			break;
		}
		return chest;
	}
	virtual boots* boots_by_level(const int& level)
	{
		boots* boot = nullptr;
		int has_boot = random(0, 99);
		switch (level)
		{
		case 3:
			if (has_boot < 50)
			{
				boot = new chainmail_boots();
			}
			break;
		case 4:
			if (has_boot < 50)
			{
				boot = new bronze_boots();
			}
			break;
		}
		return boot;
	}
	virtual weapon* weapon_by_level(const int& level)
	{
		weapon* cur = nullptr;
		switch (level)
		{
		case 1:
		case 2:
			cur = new claws();
			break;
		case 3:
			cur = new sword();
			break;
		case 4:
			cur = new axe();
			break;
		}
		return cur;
	}
	virtual void f() {}
};
class Slime : public Enemy
{
public:
	Slime(int level) : Enemy(name_by_level(level), weapon_by_level(level), health_by_level(level), level, 
		helmet_by_level(level), chestplate_by_level(level), boots_by_level(level), enemy_tier::easy) {}
	virtual std::string name_by_level(const int& level)
	{
		switch (level)
		{
		case 1:
		case 2:
			return "Slime";
			break;
		case 3:
			return "Giant Slime";
			break;
		default:
			return "ERROR SLIME LEVEL OUT OF RANGE";
		}
	}
	virtual int health_by_level(const int& level)
	{
		int health;
		switch (level)
		{
		case 1:
			health = random(3, 5);
			break;
		case 2:
			health = random(6, 8);
			break;
		case 3:
			health = random(8, 12);
			break;
		}
		return health;
	}
	virtual helmet* helmet_by_level(const int& level)
	{
		helmet* helm = nullptr;
		int has_helm = random(0, 99);
		switch (level)
		{
		case 3:
			if (has_helm < 50)
			{
				helm = new chainmail_helmet();
			}
		}
		return helm;
	}
	virtual chestplate* chestplate_by_level(const int& level)
	{
		chestplate* chest = nullptr;
		int has_chest = random(0, 99);
		switch (level)
		{
		case 3:
			if (has_chest < 50)
			{
				chest = new chainmail_chestplate();
			}
		}
		return chest;
	}
	virtual boots* boots_by_level(const int& level)
	{
		boots* boot = nullptr;
		int has_boot = random(0, 99);
		switch (level)
		{
		case 3:
			if (has_boot < 50)
			{
				boot = new chainmail_boots();
			}
		}
		return boot;
	}
	virtual weapon* weapon_by_level(const int& level)
	{
		weapon* cur = nullptr;
		switch (level)
		{
		case 1:
		case 2:
			cur = new slime_shooter();
			break;
		case 3:
			cur = new sword();
			break;
		}
		return cur;
	}
	virtual void f() {};
};


//mid
class Skeleton : public Enemy
{
public:
	Skeleton(int level) : Enemy(name_by_level(level), weapon_by_level(level), health_by_level(level), level,
		helmet_by_level(level), chestplate_by_level(level), boots_by_level(level), enemy_tier::medium){}
	virtual std::string name_by_level(const int& level)
	{
		switch (level)
		{
		case 1:
			return "Skeleton";
			break;
		case 2:
			return "Armored Skeleton";
			break;
		default:
			return "ERROR SKELETON LEVEL OUT OF RANGE";
		}
	}
	virtual int health_by_level(const int& level)
	{
		int health;
		switch (level)
		{
		case 1:
			health = random(6, 10);
			break;
		case 2:
			health = random(11, 16);
			break;
		}
		return health;
	}
	virtual helmet* helmet_by_level(const int& level)
	{
		helmet* helm = nullptr;
		int has_helm = random(0, 99);
		switch (level)
		{
		case 2:
			if (has_helm < 50)
			{
				helm = new bronze_helmet();
			}
		}
		return helm;
	}
	virtual chestplate* chestplate_by_level(const int& level)
	{
		chestplate* chest = nullptr;
		int has_chest = random(0, 99);
		switch (level)
		{
		case 2:
			if (has_chest < 50)
			{
				chest = new bronze_chestplate();
			}
		}
		return chest;
	}
	virtual boots* boots_by_level(const int& level)
	{
		boots* boot = nullptr;
		int has_boot = random(0, 99);
		switch (level)
		{
		case 2:
			if (has_boot < 50)
			{
				boot = new bronze_boots();
			}
		}
		return boot;
	}
	virtual weapon* weapon_by_level(const int& level)
	{
		weapon* cur = nullptr;
		switch (level)
		{
		case 1:
			cur = new bow();
			break;
		case 2:
			cur = new long_bow();
			break;
		}
		return cur;
	}
	virtual void f() {};
};
class Goblin : public Enemy
{
public:
	Goblin(int level) : Enemy(name_by_level(level), weapon_by_level(level), health_by_level(level), level, 
		helmet_by_level(level), chestplate_by_level(level), boots_by_level(level), enemy_tier::medium) {}
	virtual std::string name_by_level(const int& level)
	{
		return "Goblin";
	}
	virtual int health_by_level(const int& level)
	{
		int health;
		switch (level)
		{
		case 1:
			health = random(10, 14);
			break;
		case 2:
			health = random(15, 20);
			break;
		}
		return health;
	}
	virtual helmet* helmet_by_level(const int& level)
	{
		helmet* helm = nullptr;
		int has_helm = random(0, 99);
		switch (level)
		{
		case 2:
			if (has_helm < 50)
			{
				helm = new bronze_helmet();
			}
		}
		return helm;
	}
	virtual chestplate* chestplate_by_level(const int& level)
	{
		chestplate* chest = nullptr;
		int has_chest = random(0, 99);
		switch (level)
		{
		case 2:
			if (has_chest < 50)
			{
				chest = new bronze_chestplate();
			}
		}
		return chest;
	}
	virtual boots* boots_by_level(const int& level)
	{
		boots* boot = nullptr;
		int has_boot = random(0, 99);
		switch (level)
		{
		case 2:
			if (has_boot < 50)
			{
				boot = new bronze_boots();
			}
		}
		return boot;
	}
	virtual weapon* weapon_by_level(const int& level)
	{
		weapon* cur = nullptr;
		switch (level)
		{
		case 1:
			cur = new shortsword();
			break;
		case 2:
			cur = new sword();
			break;
		}
		return cur;
	}
	virtual void f() {};
};

//hard
class Orc : public Enemy
{
public:
	Orc(int level) : Enemy(name_by_level(level), weapon_by_level(level), health_by_level(level), level, 
		helmet_by_level(level), chestplate_by_level(level), boots_by_level(level), enemy_tier::hard){}
	virtual std::string name_by_level(const int& level)
	{
		return "Orc";
	}
	virtual int health_by_level(const int& level)
	{
		int health;
		switch (level)
		{
		case 1:
			health = random(15, 20);
			break;
		case 2:
			health = random(23, 30);
			break;
		}
		return health;
	}
	virtual helmet* helmet_by_level(const int& level)
	{
		helmet* helm = nullptr;
		int has_helm = random(0, 99);
		switch (level)
		{
		case 1:
			if (has_helm < 50)
			{
				helm = new bronze_helmet();
			}
			break;
		case 2:
			if (has_helm < 50)
			{
				helm = new silver_helmet();
			}
			break;
		}
		return helm;
	}
	virtual chestplate* chestplate_by_level(const int& level)
	{
		chestplate* chest = nullptr;
		int has_chest = random(0, 99);
		switch (level)
		{
		case 1:
			if (has_chest < 50)
			{
				chest = new bronze_chestplate();
			}
			break;
		case 2:
			if (has_chest < 50)
			{
				chest = new silver_chestplate();
			}
			break;
		}
		return chest;
	}
	virtual boots* boots_by_level(const int& level)
	{
		boots* boot = nullptr;
		int has_boot = random(0, 99);
		switch (level)
		{
		case 1:
			if (has_boot < 50)
			{
				boot = new bronze_boots();
			}
			break;
		case 2:
			if (has_boot < 50)
			{
				boot = new silver_boots();
			}
			break;
		}
		return boot;
	}
	virtual weapon* weapon_by_level(const int& level)
	{
		weapon* cur = nullptr;
		switch (level)
		{
		case 1:
			cur = new axe();
			break;
		case 2:
			cur = new long_axe();
			break;
		}
		return cur;
	}
	virtual void f() {};
};

//very hard
class Phantom_Knight : public Enemy
{
public:
	Phantom_Knight(int level) : Enemy(name_by_level(level), weapon_by_level(level), health_by_level(level), level, 
		helmet_by_level(level), chestplate_by_level(level), boots_by_level(level), enemy_tier::very_hard) {}
	virtual std::string name_by_level(const int& level)
	{
		return "Phantom Knight";
	}
	virtual int health_by_level(const int& level)
	{
		int health;
		switch (level)
		{
		case 1:
			health = random(30, 40);
			break;
		}
		return health;
	}
	virtual helmet* helmet_by_level(const int& level)
	{
		helmet* helm = nullptr;
		int has_helm = random(0, 99);
		switch (level)
		{
		case 1:
			if (has_helm < 50)
			{
				helm = new dragon_scale_helmet();
			}
			break;
		}
		return helm;
	}
	virtual chestplate* chestplate_by_level(const int& level)
	{
		chestplate* chest = nullptr;
		int has_chest = random(0, 99);
		switch (level)
		{
		case 1:
			if (has_chest < 50)
			{
				chest = new dragon_scale_chestplate();
			}
			break;
		}
		return chest;
	}
	virtual boots* boots_by_level(const int& level)
	{
		boots* boot = nullptr;
		int has_boot = random(0, 99);
		switch (level)
		{
		case 1:
			if (has_boot < 50)
			{
				boot = new dragon_scale_boots();
			}
			break;
		}
		return boot;
	}
	virtual weapon* weapon_by_level(const int& level)
	{
		weapon* cur = nullptr;
		switch (level)
		{
		case 1:
			cur = new daedric_sword();
			break;
		}
		return cur;
	}
	virtual void f() {};
};
class Dragon : public Enemy
{
public:
	Dragon(int level) : Enemy(name_by_level(level), weapon_by_level(level), health_by_level(level), level, 
		helmet_by_level(level), chestplate_by_level(level), boots_by_level(level), enemy_tier::very_hard) {}
	virtual std::string name_by_level(const int& level)
	{
		return "Red Dragon";
	}
	virtual int health_by_level(const int& level)
	{
		int health;
		switch (level)
		{
		case 1:
			health = random(55, 65);
			break;
		}
		return health;
	}
	virtual helmet* helmet_by_level(const int& level)
	{
		return new dragon_scale_helmet();
	}
	virtual chestplate* chestplate_by_level(const int& level)
	{
		return new dragon_scale_chestplate();
	}
	virtual boots* boots_by_level(const int& level)
	{
		return new dragon_scale_boots();
	}
	virtual weapon* weapon_by_level(const int& level)
	{
		return new dragon_fire();
	}
	virtual void f() {};
};

#endif

