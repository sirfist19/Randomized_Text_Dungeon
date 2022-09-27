#include "object.h"


//general functions at the room level
std::string get_items_string(std::vector<object*>& items, std::string before, std::string after)
{
	std::string res = before;
	for (unsigned int i = 0; i < items.size(); i++)
	{
		std::string name = items[i]->get_name();
		std::string identifier = items[i]->identify();
		int amt = items[i]->get_amt();

		std::string before_string = "";
		std::string after_string = "";

		if ((i == items.size() - 1) && (i != 0))
			before_string += "and ";

		if (amt > 1) //many items
		{
			if (identifier == "boots")
			{
				before_string += (std::to_string(amt) + " pairs of ");
			}
			else if (identifier == "gold")
			{
				before_string += (std::to_string(amt) + " ");
				//no plural
			}
			else
			{
				before_string += (std::to_string(amt) + " ");
				after_string += "s";
			}
		}
		else //only one item
		{
			if (identifier == "boots")
			{
				before_string += "a pair of ";
			}
			else if (identifier == "gold")
			{
				before_string += "1 ";
			}
			else
			{
				switch (name[0])
				{
				case 'a':
				case 'e':
				case 'i':
				case 'o':
				case 'u':
					before_string += "an ";
				default:
					before_string += "a ";
				}
			}
		}

		if ((items.size() != 1) && (i != items.size() - 1))
			after_string += ", ";

		res += (before_string + name + after_string);
	}
	res += after;//add on the after string argument made in the previous function that called this one
	return res;
}
void print_items(std::vector<object*>& items, std::string before, std::string after)
{
	std::string res = get_items_string(items, before, after);
	//actually print the items
	print_no_newline(res);
}
void print_item(object* item, std::string before, std::string after) //a wrapper for print_items for one item
{
	std::vector<object*> items;
	if (item != nullptr)
		items.push_back(item);
	print_items(items, before, after);
}