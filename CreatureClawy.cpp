#include <CreatureClawy.hpp>

CreatureClawy::CreatureClawy(SDL_Rect* ptr_my_position, int hitbox_margin) :
	Creature(CreatureClawy::my_initial_type, ptr_my_position, hitbox_margin)
{
	printf("Spawned CreatureClawy.\n");
	my_type = cre_clawy;
}
