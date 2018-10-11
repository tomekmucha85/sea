#include <CreatureBlackSmoke.hpp>

CreatureBlackSmoke::CreatureBlackSmoke(SDL_Rect* ptr_my_position, int hitbox_margin, int initial_render_layer) :
	Creature(CreatureBlackSmoke::my_initial_type, ptr_my_position, hitbox_margin, initial_render_layer)
{
	printf("Spawned CreatureBlackSmoke.\n");
}