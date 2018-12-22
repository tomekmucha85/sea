#include <CreatureSpellBall.hpp>

CreatureSpellBall::CreatureSpellBall(SDL_Rect* ptr_my_position, int hitbox_margin) :
	CreatureSpell(ptr_my_position, hitbox_margin)
{
	//printf("Spawned CreatureSpellBall.\n");
	my_type = cre_spell_ball;
	AddCyclicAction(func_destroy_after_hit);
}
