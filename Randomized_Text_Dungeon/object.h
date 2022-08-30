#ifndef object_h
#define object_h

class object
{
protected:
	std::string name;
	std::string description;
public:
	std::string get_name()
	{
		return name;
	}
	std::string get_description()
	{
		return description;
	}
	object(std::string name, std::string description):name(name), description(description)
	{
		
	}
	virtual void display() = 0;
};

class gold : public object
{
protected:
	int amt; //the amount of gold pieces
public:
	gold(int amt) : amt(amt), object("Gold", "A pile of shining golden coins worth. The front face has a three-headed dragon and the back has a picture of Emperor Septonius III of the Second Dynasty of Helena.")
	{

	}
	virtual void display()
	{
		std::cout << "Gold x"<<amt<<"\n";
		//std::cout << "\tDescription: ";
		//print(description);
	}
	gold* combine_gold_piles(gold* a, gold* b)
	{
		a->amt = a->amt + b->amt;
		delete b;
		return a;
	}
};


#endif
