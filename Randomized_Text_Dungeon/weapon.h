#ifndef weapon_h
#define weapon_h
#include "object.h"

class weapon : public object
{
protected:
	int damage;
public:
	weapon(std::string name, int damage, std::string description): object(name, description), damage(damage)
	{

	}
	virtual void display()
	{
		std::cout << "Weapon: " << name<<"\n";
		std::cout << "\tDamage - " << damage<<"\n";
		std::cout << "\tDescription - ";
		print(description);
	}
	virtual void display_chest()
	{
		std::cout << name << " (Damage: " << damage << ")\n";
	}
	std::string identify()
	{
		return "weapon";
	}
	int get_damage()
	{
		return damage;
	}
};
class fists : public weapon
{
public: 
	fists() : weapon("Fists", 1, "Just you, your fists and the world."){}
};
class claws : public weapon
{
public:
	claws() : weapon("Claws", 1, "Razor sharp claws that can tear the skin off of an opponent."){}
};
class slime_shooter : public weapon
{
public:
	slime_shooter() : weapon("Slime Shooter", 1, "Slimes attack their enemies by shooting out a small ball of slime at their targets. They may hurt themselves in the process.") {}
};
class dagger : public weapon
{
public:
	dagger() : weapon("Dagger", 2, "A small iron dagger. Not very effective but better than your fists."){}
};
class shortsword : public weapon
{
public:
	shortsword() : weapon("Shortsword", 3, "A small sword."){}
};
class sword : public weapon
{
public:
	sword() : weapon("Sword", 5, "A typical iron sword with a bronze hilt") {}
};
class axe : public weapon
{
public:
	axe() : weapon("Axe", 7, "A two-handed axe that needs lots of force to yield.") {}
};
class bow : public weapon
{
public:
	bow() : weapon("Bow", 3, "Long ranged weapon. Hard to aim however. Without much skill a bowman will miss many shots.") {}
};
class daedric_sword : public weapon
{
public:
	daedric_sword() : weapon("Daedric Sword", 20, "This legendary blade is made from volcanic magma that has been infused with magical dragon bone. Whoever yields this blade can kill their opponents very quickly.") {}
};


#endif