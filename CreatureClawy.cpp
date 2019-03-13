#include <CreatureClawy.hpp>

CreatureClawy::CreatureClawy(Coordinates* ptr_my_center, int hitbox_margin) :
	Creature(CreatureClawy::my_initial_type, ptr_my_center, hitbox_margin)
{
	printf("Spawned CreatureClawy.\n");
	my_type = cre_clawy;
	//Cre_clawy does not collide with another cre_clawy
	AddCreatureTypeToCollisionExceptions(cre_clawy);
}
