#include <CreatureWriting.hpp>

CreatureWriting::CreatureWriting(Coordinates* ptr_my_upper_left_corner, 
	std::string my_text, 
	int hitbox_margin) : Creature(my_text, ptr_my_upper_left_corner)
{
	Logger::Log("Will spawn a WritingCreature.");
	my_type = cre_writing;
}