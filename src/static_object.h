#ifndef static_object_h
#define static_object_h
#include "chest.h"

struct hidden_item
{
	object* item;
	bool hidden;

	hidden_item(object* item_, bool hidden_)
	{
		item = item_;
		hidden = hidden_;
	}
	~hidden_item()
	{
		delete item;
	}
};
class static_object : public object
{
	//an object like a piece of furniture or a waterfall
	//cannot be taken by the player
	//may have an object hidden inside under some condition

	//name, description, amt already included
protected:
	std::vector<hidden_item*> encompassed_objects;//the bool is whether the item is hidden
public:
	static_object(std::string name, std::string description, int amt)
		: object(name, amt, description)
	{

	}
	static_object(std::string name, std::string description)//make only 1 static_object
		: object(name, description)
	{

	}
	static_object(std::string name, std::string description, std::vector<hidden_item*> encompassed)//make only 1 static_object
		: object(name, description), encompassed_objects(encompassed)
	{

	}
	static_object(const static_object& in) //base copy constructor
		: object(in.name, in.amt, in.description)
	{
		for (unsigned int i = 0; i < in.encompassed_objects.size(); i++)
		{
			object* cur_obj_to_clone = in.encompassed_objects[i]->item;
			hidden_item* new_hidden_item = new hidden_item(cur_obj_to_clone->clone(), true);
			encompassed_objects.push_back(new_hidden_item);
		}
	}
	virtual ~static_object()
	{
		for (unsigned int i = 0; i < encompassed_objects.size(); i++)
		{
			delete encompassed_objects[i];
		}
	}
	void reveal_encompassed_objects()
	{
		for (unsigned int i = 0; i < encompassed_objects.size(); i++)
		{
			encompassed_objects[i]->hidden = false;
		}
	}
	virtual std::vector<object*> get_revealed_items() //returns the string to room descriptions
	{
		std::vector<object*> res;
		if (encompassed_objects.empty())
			return res;

		for (unsigned int i = 0; i < encompassed_objects.size(); i++)
		{
			if (encompassed_objects[i]->hidden == false)
			{
				res.push_back(encompassed_objects[i]->item);
			}
		}
		return res;
	}
	virtual std::string get_revealed_items_description() 
	{
		std::vector<object*> cur = get_revealed_items();
		return get_items_string(cur, "", "");
	}
	object* get_matching_revealed_item(std::string player_obj_name)
	{
		for (unsigned int i = 0; i < encompassed_objects.size(); i++)
		{
			std::string cur_name = encompassed_objects[i]->item->get_name();
			turn_to_lower_case(cur_name);

			if ((player_obj_name == cur_name) && (encompassed_objects[i]->hidden == false))
			{
				return encompassed_objects[i]->item;
			}
		}
		return nullptr;
	}
	void remove_revealed_item(object* obj_to_remove)
	{
		for (unsigned int i = 0; i < encompassed_objects.size(); i++)
		{
			if ((obj_to_remove == encompassed_objects[i]->item) && (encompassed_objects[i]->hidden == false))
			{
				encompassed_objects[i] = encompassed_objects[encompassed_objects.size() - 1];
				encompassed_objects.pop_back();
				return;
			}
		}
	}
	virtual std::string identify() { return "static object"; }
	virtual void display() { print(get_display_string()); };
	virtual std::string get_display_string() = 0;
	virtual void display_chest() { display(); };
	virtual static_object* clone() = 0;
};

class Dragon_doors : public static_object
{
protected:
public:
	Dragon_doors()
		: static_object("Dragon Doors",
			"Two giant golden doors stand in front of you. There is a jade carving of a dragon breathing fire that was created with the finest craftmanship. There is a slot for a special-kind of dragon key. Perhaps if you find the dragon key, you can unlock these doors."
		)
	{

	}
	virtual std::string get_display_string() { return get_name(); };
	Dragon_doors(const Dragon_doors& in) : static_object(in)
	{
		//copy constructor
	}
	virtual static_object* clone()
	{
		return new Dragon_doors(*this);
	}
};

class Waterfall : public static_object
{
protected:
public:
	Waterfall() 
		: static_object("Waterfall", 
			"The water crashes down at immense speeds. Going through could really hurt without a good helmet."
			)
	{

	}
	virtual std::string get_display_string() { return get_name(); };
	Waterfall(const Waterfall& in) : static_object(in)
	{
		//copy constructor
	}
	virtual static_object* clone()
	{
		return new Waterfall(*this);
	}
};

class Pit : public static_object
{
protected:
public:
	Pit()
		: static_object("Pit",
			"It's a pitch black giant gaping pit. If it's not bottomless, then it goes down a long way."
		)
	{

	}
	virtual std::string get_display_string() { return get_name(); };
	Pit(const Pit& in) : static_object(in)
	{
		//copy constructor
	}
	virtual static_object* clone()
	{
		return new Pit(*this);
	}
};

class Pedistal : public static_object
{
protected:
public:
	Pedistal()
		: static_object("Pedistal",
			"The pedistal is made out of a black volcanic stone. There is an inscription on it but you cannot read it."
		)
	{
		int num = random(0, 99);
		if (num < PEDISTAL_HAS_ITEM)
		{
			encompassed_objects.push_back(new hidden_item(new lesser_healing_potion(), true));
		}
	}
	virtual std::string get_description() override
	{
		std::string new_description = object::get_description();
		if (!encompassed_objects.empty())
			new_description += (" A " + encompassed_objects[0]->item->get_name() + " sits on the top of the pedistal.");
		return new_description;
	}

	Pedistal(const Pedistal& in) : static_object(in)
	{
		//copy constructor
	}
	virtual static_object* clone() override
	{
		return new Pedistal(*this);
	}
	
	virtual std::string get_display_string() override { return get_name(); };
	virtual std::string get_revealed_items_description() override
	{
		std::vector<object*> main = static_object::get_revealed_items();
		std::string before = "There is ";
		std::string after = " sitting on the pedistal.";

		if (main.empty())
			return "";
		else
			return get_items_string(main, before, after);
	}
};

class Moss : public static_object
{
protected:
public:
	Moss()
		: static_object("Moss",
			"The moss covers the wall in patches in some spots but is very dense in others. You touch the moss and get you hands all wet!"
		)
	{
		int rand_num = random(0, 99);
		if(rand_num < GOLD_SPAWN_IN_MOSS)
			encompassed_objects.push_back(new hidden_item(new gold(random(2,10)), true));
	}
	virtual std::string get_description() override
	{
		std::string new_description = object::get_description();
		if (!encompassed_objects.empty())
			new_description += (" A few gold coins are hidden in the denser portion of the moss.");
		return new_description;
	}
	virtual std::string get_display_string() override { return get_name(); };
	Moss(const Moss& in) : static_object(in)
	{
		//copy constructor
	}
	virtual static_object* clone() override 
	{
		return new Moss(*this);
	}
};

#endif
