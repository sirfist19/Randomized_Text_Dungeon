#ifndef static_object_h
#define static_object_h
#include "chest.h"
#include "db/static_objects_db.h"


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
protected:
    std::string static_id; // e.g. "static.pit"
    std::vector<hidden_item*> encompassed_objects;

public:
    // NEW: Construct from an id, and load name/description from StaticObjectsDB
    static_object(const std::string& id)
        : object(StaticObjectsDB::get(id).name, StaticObjectsDB::get(id).description),
          static_id(id)
    {
    }

    // Keep your existing constructors for now if other code uses them
    static_object(std::string name, std::string description, int amt)
        : object(name, amt, description)
    {
    }

    static_object(std::string name, std::string description)
        : object(name, description)
    {
    }

    static_object(std::string name, std::string description, std::vector<hidden_item*> encompassed)
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

class Dragon_doors : public static_object {
public:
    Dragon_doors() : static_object("static.dragon_doors") {}
    virtual std::string get_display_string() override { return get_name(); }
    Dragon_doors(const Dragon_doors& in) : static_object(in) {}
    virtual static_object* clone() override { return new Dragon_doors(*this); }
};

class Waterfall : public static_object {
public:
    Waterfall() : static_object("static.waterfall") {}
    virtual std::string get_display_string() override { return get_name(); }
    Waterfall(const Waterfall& in) : static_object(in) {}
    virtual static_object* clone() override { return new Waterfall(*this); }
};

class Pit : public static_object {
public:
    Pit() : static_object("static.pit") {}
    virtual std::string get_display_string() override { return get_name(); }
    Pit(const Pit& in) : static_object(in) {}
    virtual static_object* clone() override { return new Pit(*this); }
};
class Pedistal : public static_object {
public:
    Pedistal() : static_object("static.pedestal")
    {
        int num = random(0, 99);
        if (num < PEDISTAL_HAS_ITEM) {
            encompassed_objects.push_back(new hidden_item(new lesser_healing_potion(), true));
        }
    }

    virtual std::string get_description() override
    {
        std::string new_description = object::get_description();

        if (!encompassed_objects.empty()) {
            // Use message template from JSON if present
            std::string tmpl = StaticObjectsDB::message_or_empty("static.pedestal", "item_on_pedestal");
            if (tmpl.empty()) {
                new_description += (" A " + encompassed_objects[0]->item->get_name() + " sits on the top of the pedistal.");
            } else {
                // replace {item}
                std::string s = tmpl;
                // simple replace (inline)
                size_t pos = s.find("{item}");
                if (pos != std::string::npos) s.replace(pos, 6, encompassed_objects[0]->item->get_name());
                new_description += (" " + s);
            }
        }
        return new_description;
    }

    Pedistal(const Pedistal& in) : static_object(in) {}
    virtual static_object* clone() override { return new Pedistal(*this); }
    virtual std::string get_display_string() override { return get_name(); }
};


class Moss : public static_object {
public:
    Moss() : static_object("static.moss")
    {
        int rand_num = random(0, 99);
        if (rand_num < GOLD_SPAWN_IN_MOSS) {
            encompassed_objects.push_back(new hidden_item(new gold(random(2,10)), true));
        }
    }

    virtual std::string get_description() override
    {
        std::string new_description = object::get_description();

        if (!encompassed_objects.empty()) {
            std::string msg = StaticObjectsDB::message_or_empty("static.moss", "hidden_gold_found");
            if (msg.empty()) msg = "A few gold coins are hidden in the denser portion of the moss.";
            new_description += (" " + msg);
        }
        return new_description;
    }

    virtual std::string get_display_string() override { return get_name(); }
    Moss(const Moss& in) : static_object(in) {}
    virtual static_object* clone() override { return new Moss(*this); }
};

class Boulder : public static_object {
public:
    Boulder() : static_object("static.boulder") {}
    virtual std::string get_display_string() override { return get_name(); }
    Boulder(const Boulder& in) : static_object(in) {}
    virtual static_object* clone() override { return new Boulder(*this); }
};

class Dining_Table : public static_object {
public:
    Dining_Table() : static_object("static.dining_table") {}
    virtual std::string get_display_string() override { return get_name(); }
    Dining_Table(const Dining_Table& in) : static_object(in) {}
    virtual static_object* clone() override { return new Dining_Table(*this); }
};

#endif
