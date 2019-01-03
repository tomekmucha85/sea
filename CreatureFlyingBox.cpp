#include <CreatureFlyingBox.hpp>

CreatureFlyingBox::CreatureFlyingBox(Coordinates* ptr_my_center, int hitbox_margin) :
	Creature(CreatureFlyingBox::my_initial_type, ptr_my_center, hitbox_margin)
{
	//printf("Spawned CreatureFlyingBox.\n");
	my_type = cre_flying_box;
}