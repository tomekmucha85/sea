#include <CreatureCarrierA.hpp>

CreatureCarrierA::CreatureCarrierA(Coordinates* ptr_my_center, int hitbox_margin) :
	Creature(CreatureCarrierA::my_initial_type, ptr_my_center, hitbox_margin)
{
	printf("Spawned CreatureSnailmage.\n");
	my_type = cre_carrier_a;
	//Cre_clawy does not collide with another cre_clawy
	//#TODO - czy nie statyczne?
	AddCreatureTypeToCollisionExceptions(cre_clawy);
	AddCreatureTypeToCollisionExceptions(cre_carrier_a);
	TellMainVisualComponent()->SetCurrentAnimation(anim_idle);

}

void CreatureCarrierA::SetVelocity(double my_velocity)
{
	double current_velocity = TellVelocity();
	if (current_velocity == 0 && my_velocity != 0)
	{
		TellMainVisualComponent()->SetCurrentAnimation(anim_walk);
	}
	else if (current_velocity != 0 && my_velocity == 0)
	{
		TellMainVisualComponent()->SetCurrentAnimation(anim_idle);
	}
	Creature::SetVelocity(my_velocity);
}

void CreatureCarrierA::PlayAnimationIfTimeToLiveDropsBelowThreshold(int threshold_miliseconds)
{
	VisualComponent* ptr_warning_animation = SpawnSpriteUsingFactory(spr_implosion);
	AddVisualComponent(ptr_warning_animation, "warning");
}