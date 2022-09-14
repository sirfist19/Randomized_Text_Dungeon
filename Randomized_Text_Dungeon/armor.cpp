#include "armor.h"

armor* pick_random_common_armor()
{
	armor_list* list = new armor_list();
	std::vector<armor*> common_list = list->get_common_list();

	int rand_num = random(0, common_list.size() - 1);
	armor* template_armor = common_list[rand_num];
	armor* chosen_armor = template_armor->clone_armor();//copies the template weapon
	delete list;
	return chosen_armor;
}
armor* pick_random_rare_armor()
{
	armor_list* list = new armor_list();
	std::vector<armor*> rare_list = list->get_rare_list();

	int rand_num = random(0, rare_list.size() - 1);
	armor* template_armor = rare_list[rand_num];
	armor* chosen_armor = template_armor->clone_armor();//copies the template weapon
	delete list;
	return chosen_armor;
}
armor* pick_random_legendary_armor()
{
	armor_list* list = new armor_list();
	std::vector<armor*> legendary_list = list->get_legendary_list();

	int rand_num = random(0, legendary_list.size() - 1);
	armor* template_armor = legendary_list[rand_num];
	armor* chosen_armor = template_armor->clone_armor();//copies the template weapon
	delete list;
	return chosen_armor;
}