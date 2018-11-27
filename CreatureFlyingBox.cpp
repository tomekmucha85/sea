#include <CreatureFlyingBox.hpp>

CreatureFlyingBox::CreatureFlyingBox(SDL_Rect* ptr_my_position, int hitbox_margin) :
	Creature(CreatureFlyingBox::my_initial_type, ptr_my_position, hitbox_margin)
{
	printf("Spawned CreatureFlyingBox.\n");
	my_type = cre_flying_box;
}