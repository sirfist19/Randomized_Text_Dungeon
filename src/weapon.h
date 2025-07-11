#ifndef weapon_h
#define weapon_h
#include "object.h"
#include <cmath>
#include <iomanip>
#include <sstream>

enum Weapon_Effect {
	BURNING,
	CRIT,
	POISON,
	ACCURACY_BOOST,
	DAMAGE_BOOST,
	FREEZING,
	ELECTRIFIED,
	NONE // the highest enum value
};
class Weapon_Enchantment {
	private:
		Weapon_Effect effect;
		float chance;
		float multiplier;
	public:
		Weapon_Enchantment(Weapon_Effect effect, float chance, float multiplier) 
			: effect(effect), chance(chance), multiplier(multiplier)
		{

		}
		bool are_same_enchantment(Weapon_Enchantment other) const 
		{
			return (effect == other.get_effect() 
			&& chance == other.get_chance()
			&& multiplier == other.get_multiplier());
		}
		Weapon_Effect get_effect() const
		{
			return effect;
		}
		float get_chance() const 
		{
			return chance;
		}
		float get_multiplier() const
		{
			return multiplier;
		}
		virtual std::string get_display_str() 
		{
			// cut off extra decimals for the multiplier
			std::stringstream stream;
			stream << std::fixed << std::setprecision(1) << multiplier;

			switch(effect) {
				case Weapon_Effect::ACCURACY_BOOST:
					return "Accuracy boosted " + stream.str();
				case Weapon_Effect::BURNING:
					return "BURNING " + std::to_string(int(chance*100)) + "% chance";
				case Weapon_Effect::CRIT:
					return "Crit " + std::to_string(int(chance*100)) + "% chance";
				case Weapon_Effect::DAMAGE_BOOST:
					return "Damage boosted " + stream.str();
				case Weapon_Effect::POISON:
					return "POISON " + std::to_string(int(chance*100)) + "% chance";
				case Weapon_Effect::ELECTRIFIED:
					return "PARALYSIS " + std::to_string(int(chance*100)) + "% chance";
				case Weapon_Effect::FREEZING:
					return "FREEZING " + std::to_string(int(chance*100)) + "% chance";
				case Weapon_Effect::NONE:
				default:
					return "NONE";
			}
		}
		Weapon_Effect use() 
		// uses the enchantment in battle to see if a 
		//		status will or will not be inflicted
		{
			int rand_int = random(0,99);

			switch(effect) {
				case Weapon_Effect::BURNING:
				case Weapon_Effect::CRIT:
				case Weapon_Effect::POISON:
				case Weapon_Effect::FREEZING:
				case Weapon_Effect::ELECTRIFIED:
					if (rand_int < int(chance*100))
						return effect;
					else {
						return Weapon_Effect::NONE;
					}
				default:
					return Weapon_Effect::NONE;
			}
		}
		bool enchanment_is_none() 
		{
			return effect == Weapon_Effect::NONE;
		}
};

class weapon : public object
{
protected:
	int damage;
	int hit_rate;
	Weapon_Enchantment enchantment;

public:
	weapon(std::string name
		, int damage
		, int hit_rate
		, std::string description
		, Weapon_Enchantment enchantment = Weapon_Enchantment(Weapon_Effect::NONE, 0, 0)) 
	: object(name, description), damage(damage), hit_rate(hit_rate), 
		enchantment(enchantment) // chance, then multiplier
	{
		
	}
	weapon(const weapon& in) : object(in.name, in.amt, in.description)
	, damage(in.damage), enchantment(in.enchantment)
	{
		//copy constructor for random placement of weapons
	}
	virtual weapon* clone_weapon() = 0;
	virtual void display()
	{
		std::cout << get_display_string();

	}
	virtual std::string get_display_string()
	{
		return(name 
		+ " (Base Damage: " + std::to_string(damage) 
		+ ", Base Hit Rate: " + std::to_string(hit_rate) 
		+ ", Enchantment: " + enchantment.get_display_str() + ")");
	}
	virtual void display_chest()
	{
		display();
		std::cout << "\n";
	}
	bool hasEnchantment() {
		return !enchantment.enchanment_is_none();
	}
	std::string get_enchantment_str() {
		return enchantment.get_display_str();
	}
	std::string identify()
	{
		return "weapon";
	}
	
	int get_light_damage() const
	{
		return damage;
	}
	int get_heavy_damage() const
	{
		return (int)(damage * HEAVY_DAMAGE_MULTIPLIER);
	}
	int get_light_hit_rate() const
	{
		return hit_rate;
	}
	int get_heavy_hit_rate() const
	{
		return hit_rate - HEAVY_HIT_RATE_REDUCER;
	}
	
