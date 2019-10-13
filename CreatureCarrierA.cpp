#include <CreatureCarrierA.hpp>

const double CreatureCarrierA::RADIUS_FOR_MAIN_CHARACTER_PROXIMITY_CHECKS = 80;

CreatureCarrierA::CreatureCarrierA(Coordinates* ptr_my_center, int hitbox_margin) :
	CreatureNPC(CreatureCarrierA::my_initial_type, ptr_my_center, hitbox_margin)
{
	printf("Spawned CreatureSnailmage.\n");
	my_type = cre_carrier_a;
	//Cre_clawy does not collide with another cre_clawy
	//#TODO - czy nie statyczne?
	AddCreatureTypeToCollisionExceptions(cre_clawy);
	AddCreatureTypeToCollisionExceptions(cre_carrier_a);
	TellMainVisualComponent()->SetCurrentAnimation(anim_idle);
	//ptr_timer_for_hero_proximity = new TimerStartStop();
	//#TODO - napisa� funkcj� do dodawania cyclic actions na poziomie klasy
	//cyclic_actions_class_specific.push_back(func_warn_if_time_to_live_gets_short);
	//cyclic_actions_class_specific.push_back(func_manage_main_character_proximity_trigger);
	SetTimeToLive(default_time_to_live_miliseconds);
	GenerateTypicalRepliesForGreeting();
}

CreatureCarrierA::~CreatureCarrierA()
{
	delete ptr_timer_for_hero_proximity;
}

void CreatureCarrierA::GenerateTypicalRepliesForGreeting()
{
	std::string creature_name = "snail";

	std::string calm_down_words = "calm down";
	std::vector<std::function<void(Creature*)>> calm_down_coroutines = { func_calm_main_character_down };
	ConversationalMessage calm_down = ConstructConversationalMessage(calm_down_words, 
		creature_name,
		calm_down_coroutines);
	typical_replies_to_greeting.push_back(calm_down);

	std::string aggravate_words = "bite me!";
	std::vector<std::function<void(Creature*)>> aggravate_coroutines = { func_aggravate_main_character };
	ConversationalMessage aggravate = ConstructConversationalMessage(aggravate_words,
		creature_name,
		aggravate_coroutines);
	typical_replies_to_greeting.push_back(aggravate);
}

void CreatureCarrierA::PerformCyclicActionsClassSpecific()
{
	for (std::function<void(CreatureCarrierA*)> action : cyclic_actions_class_specific)
	{
		//printf("Cyclic action class specific for %p.\n", this);
		action(this);
	}
}

void CreatureCarrierA::SetVelocityDependentAnimation(double previous_velocity, double new_velocity)
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

void CreatureCarrierA::PlayWarningAnimationIfTimeToLiveDropsBelowThreshold(Uint32 threshold_miliseconds)
{
	Uint32 time_to_live = HowMuchTimeLeftForMe();
	std::string name_of_warning_visual_component = "warning_time_threshold";
	if (time_to_live <= threshold_miliseconds)
	{
		//printf("Warn! - %d ms left while threshold is %d\n", time_to_live, threshold_miliseconds);
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
		//printf("No worry - %d miliseconds left while threshold is %d.\n", time_to_live, threshold_miliseconds);
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
			//printf("%p is close to main character: %f!\n", this, distance_to_hero);
			result = true;
		}
		else
		{
			result = false;
		}
	}
	return result;
}

void CreatureCarrierA::ManageMainCharacterProximityTrigger()
{
	if (was_main_character_presence_trigger_activated == true)
	{
		if (AmIWithinMainCharacterProximityRadius() == false)
		{
			SetTimeToLive(default_time_to_live_miliseconds);
			was_main_character_presence_trigger_activated = false;
		}
	}
	else
	{
		if (AmIWithinMainCharacterProximityRadius() == true)
		{
			SetTimeToLive(allowed_time_to_live_in_presence_of_main_character_miliseconds);
			was_main_character_presence_trigger_activated = true;
		}
	}
}

void CreatureCarrierA::ReactForReceivedConversationalMessage(ConversationalMessage my_message)
{
	Logger::Log("Received message: " + my_message.message_text + "\n");
	if (my_message.message_text == "hi!")
	{
		//RANDOM NUMBER IN RANGE:
		unsigned int random_reply_index = (rand() % (unsigned int)(typical_replies_to_greeting.size()));
		ConversationalMessage reply = typical_replies_to_greeting[random_reply_index];
		SendConversationalMessage(my_message.ptr_sender, reply);
	}
}
