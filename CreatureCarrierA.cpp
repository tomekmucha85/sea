#include <CreatureCarrierA.hpp>

CreatureCarrierA::CreatureCarrierA(Coordinates* ptr_my_center, int hitbox_margin) :
	Creature(CreatureCarrierA::my_initial_type, ptr_my_center, hitbox_margin)
{
	printf("Spawned CreatureSnailmage.\n");
	my_type = cre_carrier_a;
	//Cre_clawy does not collide with another cre_clawy
	//#TODO - czy nie statyczne?
	//#TODO - przerobiŠ dla snailmage
	AddCreatureTypeToCollisionExceptions(cre_clawy);
	AddCreatureTypeToCollisionExceptions(cre_carrier_a);
	VisualComponent* ptr_main_visual_component = visual_components[0];
	ptr_main_visual_component->SetCurrentAnimation(anim_idle);

}

void CreatureCarrierA::SetVelocity(double my_velocity)
{
	double current_velocity = TellVelocity();
	VisualComponent* ptr_main_visual_component = visual_components[0];
	if (current_velocity == 0 && my_velocity != 0)
	{
		ptr_main_visual_component->SetCurrentAnimation(anim_walk);
	}
	else if (current_velocity != 0 && my_velocity == 0)
	{
		ptr_main_visual_component->SetCurrentAnimation(anim_idle);
	}
	Creature::SetVelocity(my_velocity);
}

