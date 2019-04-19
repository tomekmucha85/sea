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
	AddCyclicAction(func_warn_if_time_to_live_gets_short);
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

void CreatureCarrierA::PlayWarningAnimationIfTimeToLiveDropsBelowThreshold(Uint32 threshold_miliseconds)
{
	Uint32 time_to_live = HowMuchTimeLeftForMe();
	std::string name_of_warning_visual_component = "warning_time_threshold";
	if (time_to_live <= threshold_miliseconds)
	{
		//printf("Warn! - %d ms left\n", time_to_live);
		if (was_time_warning_activated == false)
		{
			//printf("Warning activated!\n");
			VisualComponent* ptr_warning_animation = SpawnSpriteUsingFactory(spr_implosion);
			AddVisualComponent(ptr_warning_animation, name_of_warning_visual_component);
			visual_components[name_of_warning_visual_component]->SetCurrentAnimation(anim_idle);
			was_time_warning_activated = true;
		}
	}
	else
	{
		//In case time to live was increased - get rid of warning animation
		if (was_time_warning_activated == true)
		{
			//printf("Warning revoked! - %d ms left\n", time_to_live);
			RemoveVisualComponent(name_of_warning_visual_component);
			was_time_warning_activated = false;
		}
	}
}