	Weapon_Effect use_enchantment() 
	{
		return enchantment.use();
	}
	Weapon_Enchantment get_enchantment() const 
	{
		return enchantment;
	}
	bool have_same_enchantment(weapon* other) 
	{	
		return enchantment.are_same_enchantment(other->get_enchantment());
	}
	void add_enchantment(Weapon_Effect effect, float chance, float multiplier) 
	{
		enchantment = Weapon_Enchantment(effect, chance, multiplier);
		update_stats_from_enchantment();
	}
	virtual bool is_same_type_of_object(object* other)
	{
		bool same_names = name == other->get_name();
		bool same_descriptions = description == other->get_description();
		bool same_types = (this->identify() == other->identify());

		// check to see if the objects are weapons
		std::string type = this->identify();
		if ((type == "weapon") && same_types) 
		{
			bool same_enchantments = this->have_same_enchantment((weapon*)other);
			if (same_names && same_descriptions
			&& same_enchantments
			)
				return true;
			return false;
		}

		// else call the base version
		return object::is_same_type_of_object(other);
	}
	void add_random_enchantment() 
	{
		// randomly add enchantment here!
		
		int rand_enchant = random(0, 99);

		if (rand_enchant < ENCHANT_CHANCE) 
		{
			// pick an enchantment to do
			Weapon_Effect effect = Weapon_Effect(random(0, Weapon_Effect::NONE-1));

			// pick a value for multiplier a value from 1 to 1.5 rounded to 1 decimal place
			float multiplier = 1.0 + random(0,50)/100.0;
			multiplier = std::round(multiplier*10)/ 10.0;
			
			// pick a value for chance 0.1 to 0.5
			float chance = random(10, 50) / 100.0;
			chance = std::round(chance*10)/ 10.0;
			//float chance = 1;
			//std::cout<<"Mult:"<<multiplier<<"Chance"<<chance<<std::endl;
			this->add_enchantment(effect, chance, multiplier);
		}
		update_stats_from_enchantment();
	}
	void update_stats_from_enchantment()
	{
		// update the damage and accuracy if needed
		if (enchantment.get_effect() == Weapon_Effect::DAMAGE_BOOST) 
		{
			damage *= enchantment.get_multiplier();
		}
		else if (enchantment.get_effect() == Weapon_Effect::ACCURACY_BOOST)
		{
			hit_rate *= enchantment.get_multiplier();
			if (hit_rate > 100)
			{
				hit_rate = 100;
			}
		}
	}
	virtual void use()
	{
		print("You can't use a weapon like that.");
	}
};

