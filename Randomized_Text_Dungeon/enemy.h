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
	Enemy(std::string name, weapon* main_weapon, int health, enemy_tier tier) : Alive_Entity(name, main_weapon, health), tier(tier)//for basic enemy
	{

	}
	
	virtual void f() = 0;
	virtual void display_attack_info() {
		std::cout << "Enemy: " << name << "\n";
		health->display_health_bar();
		std::cout << "Weapon: " << main_weapon->get_name() << "\n\n";
	}
};

//easy
class Rat : public Enemy
{
public:
	Rat() : Enemy("Rat", new claws(), random(1, 4), enemy_tier::easy) {}
	virtual void f() {};
};
class Slime : public Enemy
{
public:
	Slime() : Enemy("Slime", new slime_shooter(), random(3, 5), enemy_tier::easy) {}
	virtual void f() {};
};


//mid
class Skeleton : public Enemy
{
public:
	Skeleton() : Enemy("Skeleton", new bow(), random(6,10), enemy_tier::medium){}
	virtual void f() {};
};
class Goblin : public Enemy
{
public:
	Goblin() : Enemy("Goblin", new shortsword(), random(10, 12), enemy_tier::medium) {}
	virtual void f() {};
};

//hard
class Orc : public Enemy
{
public:
	Orc() : Enemy("Orc", new axe(), random(15,20), enemy_tier::hard){}
	virtual void f() {};
};

//very hard
class Phantom_Knight : public Enemy
{
public:
	Phantom_Knight() : Enemy("Phantom Knight", new daedric_sword(), random(30, 40), enemy_tier::very_hard) {}
	virtual void f() {};
};
class Dragon : public Enemy
{
public:
	Dragon() : Enemy("Red Dragon", new dragon_fire(), random(50, 65), enemy_tier::very_hard) {} 
	virtual void f() {};
};

#endif

