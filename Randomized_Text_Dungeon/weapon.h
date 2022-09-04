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
	weapon(const weapon& in) : object(in.name, in.amt, in.description), damage(in.damage)
	{
		//copy constructor for random placement of weapons
	}
	virtual weapon* clone_weapon() = 0;
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
	virtual void use()
	{
		print("You can't use a weapon like that.");
	}
};

class fists : public weapon
{
public: 
	fists() : weapon("Fists", 1, "Just you, your fists and the world."){}
	fists(const fists& in) : weapon(in.name, in.damage, in.description)
	{
		//copy constructor
	}
	virtual object* clone()
	{
		return new fists(*this);
	}
	virtual weapon* clone_weapon()
	{
		return new fists(*this);
	}
};
class claws : public weapon
{
public:
	claws() : weapon("Claws", 1, "Razor sharp claws that can tear the skin off of an opponent."){}
	claws(const claws& in) : weapon(in.name, in.damage, in.description)
	{
		//copy constructor
	}
	virtual object* clone()
	{
		return new claws(*this);
	}
	virtual weapon* clone_weapon()
	{
		return new claws(*this);
	}
};
class slime_shooter : public weapon
{
public:
	slime_shooter() : weapon("Slime Shooter", 1, "Slimes attack their enemies by shooting out a small ball of slime at their targets. They may hurt themselves in the process.") {}
	slime_shooter(const slime_shooter& in) : weapon(in.name, in.damage, in.description)
	{
		//copy constructor
	}
	virtual object* clone()
	{
		return new slime_shooter(*this);
	}
	virtual weapon* clone_weapon()
	{
		return new slime_shooter(*this);
	}
};
class dagger : public weapon
{
public:
	dagger() : weapon("Dagger", 2, "A small iron dagger. Not very effective but better than your fists.") {}
	dagger(const dagger& in) : weapon(in.name, in.damage, in.description)
	{
		//copy constructor
	}
	virtual object* clone()
	{
		return new dagger(*this);
	}
	virtual weapon* clone_weapon()
	{
		return new dagger(*this);
	}
};
class shortsword : public weapon
{
public:
	shortsword() : weapon("Shortsword", 3, "A small sword."){}
	shortsword(const shortsword& in) : weapon(in.name, in.damage, in.description)
	{
		//copy constructor
	}
	virtual object* clone()
	{
		return new shortsword(*this);
	}
	virtual weapon* clone_weapon()
	{
		return new shortsword(*this);
	}
};
class sword : public weapon
{
public:
	sword() : weapon("Sword", 5, "A typical iron sword with a bronze hilt") {}
	sword(const sword& in) : weapon(in.name, in.damage, in.description)
	{
		//copy constructor
	}
	virtual object* clone()
	{
		return new sword(*this);
	}
	virtual weapon* clone_weapon()
	{
		return new sword(*this);
	}
};
class axe : public weapon
{
public:
	axe() : weapon("Axe", 7, "A two-handed axe that needs lots of force to yield.") {}
	axe(const axe& in) : weapon(in.name, in.damage, in.description)
	{
		//copy constructor
	}
	virtual object* clone()
	{
		return new axe(*this);
	}
	virtual weapon* clone_weapon()
	{
		return new axe(*this);
	}
};
class bow : public weapon
{
public:
	bow() : weapon("Bow", 3, "Long ranged weapon. Hard to aim however. Without much skill a bowman will miss many shots.") {}
	bow(const bow& in) : weapon(in.name, in.damage, in.description)
	{
		//copy constructor
	}
	virtual object* clone()
	{
		return new bow(*this);
	}
	virtual weapon* clone_weapon()
	{
		return new bow(*this);
	}
};
class daedric_sword : public weapon
{
public:
	daedric_sword() : weapon("Daedric Sword", 20, "This legendary blade is made from volcanic magma that has been infused with magical dragon bone. Whoever yields this blade can kill their opponents very quickly.") {}
	daedric_sword(const daedric_sword& in) : weapon(in.name, in.damage, in.description)
	{
		//copy constructor
	}
	virtual object* clone()
	{
		return new daedric_sword(*this);
	}
	virtual weapon* clone_weapon()
	{
		return new daedric_sword(*this);
	}
};
class mjolnir : public weapon
{
public:
	mjolnir() : weapon("Mjolnir", 22, "The legendary hammer of the Norse god of thunder Thor.") {}
	mjolnir(const mjolnir& in) : weapon(in.name, in.damage, in.description)
	{
		//copy constructor
	}
	virtual object* clone()
	{
		return new mjolnir(*this);
	}
	virtual weapon* clone_weapon()
	{
		return new mjolnir(*this);
	}
};
class dragon_fire : public weapon
{
public:
	dragon_fire() : weapon("Dragon Fire", 45, "Fire that using the magical power of dragon's can turn anything to dust.") {}
	dragon_fire(const dragon_fire& in) : weapon(in.name, in.damage, in.description)
	{
		//copy constructor
	}
	virtual object* clone()
	{
		return new dragon_fire(*this);
	}
	virtual weapon* clone_weapon()
	{
		return new dragon_fire(*this);
	}
};

class weapon_list
{
private:
	std::vector<weapon*> common;
	std::vector<weapon*> rare;
	std::vector<weapon*> legendary;
public:
	weapon_list()
	{
		common.push_back(new dagger());
		common.push_back(new shortsword());
		common.push_back(new bow());
		rare.push_back(new axe());
		rare.push_back(new sword());
		legendary.push_back(new daedric_sword());
		legendary.push_back(new mjolnir());
	}
	~weapon_list()
	{
		for (unsigned int i = 0; i < common.size(); i++)
		{
			delete common[i];
		}
		for (unsigned int i = 0; i < rare.size(); i++)
		{
			delete rare[i];
		}
		for (unsigned int i = 0; i < legendary.size(); i++)
		{
			delete legendary[i];
		}
	}
	std::vector<weapon*> get_common_list()
	{
		return common;
	}
	std::vector<weapon*> get_rare_list()
	{
		return rare;
	}
	std::vector<weapon*> get_legendary_list()
	{
		return legendary;
	}
};


#endif