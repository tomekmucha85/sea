#include <CreatureSpell.hpp>

CreatureSpell::CreatureSpell(SDL_Rect* ptr_my_position, int hitbox_margin) :
	Creature(CreatureSpell::my_initial_type, ptr_my_position, hitbox_margin)
{
	//printf("Spawned CreatureSpell.\n");
	my_type = cre_spell;
}
