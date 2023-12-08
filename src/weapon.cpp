#include "weapon.h"

weapon* pick_random_common_weapon()
{
	weapon_list* list = new weapon_list();
	std::vector<weapon*> common_list = list->get_common_list();

	int rand_num = random(0, common_list.size() - 1);
	weapon* template_weapon = common_list[rand_num];
	weapon* chosen_weapon = template_weapon->clone_weapon();//copies the template weapon

	chosen_weapon->add_random_enchantment();


	delete list;
	return chosen_weapon;
}
weapon* pick_random_rare_weapon()
{
	weapon_list* list = new weapon_list();
	std::vector<weapon*> rare_list = list->get_rare_list();

	int rand_num = random(0, rare_list.size() - 1);
	weapon* template_weapon = rare_list[rand_num];
	weapon* chosen_weapon = template_weapon->clone_weapon();//copies the template weapon
	delete list;

	// randomly add enchantment here!
	chosen_weapon->add_random_enchantment();
	return chosen_weapon;
}
weapon* pick_random_legendary_weapon()
{
	weapon_list* list = new weapon_list();
	std::vector<weapon*> legendary_list = list->get_legendary_list();

	int rand_num = random(0, legendary_list.size() - 1);
	weapon* template_weapon = legendary_list[rand_num];
	weapon* chosen_weapon = template_weapon->clone_weapon();//copies the template weapon
	delete list;

	// randomly add enchantment here!
	chosen_weapon->add_random_enchantment();
	return chosen_weapon;
}