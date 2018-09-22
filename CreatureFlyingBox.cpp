#include <CreatureFlyingBox.hpp>
CreatureFlyingBox::CreatureFlyingBox(Sprite *ptr_my_sprite, int hitbox_margin) :
	Creature(ptr_my_sprite, hitbox_margin)
{
	printf("Spawned CreatureFlyingBox.\n");
}
//