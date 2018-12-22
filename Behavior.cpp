#include <Creature.hpp>

void Behavior::WhatToDo(Creature* ptr_my_creature)
{
	//Main character should not have any AI behavior.
	if (ptr_my_creature != Creature::ptr_current_main_charater)
	{
		if (mode == beh_idle)
		{
			;
		}
		else if (mode == beh_chase_hero)
		{
			ptr_my_creature->Move(0,-1);
		}
	}
}

void Behavior::SetMode(BehaviorMode mode_to_be_set)
{
	mode = mode_to_be_set;
}
