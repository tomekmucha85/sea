#include <CreatureBlackSmoke.hpp>

CreatureBlackSmoke::CreatureBlackSmoke(Coordinates* ptr_my_center, int hitbox_margin) :
	Creature(CreatureBlackSmoke::my_initial_type, ptr_my_center, hitbox_margin)
{
	printf("Spawned CreatureBlackSmoke.\n");
	my_type = cre_black_smoke;
}