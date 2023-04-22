#ifndef teleporter_h
#define teleporter_h
#include "object.h"
class room;

class teleporter : public object
{
private:
	room* last_teleporter_pos;
public:
	teleporter() : 
		object("Teleporter", 1, "A strange high-tech device. If you use it you can instantly be taken to key dungeon points you've been to before!"),
		last_teleporter_pos(nullptr)
	{
	
	}
	teleporter(const teleporter& in) : object(in.name, in.amt, in.description)
	{
		//copy constructor
	}
	virtual object* clone()
	{
		return new teleporter(*this);
	}
	virtual void display()
	{
		std::cout << get_display_string();
	}
	virtual std::string get_display_string()
	{
		return ("Teleporter");
	}
	virtual void display_chest()
	{
		display();
		print();
	}
	virtual std::string identify()
	{
		return "teleporter";
	}
	room* get_last_teleported_pos()
	{
		return last_teleporter_pos;
	}
	void set_last_teleported_pos(room* in)
	{
		last_teleporter_pos = in;
	}
};


#endif