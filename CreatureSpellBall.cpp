#include <CreatureSpellBall.hpp>

CreatureSpellBall::CreatureSpellBall(Coordinates* ptr_my_center, int hitbox_margin) :
	CreatureSpell(CreatureSpellBall::my_initial_type, ptr_my_center, hitbox_margin)
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
		else if (ptr_one_sorry_creature->my_type == cre_spell_open_doors)
		{
			ptr_one_sorry_creature->Kill();
		}
	}
}

// #TODO - dodaæ pole animacji w Creature! Oderwaæ animacjê od logiki

bool CreatureSpellBall::Move(double x, double y)
{
	//Overload of Creature::Move causing object to be killed after first collision and damage all object in given radius
	bool was_move_successfull = Creature::Move(x,y);

	//#TODO ucywilizowaæ
	//#TODO - niebezpieczne za³o¿enie z castem
	Sprite* ptr_sprite = static_cast<Sprite*>(visual_components[0]);
	ptr_sprite->VortexAnimation();

	if (!was_move_successfull)
	{
		DealDamageInRadius(40);
		Kill();
	}
	return was_move_successfull;
}
