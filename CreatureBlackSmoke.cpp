#include <CreatureBlackSmoke.hpp>

CreatureBlackSmoke::CreatureBlackSmoke(Coordinates* ptr_my_center, int hitbox_margin, int initial_render_layer) :
	Creature(CreatureBlackSmoke::my_initial_type, ptr_my_center, hitbox_margin, initial_render_layer)
{
	printf("Spawned CreatureBlackSmoke.\n");
	my_type = cre_black_smoke;
}