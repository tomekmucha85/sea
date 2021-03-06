#include <CreatureClawy.hpp>

const double CreatureClawy::DEFAULT_CLAWY_VELOCITY = 150;
const double CreatureClawy::DEFAULT_CLAWY_TURBO_VELOCITY = 300;

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
	cyclic_actions_class_specific.push_back(func_auto_attack_trigger);
}

CreatureClawy::~CreatureClawy()
{
	delete ptr_timer_for_prey_proximity;
}

double CreatureClawy::TellDefaultVelocity()
{
	return DEFAULT_CLAWY_VELOCITY;
}

void CreatureClawy::SetVelocityDependentAnimation(double previous_velocity, double new_velocity)
{
	if (previous_velocity == 0 && new_velocity != 0)
	{
		TellMainVisualComponent()->SetCurrentAnimation(anim_walk);
	}
	else if (previous_velocity != 0 && new_velocity == 0)
	{
		TellMainVisualComponent()->SetCurrentAnimation(anim_idle);
	}
}

void CreatureClawy::PerformCyclicActionsClassSpecific()
{
	for (std::function<void(CreatureClawy*)> action : cyclic_actions_class_specific)
	{
		//printf("Cyclic action class specific for %p.\n", this);
		action(this);
	}
}

void CreatureClawy::PlayWarningAnimationIfTimeSpentInPreyProximityRisesAboveThreshold(Uint32 threshold_miliseconds)
{
	std::string name_of_warning_visual_component = "warning_time_threshold";
	if (ptr_timer_for_prey_proximity->Read() >= threshold_miliseconds)
	{
		Logger::Log("Prey proximity trigger exceeded threshold! (" + 
			std::to_string(ptr_timer_for_prey_proximity->Read()) +
		    " )", debug_full);
		if (was_hunger_warning_activated == false)
		{
			Logger::Log("Prey proximity warning activated!");
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
	//#TODO - do rozbicia na funkcj� od timera i g�odu
	static Uint32 last_proximity_timer_indication = 0;
	if (was_prey_presence_trigger_activated)
	{
		if (AmIWithinProximityRadiusOfCertainTypeCreature(cre_carrier_a, RADIUS_FOR_PREY_PROXIMITY_CHECKS) == false)
		{
			//Cleanup when leaving prey proximity
			was_prey_presence_trigger_activated = false;
			ptr_timer_for_prey_proximity->Reset();
			SetHungerLevel(0);
			last_proximity_timer_indication = 0;
		}
		else
		{
			//Building up hunger in prey proximity
			if (last_proximity_timer_indication == 0)
			{
				last_proximity_timer_indication = ptr_timer_for_prey_proximity->Read();
			}
			else
			{
				//Check if next quant is reached
				Uint32 current_proximity_timer_indication = ptr_timer_for_prey_proximity->Read();
				Uint32 current_quant_reached = current_proximity_timer_indication / time_spent_in_prey_proximity_that_elevates_hunger;
				Uint32 previous_quant_reached = last_proximity_timer_indication / time_spent_in_prey_proximity_that_elevates_hunger;
				if (current_quant_reached - previous_quant_reached >= 1)
				{
					ChangeHungerLevel(1);
				}
				last_proximity_timer_indication = current_proximity_timer_indication;
			}
		}
	}
	else
	//If prey proximity trigger is not activated.
	{
		if (AmIWithinProximityRadiusOfCertainTypeCreature(cre_carrier_a, RADIUS_FOR_PREY_PROXIMITY_CHECKS) == true)
		{
				was_prey_presence_trigger_activated = true;
				ptr_timer_for_prey_proximity->Start();
		}
	}
}

void CreatureClawy::AutoAttack(double likehood)
{
	//#TODO - to bardziej nale�y do Behavioru
	//Triggering automatic attack if hunger threshold if exceeded and prey is near.
	if (TellHungerLevel() >= hunger_threshold)
	{
		Logger::Log("Hunger level " +
            std::to_string(TellHungerLevel()) +
            " above threshold of " +
            std::to_string(hunger_threshold));
		//Timer ticks if prey is near.
		if (ptr_timer_for_prey_proximity->IsTicking())
		{
			Attack(attack_melee);
			last_attack_time = SDL_GetTicks();
			//#TODO - powinno si� to odbywa� w ramach innej funkcji.
			SetHungerLevel(0);
		}
	}
}

Uint32 CreatureClawy::TellWhenCreatureAutoAttackedForTheLastTime()
{
	return last_attack_time;
}

void CreatureClawy::ReactForReceivedConversationalMessage(ConversationalMessage my_message)
{
	Logger::Log("Received message: " + my_message.message_text + "\n");
}