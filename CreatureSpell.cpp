#include <CreatureSpell.hpp>

CreatureSpell::CreatureSpell(SpriteType my_initial_type, Coordinates* ptr_my_center, int hitbox_margin) :
	Creature(my_initial_type, ptr_my_center, hitbox_margin)
{
	//printf("Spawned CreatureSpell.\n");
	my_type = cre_spell;
}

