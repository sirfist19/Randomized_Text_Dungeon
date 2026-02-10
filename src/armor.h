#ifndef armor_h
#define armor_h
#include "object.h"

class armor : public object
{
protected:
	int defense;
public:
	armor(std::string name, int defense, std::string description) : object(name, description), defense(defense)
	{

	}
	armor(const armor& in) : object(in.name, in.amt, in.description), defense(in.defense)
	{
		//copy constructor for random placement of armor
	}
	virtual armor* clone_armor() = 0;
	virtual void display()
	{
		std::cout << get_display_string();
	}
	virtual std::string get_display_string()
	{
		return(name + " (Defense: " + std::to_string(defense) + ")");
	}
	virtual void display_chest()
	{
		display();
		std::cout << "\n";
	}
	std::string identify()
	{
		return "armor";
	}
	virtual int get_defense()
	{
		return defense;
	}
	virtual void use()
	{
		print("You can't use armor like that.");
	}
};

class helmet : public armor
{
public:
	helmet(std::string name, int defense, std::string description) : armor(name, defense, description)
	{

	}
	std::string identify()
	{
		return "helmet";
	}
};
class chestplate : public armor
{
public:
	chestplate(std::string name, int defense, std::string description) : armor(name, defense, description)
	{

	}
	std::string identify()
	{
		return "chestplate";
	}
};
class boots : public armor
{
public:
	boots(std::string name, int defense, std::string description) : armor(name, defense, description)
	{

	}
	std::string identify()
	{
		return "boots";
	}
};
//chainmail armor - common (worst)
//bronze armor - common
//silver armor - rare
//dragon scale - legendary (best)
class chainmail_helmet : public helmet
{
public:
	chainmail_helmet() : helmet("Chainmail Helmet", CHAINMAIL_HELMET_DEFENSE, "A helmet made of chainmail. It's a little flimsy.") {}
	chainmail_helmet(const chainmail_helmet& in) : helmet(in.name, in.defense, in.description)
	{
		//copy constructor
	}
	virtual object* clone()
	{
		return new chainmail_helmet(*this);
	}
	virtual armor* clone_armor()
	{
		return new chainmail_helmet(*this);
	}
};
class chainmail_chestplate : public chestplate
{
public:
	chainmail_chestplate() : chestplate("Chainmail Chestplate", CHAINMAIL_CHESTPLATE_DEFENSE, "A chestplate made of chainmail. It's a little flimsy.") {}
	chainmail_chestplate(const chainmail_chestplate& in) : chestplate(in.name, in.defense, in.description)
	{
		//copy constructor
	}
	virtual object* clone()
	{
		return new chainmail_chestplate(*this);
	}
	virtual armor* clone_armor()
	{
		return new chainmail_chestplate(*this);
	}
};
class chainmail_boots : public boots
{
public:
	chainmail_boots() : boots("Chainmail Boots", CHAINMAIL_BOOTS_DEFENSE, "A pair of boots made of chainmail. It's a little flimsy.") {}
	chainmail_boots(const chainmail_boots& in) : boots(in.name, in.defense, in.description)
	{
		//copy constructor
	}
	virtual object* clone()
	{
		return new chainmail_boots(*this);
	}
	virtual armor* clone_armor()
	{
		return new chainmail_boots(*this);
	}
};
class bronze_helmet : public helmet
{
public:
	bronze_helmet() : helmet("Bronze Helmet", BRONZE_HELMET_DEFENSE, "A bronze helmet that has a dull brown and green color.") {}
	bronze_helmet(const bronze_helmet& in) : helmet(in.name, in.defense, in.description)
	{
		//copy constructor
	}
	virtual object* clone()
	{
		return new bronze_helmet(*this);
	}
	virtual armor* clone_armor()
	{
		return new bronze_helmet(*this);
	}
};
class bronze_chestplate : public chestplate
{
public:
	bronze_chestplate() : chestplate("Bronze Chestplate", BRONZE_CHESTPLATE_DEFENSE, "A bronze chestplate that has a dull brown and green color.") {}
	bronze_chestplate(const bronze_chestplate& in) : chestplate(in.name, in.defense, in.description)
	{
		//copy constructor
	}
	virtual object* clone()
	{
		return new bronze_chestplate(*this);
	}
	virtual armor* clone_armor()
	{
		return new bronze_chestplate(*this);
	}
};
class bronze_boots : public boots
{
public:
	bronze_boots() : boots("Bronze Boots", BRONZE_BOOTS_DEFENSE, "A pair of bronze boots that has a dull brown and green color.") {}
	bronze_boots(const bronze_boots& in) : boots(in.name, in.defense, in.description)
	{
		//copy constructor
	}
	virtual object* clone()
	{
		return new bronze_boots(*this);
	}
	virtual armor* clone_armor()
	{
		return new bronze_boots(*this);
	}
};
class silver_helmet : public helmet
{
public:
	silver_helmet() : helmet("Silver Helmet", SILVER_HELMET_DEFENSE, "A silver helmet that shines in the light.") {}
	silver_helmet(const silver_helmet& in) : helmet(in.name, in.defense, in.description)
	{
		//copy constructor
	}
	virtual object* clone()
	{
		return new silver_helmet(*this);
	}
	virtual armor* clone_armor()
	{
		return new silver_helmet(*this);
	}
};
class silver_chestplate : public chestplate
{
public:
	silver_chestplate() : chestplate("Silver Chestplate", SILVER_CHESTPLATE_DEFENSE, "A silver chestplate that shines in the light.") {}
	silver_chestplate(const silver_chestplate& in) : chestplate(in.name, in.defense, in.description)
	{
		//copy constructor
	}
	virtual object* clone()
	{
		return new silver_chestplate(*this);
	}
	virtual armor* clone_armor()
	{
		return new silver_chestplate(*this);
	}
};
class silver_boots : public boots
{
public:
	silver_boots() : boots("Silver Boots", SILVER_BOOTS_DEFENSE, "A pair of silver boots that shines in the light.") {}
	silver_boots(const silver_boots& in) : boots(in.name, in.defense, in.description)
	{
		//copy constructor
	}
	virtual object* clone()
	{
		return new silver_boots(*this);
	}
	virtual armor* clone_armor()
	{
		return new silver_boots(*this);
	}
};
class dragon_scale_helmet : public helmet
{
public:
	dragon_scale_helmet() : helmet("Dragon Scale Helmet", DRAGON_SCALE_HELMET_DEFENSE, "A dragon scale helmet that shines in the light.") {}
	dragon_scale_helmet(const dragon_scale_helmet& in) : helmet(in.name, in.defense, in.description)
	{
		//copy constructor
	}
	virtual object* clone()
	{
		return new dragon_scale_helmet(*this);
	}
	virtual armor* clone_armor()
	{
		return new dragon_scale_helmet(*this);
	}
};
class dragon_scale_chestplate : public chestplate
{
public:
	dragon_scale_chestplate() : chestplate("Dragon Scale Chestplate", DRAGON_SCALE_CHESTPLATE_DEFENSE, "A dragon scale chestplate that shines in the light.") {}
	dragon_scale_chestplate(const dragon_scale_chestplate& in) : chestplate(in.name, in.defense, in.description)
	{
		//copy constructor
	}
	virtual object* clone()
	{
		return new dragon_scale_chestplate(*this);
	}
	virtual armor* clone_armor()
	{
		return new dragon_scale_chestplate(*this);
	}
};
class dragon_scale_boots : public boots
{
public:
	dragon_scale_boots() : boots("Dragon Scale Boots", DRAGON_SCALE_BOOTS_DEFENSE, "A pair of Dragon Scale boots that shines in the light.") {}
	dragon_scale_boots(const dragon_scale_boots& in) : boots(in.name, in.defense, in.description)
	{
		//copy constructor
	}
	virtual object* clone()
	{
		return new dragon_scale_boots(*this);
	}
	virtual armor* clone_armor()
	{
		return new dragon_scale_boots(*this);
	}
};

armor* pick_random_common_armor();
armor* pick_random_rare_armor();
armor* pick_random_legendary_armor();

class armor_list
{
private:
	std::vector<armor*> common;
	std::vector<armor*> rare;
	std::vector<armor*> legendary;
public:
	armor_list()
	{
		common.push_back(new chainmail_helmet());
		common.push_back(new chainmail_chestplate());
		common.push_back(new chainmail_boots());
		common.push_back(new bronze_helmet());
		common.push_back(new bronze_chestplate());
		common.push_back(new bronze_boots());
		rare.push_back(new silver_helmet());
		rare.push_back(new silver_chestplate());
		rare.push_back(new silver_boots());
		legendary.push_back(new dragon_scale_helmet());
		legendary.push_back(new dragon_scale_chestplate());
		legendary.push_back(new dragon_scale_boots());
	}
	~armor_list()
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
	std::vector<armor*> get_common_list()
	{
		return common;
	}
	std::vector<armor*> get_rare_list()
	{
		return rare;
	}
	std::vector<armor*> get_legendary_list()
	{
		return legendary;
	}
};
#endif
