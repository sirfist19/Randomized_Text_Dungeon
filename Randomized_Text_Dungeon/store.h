#ifndef store_h
#define store_h
#include "object.h"
#include "weapon.h"
#include "armor.h"
#include "potion.h"
#include <map>

class ware
{
private:
	object* item;
	int price;
	int stock; //usual amount sold
public:
	ware(object* item, int price, int stock) :item(item), price(price), stock(stock)
	{

	}
	ware(const ware& in) : price(in.price), stock(in.stock)
	{
		item = in.item->clone();
	}
	virtual ~ware()
	{
		delete item;
	}
	void decrease_stock(int& amt)
	{
		stock -= amt;
	}
	int get_price()
	{
		return price;
	}
	object* get_item()
	{
		return item;
	}
	int get_stock()
	{
		return stock;
	}
	std::string get_name()
	{
		return item->get_name();
	}
};
class store //a singleton (for now)
{
private:
	std::string shopkeeper_name;
	std::string shopkeeper_intro_dialogue;
	std::vector<ware*> inventory;//current items at the store
	std::map<std::string, ware*> catalog; //everything that can possible generate along with it's prices
public:
	store() : shopkeeper_name("Greedy Goblin"), 
		shopkeeper_intro_dialogue("Hello far traveler! Have a look at my wares. Let me know if you want to buy anything.")
	{
		set_catalog();
		set_inventory();
	}
	virtual ~store()
	{
		for (unsigned int i = 0; i < inventory.size(); i++)
		{
			delete inventory[i];
		}
		for (std::map<std::string, ware*>::iterator it = catalog.begin(); it != catalog.end(); it++)
		{
			delete it->second;
		}
	}
	std::vector<object*> buy_items(int& player_gold, int& amt_spent)
	{
		std::vector<object*> bought_items;
		amt_spent = 0;

		if (inventory.empty())
		{
			print("\n" + shopkeeper_name + ": 'You bought up the whole store and now you want to buy more?! You might as well just give me more money.'");
			return bought_items;
		}

		//return a vector of items bought
		print(shopkeeper_name + ": 'Ah so you'd like to buy something then. What can I get for yah?'\n");
		print_no_newline("Select an item to buy. Either type the number of the item or the item's name.");
		
		bool valid = false;
		std::vector<std::string> acceptable_inputs;
		std::string name;
		for (unsigned int i = 0; i < inventory.size(); i++)
		{
			acceptable_inputs.push_back(std::to_string(i + 1));
			name = inventory[i]->get_item()->get_name();
			turn_to_lower_case(name);
			acceptable_inputs.push_back(name);
		}
		
		std::string player_input = input();
		valid = str_input_accepted(player_input, acceptable_inputs, valid);

		if (valid == false)
		{
			printUnderscore();
			print(shopkeeper_name + ": 'Sorry, we don't carry that item. How can I help you?'");
		}
		else //if the input is actually valid, select the item and process the number of items wanted
		{
			printUnderscore();
			ware* cur = get_matching_inventory_ware(player_input);
			int stock = cur->get_stock();
			std::string item_name = cur->get_name();
			print_no_newline(shopkeeper_name + ": 'So you want a " + item_name + ", ey. I have " + std::to_string(stock) + " of those. How many do yah want?'");

			std::string amt_wanted = "0";

			while (!is_number_in_range(amt_wanted, 1, stock))
			{
				amt_wanted = input();
				if (!is_number_in_range(amt_wanted, 1, stock))
				{
					print(shopkeeper_name + ": 'I don't have that many of those. How many do yah actually want?'");
				}
			}
			int amt_to_buy = std::stoi(amt_wanted);
			int total_amt = amt_to_buy * cur->get_price();

			if (player_gold < total_amt)
			{
				printUnderscore();
				print(shopkeeper_name + ": 'I'm afraid you don't have enough gold to buy that.'");//return with no bought items
			}
			else //the player can buy the items
			{
				printUnderscore();
				print("\n" + shopkeeper_name + ": 'That will cost you " + std::to_string(total_amt) + " Gold. Still want em?'");
				print("1. Yes");
				print("2. No");
				bool valid_confirmation = false;
				std::string confirmation;
				while (!valid_confirmation)
				{
					confirmation = input();

					if ((confirmation == "1") || (confirmation == "yes"))
					{
						valid_confirmation = true;
						amt_spent = total_amt;
						object* seller_version = cur->get_item();
						object* player_version = seller_version->clone();
						player_version->set_amt(amt_to_buy);
						bought_items.push_back(player_version);

						//change the seller's version
						int seller_new_stock = stock - amt_to_buy;
						cur->decrease_stock(amt_to_buy);

						if (seller_new_stock <= 0) //delete that item from the inventory if it is at or below 0 stock
						{
							delete_matching_inventory_ware(cur);
						}
					}
					else if ((confirmation == "2") || (confirmation == "no"))
					{
						valid_confirmation = true;
						print("\n" + shopkeeper_name + ": 'Alight, how can I help you then?'");
					}
					else
					{
						invalid_input();
					}
				}
			}
		}

		//print to the screen the result of this function
		if (amt_spent != 0)
		{
			printUnderscore();
			print("You gave the " + shopkeeper_name + " " + std::to_string(amt_spent) + " Gold");
			print("\nThe " + shopkeeper_name + " counts each an every coin to make sure it's the right amount. This " + shopkeeper_name + " really loves his Gold. Once he's sure that you didn't short him he says: 'Thanks for doing business with the " + shopkeeper_name + "!'\n");
			for (unsigned int i = 0; i < bought_items.size(); i++)
			{
				print(std::to_string(bought_items[i]->get_amt()) + " " + bought_items[i]->get_name() + " have been placed in your inventory.");
			}
		}
		return bought_items;
	}
	void talk_with_shopkeeper()
	{
		print("\nGreedy Goblin: 'Hello again! I'm the Greedy Goblin as they call me. I was just a normal goblin that liked to fight all the time, but now I prefer to make an honest living. It gets more gold anyway, so I suppose it's a good life style.'");
		printUnderscore();
	}
	ware* get_matching_inventory_ware(std::string& player_input)
	{
		//return the ware the player requests
		std::string name, num_str;
		for (unsigned int i = 0; i < inventory.size(); i++)
		{
			num_str = std::to_string(i + 1);
			name = inventory[i]->get_name();
			turn_to_lower_case(name);

			if ((player_input == name) || (player_input == num_str))
				return inventory[i];
		}
		return nullptr;
	}
	void delete_matching_inventory_ware(ware* cur_ware)
	{
		//return the ware the player requests
		for (std::vector<ware*>::iterator it = inventory.begin(); it != inventory.end(); it++)
		{
			if (cur_ware == *it)
			{
				//there is a match
				inventory.erase(it);
				delete cur_ware;
				return;
			}
		}
	}
	void display_inventory()
	{
		print("The shop keeper greets you.");
		print("\t" + shopkeeper_name + ": '" + shopkeeper_intro_dialogue + "'\n");
		print("Items:");
		
		if (inventory.size() == 0)
		{
			print("The " + shopkeeper_name + " looks very suprised. 'You just bought literally everything that I have. How did you possibly get that much money!' I would say my business is ruined, but look at how rich I am! Woohoo!'");
		}
		for (unsigned int i = 0; i < inventory.size(); i++)
		{
			for (int i = 0; i < STORE_NUM_DASHES; i++)
			{
				std::cout << "_";
			}
			std::cout << "\n";
			std::string res = std::to_string(i+1) + ". " + inventory[i]->get_item()->get_display_string();
			std::string stock_str = "Stock: " + std::to_string(inventory[i]->get_stock());
			std::string price_str = "Price: " + std::to_string(inventory[i]->get_price()) + " Gold";

			const int stock_start = 60;
			const int price_start = 80;

			std::cout << res;
			int cur_pos = res.size();

			while (cur_pos < stock_start)
			{
				std::cout << " ";
				cur_pos++;
			}
			std::cout << stock_str;
			cur_pos += stock_str.size();

			while (cur_pos < price_start)
			{
				std::cout << " ";
				cur_pos++;
			}
			std::cout << price_str << std::endl;
		}
		for (int i = 0; i < STORE_NUM_DASHES; i++)
		{
			std::cout << "_";
		}
	}
	void display_options()
	{
		//std::cout << "\n";
		print("Options: ");
		print("1. Buy");
		print("2. Talk with the shopkeeper.");
		print("3. Or use normal commands.");
	}
	void set_inventory()
	{
		weapon* common_weapon = pick_random_common_weapon();//from weapon.h
		armor* common_armor = pick_random_common_armor();//from armor.h
		weapon* rare_weapon = pick_random_rare_weapon();//from weapon.h
		armor* rare_armor = pick_random_rare_armor();//from armor.h
		weapon* legendary_weapon = pick_random_legendary_weapon();//from weapon.h
		armor* legendary_armor = pick_random_legendary_armor();//from armor.h
		inventory.push_back(new ware(*catalog[common_armor->get_name()]));
		inventory.push_back(new ware(*catalog[rare_armor->get_name()]));
		inventory.push_back(new ware(*catalog[legendary_armor->get_name()]));
		inventory.push_back(new ware(*catalog[common_weapon->get_name()]));
		inventory.push_back(new ware(*catalog[rare_weapon->get_name()]));
		inventory.push_back(new ware(*catalog[legendary_weapon->get_name()]));
		inventory.push_back(new ware(*catalog[lesser_healing_potion().get_name()]));
		inventory.push_back(new ware(*catalog[common_healing_potion().get_name()]));
		inventory.push_back(new ware(*catalog[greater_healing_potion().get_name()]));
	}
	void set_catalog()
	{
		//weapons
		catalog[slime_shooter().get_name()] = new ware(new slime_shooter(), 10, 3);
		catalog[dagger().get_name()] = new ware(new dagger(), 25, 5);
		catalog[bow().get_name()] = new ware(new bow(), 27, 20);
		catalog[shortsword().get_name()] = new ware(new shortsword(), 30, 10);
		catalog[sword().get_name()] = new ware(new sword(), 40, 6);
		catalog[long_bow().get_name()] = new ware(new long_bow(), 60, 4);
		catalog[axe().get_name()] = new ware(new axe(), 80, 10);
		catalog[long_axe().get_name()] = new ware(new long_axe(), 150, 10);
		catalog[daedric_sword().get_name()] = new ware(new daedric_sword(), 200, 3);
		catalog[mjolnir().get_name()] = new ware(new mjolnir(), 300, 1);
		catalog[excalibur().get_name()] = new ware(new excalibur(), 500, 1);

		//potions
		catalog[lesser_healing_potion().get_name()] = new ware(new lesser_healing_potion(), 15, 15);
		catalog[common_healing_potion().get_name()] = new ware(new common_healing_potion(), 35, 10);
		catalog[greater_healing_potion().get_name()] = new ware(new greater_healing_potion(), 70, 5);

		//armor
		catalog[chainmail_helmet().get_name()] = new ware(new chainmail_helmet(), 18, 5);
		catalog[chainmail_chestplate().get_name()] = new ware(new chainmail_chestplate(), 20, 5);
		catalog[chainmail_boots().get_name()] = new ware(new chainmail_boots(), 15, 5);
		catalog[bronze_helmet().get_name()] = new ware(new bronze_helmet(), 35, 5);
		catalog[bronze_chestplate().get_name()] = new ware(new bronze_chestplate(), 40, 5);
		catalog[bronze_boots().get_name()] = new ware(new bronze_boots(), 32, 5);
		catalog[silver_helmet().get_name()] = new ware(new silver_helmet(), 65, 2);
		catalog[silver_chestplate().get_name()] = new ware(new silver_chestplate(), 70, 2);
		catalog[silver_boots().get_name()] = new ware(new silver_boots(), 60, 2);
		catalog[dragon_scale_helmet().get_name()] = new ware(new dragon_scale_helmet(), 250, 1);
		catalog[dragon_scale_chestplate().get_name()] = new ware(new dragon_scale_chestplate(), 300, 1);
		catalog[dragon_scale_boots().get_name()] = new ware(new dragon_scale_boots(), 200, 1);
	}
};

#endif
