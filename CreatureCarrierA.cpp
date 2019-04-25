#include <CreatureCarrierA.hpp>

const double CreatureCarrierA::RADIUS_FOR_MAIN_CHARACTER_PROXIMITY_CHECKS = 40;

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
	ptr_timer_for_hero_proximity = new TimerStartStop();
	//#TODO - napisaæ funkcjê do dodawania cyclic actions na poziomie klasy
	cyclic_actions_class_specific.push_back(func_warn_if_time_to_live_gets_short);
	SetTimeToLive(default_time_to_live_seconds);
}

CreatureCarrierA::~CreatureCarrierA()
{
	delete ptr_timer_for_hero_proximity;
}

void CreatureCarrierA::PerformCyclicActionsClassSpecific()
{
	for (std::function<void(CreatureCarrierA*)> action : cyclic_actions_class_specific)
	{
		action(this);
	}
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

bool CreatureCarrierA::AmIWithinMainCharacterProximityRadius()
{
	bool result = false;
	if (AmIMainCharacter())
	{
		//Check is pointless if I AM the main character.
		result = false;
	}
	else
	{
		double distance_to_hero = Distance::CalculateDistanceBetweenPoints(TellCenterPoint(), Creature::ptr_current_main_charater->TellCenterPoint());
		if (distance_to_hero <= RADIUS_FOR_MAIN_CHARACTER_PROXIMITY_CHECKS)
		{
			result = true;
		}
		else
		{
			result = false;
		}
	}
	return result;
}

void CreatureCarrierA::MeasureTimeSpentInMainCharacterProximity()
{
	if (AmIWithinMainCharacterProximityRadius())
	{
		if (ptr_timer_for_hero_proximity->IsTicking())
		{
			;
		}
		else
		{
			ptr_timer_for_hero_proximity->ResetTimer();
		}
	}
	else
	{
		if (ptr_timer_for_hero_proximity->IsTicking())
		{
			ptr_timer_for_hero_proximity->Stop();
		}
		else
		{
			;
		}
	}
}

Uint32 CreatureCarrierA::TellTimeSpentInHeroProximity()
{
	return ptr_timer_for_hero_proximity->Read();
}