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
	//init one obj
	object(std::string name, std::string description):name(name), description(description), amt(1)
	{
		
	}
	//init a stack of objs
	object(std::string name, int amt, std::string description) :name(name), description(description), amt(amt)
	{

	}
	virtual void display() = 0;
	virtual void display_chest() = 0;
	virtual std::string identify() = 0;
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
		std::cout << "Gold x" << amt << "\n";
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