/* 
	Definitions of all actual non ABC weapons
*/
class fists : public weapon
{
public: 
	fists() : weapon("Fists", 1, 95, "Just you, your fists and the world."){}
	fists(const fists& in) : weapon(in.name, in.damage, in.hit_rate, in.description, in.enchantment)
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
	claws() : weapon("Claws", 3, 80, "Razor sharp claws that can tear the skin off of an opponent."){}
	claws(const claws& in) : weapon(in.name, in.damage, in.hit_rate, in.description, in.enchantment)
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
class broken_slime_shooter : public weapon
{
public:
	broken_slime_shooter() : weapon("Broken Slime Shooter", 3, 50, "Slimes attack their enemies by shooting out a small ball of slime at their targets. They may hurt themselves in the process.") {}
	broken_slime_shooter(const broken_slime_shooter& in) : weapon(in.name, in.damage, in.hit_rate, in.description, in.enchantment)
	{
		//copy constructor
	}
	virtual object* clone()
	{
		return new broken_slime_shooter(*this);
	}
	virtual weapon* clone_weapon()
	{
		return new broken_slime_shooter(*this);
	}
};
class slime_shooter : public weapon
{
public:
	slime_shooter() : weapon("Slime Shooter", 4, 70, "Slimes attack their enemies by shooting out a small ball of slime at their targets. They may hurt themselves in the process.") {}
	slime_shooter(const slime_shooter& in) : weapon(in.name, in.damage, in.hit_rate, in.description, in.enchantment)
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
	dagger() : weapon("Dagger", 5, 90, "A small iron dagger. Not very effective but better than your fists.") {}
	dagger(const dagger& in) : weapon(in.name, in.damage, in.hit_rate, in.description, in.enchantment)
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
class bow : public weapon
{
public:
	bow() : weapon("Bow", 8, 60, "Long ranged weapon. Hard to aim however. Without much skill a bowman will miss many shots.") {}
	bow(const bow& in) : weapon(in.name, in.damage, in.hit_rate, in.description, in.enchantment)
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
class shortsword : public weapon
{
public:
	shortsword() : weapon("Shortsword", 6, 95, "A small sword."){}
	shortsword(const shortsword& in) : weapon(in.name, in.damage, in.hit_rate, in.description, in.enchantment)
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
	sword() : weapon("Sword", 10, 100, "A typical iron sword with a bronze hilt") {}
	sword(const sword& in) : weapon(in.name, in.damage, in.hit_rate, in.description, in.enchantment)
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
class long_bow : public weapon
{
public:
	long_bow() : weapon("Long Bow", 12, 80, "Long ranged weapon. Hard to aim however. Without much skill a bowman will miss many shots.") {}
	long_bow(const long_bow& in) : weapon(in.name, in.damage, in.hit_rate, in.description, in.enchantment)
	{
		//copy constructor
	}
	virtual object* clone()
	{
		return new long_bow(*this);
	}
	virtual weapon* clone_weapon()
	{
		return new long_bow(*this);
	}
};

class axe : public weapon
{
public:
	axe() : weapon("Axe", 15, 85, "A two-handed axe that needs lots of force to yield.") {}
	axe(const axe& in) : weapon(in.name, in.damage, in.hit_rate, in.description, in.enchantment)
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

class nunchucks : public weapon
{
public:
	nunchucks() : weapon(
		"Nunchucks", 18, 95,
		"Swing em around and hit your enemies hard!") {}
	nunchucks(const nunchucks& in) :
		weapon(in.name, in.damage, in.hit_rate, in.description, in.enchantment)
	{
		//copy constructor
	}
	virtual object* clone()
	{
		return new nunchucks(*this);
	}
	virtual weapon* clone_weapon()
	{
		return new nunchucks(*this);
	}
};

class katana : public weapon
{
public:
	katana() : weapon(
		"Katana", 20, 90,
		"A blade for a seasoned Samarai, sharp as glass. Cuts deep into your enemies with only the slightest movements.") {}
	katana(const katana& in) :
		weapon(in.name, in.damage, in.hit_rate, in.description, in.enchantment)
	{
		//copy constructor
	}
	virtual object* clone()
	{
		return new katana(*this);
	}
	virtual weapon* clone_weapon()
	{
		return new katana(*this);
	}
};
class long_axe : public weapon
{
public:
	long_axe() : weapon("Long Axe", 22, 75, 
		"A two-handed axe that needs lots of force to yield.") {}
	long_axe(const long_axe& in) 
		: weapon(in.name, in.damage, in.hit_rate, in.description, in.enchantment)
	{
		//copy constructor
	}
	virtual object* clone()
	{
		return new long_axe(*this);
	}
	virtual weapon* clone_weapon()
	{
		return new long_axe(*this);
	}
};
class daedric_sword : public weapon
{
public:
	daedric_sword() : weapon("Daedric Sword", 30, 90, "This legendary blade is made from volcanic magma that has been infused with magical dragon bone. Whoever yields this blade can kill their opponents very quickly.") {}
	daedric_sword(const daedric_sword& in) 
	: weapon(in.name, in.damage, in.hit_rate, in.description, in.enchantment)
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
	mjolnir() : weapon("Mjolnir", 35, 90, "The legendary hammer of the Norse god of thunder Thor.") {}
	mjolnir(const mjolnir& in) 
	: weapon(in.name, in.damage, in.hit_rate, in.description, in.enchantment)
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
class excalibur : public weapon
{
public:
	excalibur() : weapon("Excalibur", 40, 100, "The legendary sword in the stone.") {}
	excalibur(const excalibur& in) 
	: weapon(in.name, in.damage, in.hit_rate, in.description, in.enchantment)
	{
		//copy constructor
	}
	virtual object* clone()
	{
		return new excalibur(*this);
	}
	virtual weapon* clone_weapon()
	{
		return new excalibur(*this);
	}
};
class dragon_fire : public weapon
{
public:
	dragon_fire() : weapon("Dragon Fire", 32, 70, "Fire that using the magical power of dragon's can turn anything to dust.") {}
	dragon_fire(const dragon_fire& in) 
		: weapon(in.name, in.damage, in.hit_rate, in.description, in.enchantment)
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

/* Picking weapon section */
weapon* pick_random_common_weapon();
weapon* pick_random_rare_weapon();
weapon* pick_random_legendary_weapon();

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
		rare.push_back(new long_bow());
		rare.push_back(new nunchucks());
		rare.push_back(new katana());
		legendary.push_back(new daedric_sword());
		legendary.push_back(new long_axe());
		legendary.push_back(new mjolnir());
		legendary.push_back(new excalibur());
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