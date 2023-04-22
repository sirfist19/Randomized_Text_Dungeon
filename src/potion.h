#ifndef potion_h
#define potion_h
#include <string>
#include <iostream>
#include "object.h"

class potion : public object //abstract potion class
{
protected:

public:
	potion(int amt, std::string _name, std::string _description) : object(_name, amt, _description)
	{

	}
	std::string identify()
	{
		return "potion";
	}
};

class healing_potion : public potion //abstract healing potion class
{
protected:
	int heal_amt;
public:
	healing_potion(int amt, std::string _name, std::string _description, int healing_amt) : potion(amt, _name, _description), heal_amt(healing_amt)
	{

	}
	std::string identify()
	{
		return "healing potion";
	}
	int get_heal_amt() const
	{
		return heal_amt;
	}
	virtual void display()
	{
		std::cout << name << " (Heals " << heal_amt << " health)";
	}
	virtual std::string get_display_string()
	{
		return(name + " (Heals " + std::to_string(heal_amt) + " health)");
	}
	virtual void display_chest()
	{
		std::cout << name << " (Heals " << heal_amt << " health)\n";
	}
};

class lesser_healing_potion : public healing_potion //abstract healing potion class
{
protected:
public:
	lesser_healing_potion() : healing_potion(1, "Lesser Healing Potion", "When you drink this potion it restores 5 health.", 5)
	{

	}
	lesser_healing_potion(const lesser_healing_potion& in) : healing_potion(in.amt, in.name, in.description, in.heal_amt)
	{
		//copy constructor
	}
	virtual object* clone()
	{
		return new lesser_healing_potion(*this);
	}
};

class common_healing_potion : public healing_potion //abstract healing potion class
{
protected:
public:
	common_healing_potion() : healing_potion(1, "Common Healing Potion", "When you drink this potion it restores 10 health.", 10)
	{

	}
	common_healing_potion(const common_healing_potion& in) : healing_potion(in.amt, in.name, in.description, in.heal_amt)
	{
		//copy constructor
	}
	virtual object* clone() //polymorphism copy constructor
	{
		return new common_healing_potion(*this);
	}
};

class greater_healing_potion : public healing_potion //abstract healing potion class
{
protected:
public:
	greater_healing_potion() : healing_potion(1, "Greater Healing Potion", "When you drink this potion it restores 20 health.", 20)
	{

	}
	greater_healing_potion(const greater_healing_potion& in) : healing_potion(in.amt, in.name, in.description, in.heal_amt)
	{
		//copy constructor
	}
	virtual object* clone()
	{
		return new greater_healing_potion(*this);
	}
	
};
#endif
