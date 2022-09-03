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
	std::string identify() { return "object"; }
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
	gold* combine_gold_piles(gold* a, gold* b)
	{
		a->amt = a->amt + b->amt;
		delete b;
		return a;
	}
};

#endif
