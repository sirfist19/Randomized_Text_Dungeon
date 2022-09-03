#ifndef object_h
#define object_h

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
	std::string get_description()
	{
		return description;
	}
	virtual int get_amt()
	{
		return amt;
	}
	virtual std::string identify() { return "object"; }
	virtual void display(){ print(name); }
	virtual void display_chest(){ print(name); }
	virtual void decrease_amt(const int& _amt)
	{
		amt -= _amt;
		if (amt < 0)
			amt = 0;
	}
	bool is_same_type_of_object(object* other)
	{
		if ((name == other->get_name()) && (description == other->get_description()))
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
	object(const object& in, int _amt) //copy constructor
	{
		name = in.name;
		description = in.description;
		amt = _amt;
	}
	//init a stack of objs
	object(std::string name, int amt, std::string description) :name(name), description(description), amt(amt)
	{

	}
};
class key : public object
{
public:
	key(int amt, std::string name, std::string description) : object(name, amt, description)
	{

	}
	virtual void display() = 0;
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
class dragon_key : public object
{
public:
	dragon_key() : object("Dragon Key", 1, "A bright golden key with an engraving of a fire breathing dragon on its handle. This is the key to open the Golden Dragon door in the entrance room. Its your way out!")
	{

	}
	virtual void display()
	{
		std::cout << "Dragon Key\n";
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
			if (cur_room_exits[i] == -1)
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
	gold(int amt) : object("Gold", amt, "A pile of shining golden coins worth. The front face has a three-headed dragon and the back has a picture of Emperor Septonius III of the Second Dynasty of Helena.")
	{

	}
	virtual void display()
	{
		std::cout << "Gold x"<<amt<<"\n";
		//std::cout << "\tDescription: ";
		//print(description);
		//print(description);
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

#endif
