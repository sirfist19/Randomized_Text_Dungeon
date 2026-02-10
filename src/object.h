#ifndef object_h
#define object_h
#include "constants.h"
#include <vector>
#include "helper_fxns.h"



class object
{
protected:
	std::string name;
	std::string description;
	int amt; //the amount of items stacked 
public:
	std::string get_name()
	{
		return name;
	}
	virtual std::string get_description()
	{
		return description;
	}
	virtual int get_amt()
	{
		return amt;
	}
	void set_amt(const int& in_amt)
	{
		amt = in_amt;
	}
	 
	virtual std::string identify() { return "object"; }
	virtual void display() = 0;
	virtual std::string get_display_string() = 0;
	virtual void display_chest() = 0;
	virtual void decrease_amt(const int& _amt)
	{
		amt -= _amt;
		if (amt < 0)
			amt = 0;
	}
	virtual bool is_same_type_of_object(object* other)
	{
		bool same_names = name == other->get_name();
		bool same_descriptions = description == other->get_description();
		bool same_types = (this->identify() == other->identify());

		// for default objects
		if (same_names && same_descriptions && same_types)
			return true;
		return false;
	}
	void combine(object* other) //combines the parameter into the current object
	{
		amt += other->amt;
		other = nullptr;
	}
	virtual void use() 
	{
		print("ERROR: You can't use a base class object.");
		exit(1);
	}
	virtual void use(int* cur_room_exits)
	{
		print("ERROR: You can't use a base class object.");
		exit(1);
	}
	//init one obj
	object(std::string name, std::string description):name(name), description(description), amt(1)
	{
		
	}
	virtual ~object(){}
	//init a stack of objs
	object(std::string name, int amt, std::string description) :name(name), description(description), amt(amt)
	{

	}
	object(const object& in, int _amt) //copy constructor
	{
		name = in.name;
		description = in.description;
		amt = _amt;
	}
	virtual object* clone() = 0;
	
};
class key : public object
{
public:
	key(std::string name, int amt, std::string description) : object(name, amt, description)
	{

	}
	virtual void display() = 0;
	virtual std::string get_display_string() = 0;
	virtual void display_chest()
	{
		std::cout << "Key\n";
	}
	virtual std::string identify()
	{
		return "key";
	}
	virtual void use(int* cur_room_exits) = 0;
};

//object level general fxns
std::string get_items_string(std::vector<object*>& items, std::string before, std::string after);
void print_items(std::vector<object*>& items, std::string before, std::string after);
void print_item(object* item, std::string before, std::string after);

class dragon_key : public key
{
public:
	dragon_key() : key("Dragon Key", 1, "A bright golden key with an engraving of a fire breathing dragon on its handle. This is the key to open the Golden Dragon door in the entrance room. Its your way out!")
	{

	}
	dragon_key(const dragon_key& in) : key(in.name, in.amt, in.description)
	{
		//copy constructor
	}
	virtual object* clone()
	{
		return new dragon_key(*this);
	}
	virtual void display()
	{
		std::cout << "Dragon Key";
	}
	virtual std::string get_display_string()
	{
		return ("Dragon Key");
	}
	virtual void display_chest()
	{
		std::cout << "Dragon Key\n";
	}
	virtual std::string identify()
	{
		return "key";
	}
	virtual void use(int* cur_room_exits)
	{
		for (unsigned int i = 0; i < 4; i++)
		{
			if (cur_room_exits[i] == -2)
			{
				winning_screen();
			}
		}
		print("The dragon key doesn't do anything here.");
	}
};
class gold : public object
{
protected:
	
public:
	gold(int amt) : object("Gold", amt, "A pile of shining golden coins. Each piece has the front face has a three-headed dragon and the back has a picture of Emperor Septonius III of the Second Dynasty of Helena.")
	{

	}
	gold() : object("Gold", 1, "A pile of shining golden coins. Each piece has the front face has a three-headed dragon and the back has a picture of Emperor Septonius III of the Second Dynasty of Helena.")
	{

	}
	gold(const gold& in) : object(in.name, in.amt, in.description)
	{
		//copy constructor
	}
	virtual object* clone()
	{
		return new gold(*this);
	}
	virtual void display()
	{
		std::cout << "Gold x"<<amt;
		//std::cout << "\tDescription: ";
		//print(description);
		//print(description);
	}
	virtual std::string get_display_string()
	{
		return ("Gold x" + std::to_string(amt));
	}
	virtual void display_chest()
	{
		std::cout << "Gold\n";
	}
	std::string identify()
	{
		return "gold";
	}
	void use()
	{
		print("Not yet implemented.");
	}
};
class compass : public object
{
protected:

public:
	compass() : object("Compass", 1, "A small metallic compass. It points toward the boss room and the store.")
	{

	}
	compass(const compass& in) : object(in.name, in.amt, in.description)
	{
		//copy constructor
	}
	virtual object* clone()
	{
		return new compass(*this);
	}
	virtual void display()
	{
		std::cout << "Compass";
	}
	virtual std::string get_display_string()
	{
		return ("Compass");
	}
	virtual void display_chest()
	{
		std::cout << "Compass\n";
	}
	std::string identify()
	{
		return "compass";
	}
};

#endif
