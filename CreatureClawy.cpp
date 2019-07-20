#include <CreatureClawy.hpp>

CreatureClawy::CreatureClawy(Coordinates* ptr_my_center, int hitbox_margin) :
	Creature(CreatureClawy::my_initial_type, ptr_my_center, hitbox_margin)
{
	printf("Spawned CreatureClawy.\n");
	my_type = cre_clawy;
	//Cre_clawy does not collide with another cre_clawy
	//#TODO - czy nie statyczne?
	AddCreatureTypeToCollisionExceptions(cre_clawy);
	AddCreatureTypeToCollisionExceptions(cre_carrier_a);
	TellMainVisualComponent()->SetCurrentAnimation(anim_idle);
	ptr_timer_for_prey_proximity = new TimerStartStop();
	//#TODO - napisa� funkcj� do dodawania cyclic actions na poziomie klasy
	cyclic_actions_class_specific.push_back(func_warn_if_hunger_rises_above_threshold);
	cyclic_actions_class_specific.push_back(func_manage_prey_proximity_trigger);

}

CreatureClawy::~CreatureClawy()
{
	delete ptr_timer_for_prey_proximity;
}

void CreatureClawy::SetVelocity(double my_velocity)
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

void CreatureClawy::PerformCyclicActionsClassSpecific()
{
	for (std::function<void(CreatureClawy*)> action : cyclic_actions_class_specific)
	{
		//printf("Cyclic action class specific for %p.\n", this);
		action(this);
	}
}

void CreatureClawy::PlayWarningAnimationIfHungerRisesAboveThreshold(Uint32 threshold_miliseconds)
{
	std::string name_of_warning_visual_component = "warning_time_threshold";
	if (ptr_timer_for_prey_proximity->Read() >= threshold_miliseconds)
	{
		Logger::Log("Prey proximity trigger exceeded threshold! (" + 
			std::to_string(ptr_timer_for_prey_proximity->Read()) +
		    " )");
		if (was_hunger_warning_activated == false)
		{
			Logger::Log("Huger warning activated!");
			VisualComponent* ptr_warning_animation = SpawnSpriteUsingFactory(spr_implosion);
			AddVisualComponent(ptr_warning_animation, name_of_warning_visual_component);
			visual_components[name_of_warning_visual_component]->SetCurrentAnimation(anim_idle);
			was_hunger_warning_activated = true;
		}
	}
	else
	{
		if (was_hunger_warning_activated == true)
		{
			Logger::Log("Hunger warning revoked!");
			RemoveVisualComponent(name_of_warning_visual_component);
			was_hunger_warning_activated = false;
		}
	}
}

void CreatureClawy::ManagePreyProximityTrigger()
{
	if (was_prey_presence_trigger_activated)
	{
		if (AmIWithinProximityRadiusOfCertainTypeCreature(cre_carrier_a, 300) == false)
		{
			was_prey_presence_trigger_activated = false;
			ptr_timer_for_prey_proximity->StopAndErase();
		}
	}
	else
	{
		if (AmIWithinProximityRadiusOfCertainTypeCreature(cre_carrier_a, 300) == true)
		{
				was_prey_presence_trigger_activated = true;
				ptr_timer_for_prey_proximity->Start();
		}
	}
}