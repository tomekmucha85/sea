#ifndef CREATURE_WRITING_HPP
#define CREATURE_WRITING_HPP
#include <Creature.hpp>
class CreatureWriting : public Creature
{
private:
	
public:
	CreatureWriting(Coordinates* ptr_my_upper_left_corner, std::string my_text, int hitbox_margin = 10);
	//bool Move(double x, double y);
};
#endif // CREATURE_WRITING_HPP