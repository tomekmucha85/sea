#include <CreatureSpell.hpp>

CreatureSpell::CreatureSpell(SpriteType my_initial_type,SDL_Rect* ptr_my_position, int hitbox_margin) :
	Creature(my_initial_type, ptr_my_position, hitbox_margin)
{
	//printf("Spawned CreatureSpell.\n");
	my_type = cre_spell;
}
