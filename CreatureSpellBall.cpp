#include <CreatureSpellBall.hpp>

CreatureSpellBall::CreatureSpellBall(SDL_Rect* ptr_my_position, int hitbox_margin) :
	CreatureSpell(CreatureSpellBall::my_initial_type,  ptr_my_position, hitbox_margin)
{
	printf("Spawned CreatureSpellBall.\n");
	my_type = cre_spell_ball;
	AddCyclicAction(func_destroy_hit_object);
}

void CreatureSpellBall::DealDamageInRadius(int radius)
{
	std::vector<Creature*> creatures_to_be_damaged = FindNeighborsInSet(&Creature::current_environment);
	for (Creature* ptr_one_sorry_creature : creatures_to_be_damaged)
	{
		if (ptr_one_sorry_creature->my_type == cre_flying_box)
		{
			ptr_one_sorry_creature->Kill();
		}
	}
}

// #TODO - dodaæ pole animacji w Creature! oderwaæ animacjê od logiki

bool CreatureSpellBall::Move(int x, int y)
{
	//Overload of Creature::Move causing object to be killed after first collision
	bool was_move_successfull = Creature::Move(x,y);
	ptr_creature_sprite->VortexAnimation();
	if (!was_move_successfull)
	{
		DealDamageInRadius(40);
		Kill();
	}
	return was_move_successfull;
}
