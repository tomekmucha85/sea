#include <Creature.hpp>

void Behavior::WhatToDo(Creature* ptr_my_creature)
{
	//Main character should not have any AI behavior.
	if (ptr_my_creature != Creature::ptr_current_main_charater)
	{
		if (mode == beh_idle)
		{
			if (ptr_my_creature->my_type == cre_spell_ball)
			{
				ptr_my_creature->ptr_creature_sprite->VortexAnimation();
			}
		}
		else if (mode == beh_chase_hero)
		{
			ptr_my_creature->Move(-1,0);
		}
	}
}

void Behavior::SetMode(BehaviorMode mode_to_be_set)
{
	mode = mode_to_be_set;
}
