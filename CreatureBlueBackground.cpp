#include <CreatureBlueBackground.hpp>

CreatureBlueBackground::CreatureBlueBackground(Coordinates* ptr_my_center, int hitbox_margin) :
	Creature(CreatureBlueBackground::my_initial_type, ptr_my_center, hitbox_margin)
{
	printf("Spawned CreatureBlueBackground.\n");
	my_type = cre_blue_bground;
}