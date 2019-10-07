#ifndef CREATURE_CARRIER_A_HPP
#define CREATURE_CARRIER_A_HPP
#include <Creature.hpp>
#include <CreatureNPC.hpp>
class CreatureCarrierA : public CreatureNPC
{
private:
	static const SpriteType my_initial_type = spr_snailmage;
	static const double RADIUS_FOR_MAIN_CHARACTER_PROXIMITY_CHECKS;
	//Contains actions associated to specific Creature Carrier A which will be performed during every game loop / every n game loops.
	std::vector<std::function<void(CreatureCarrierA*)>> cyclic_actions_class_specific = {};
	std::vector<ConversationalMessage> typical_replies_to_greeting = {};

	//TIMING
    //Timer for measuring how long was the creature remaining in hero's proximity.
	TimerStartStop* ptr_timer_for_hero_proximity = nullptr;
	//How long can the creature survive in presence of main hero
	Uint32 allowed_time_to_live_in_presence_of_main_character_miliseconds = 5000;

public:

    //#####################
    //# VARIABLES
    //#####################
	unsigned int default_time_to_live_miliseconds = 120000;
	bool was_time_warning_activated = false;
	bool was_main_character_presence_trigger_activated = false;

	//#####################
    //# FUNCTIONS
    //#####################
	CreatureCarrierA(Coordinates* ptr_my_center, int hitbox_margin = 30);
	~CreatureCarrierA();
	void GenerateTypicalRepliesForGreeting();
	void PerformCyclicActionsClassSpecific();
	void SetVelocityDependentAnimation(double previous_velocity, double new_velocity);
	void PlayWarningAnimationIfTimeToLiveDropsBelowThreshold(Uint32 threshold_miliseconds);
	bool AmIWithinMainCharacterProximityRadius();
	void ManageMainCharacterProximityTrigger();
	void ReactForReceivedConversationalMessage(ConversationalMessage my_message);

	std::function<void(CreatureCarrierA*)> func_warn_if_time_to_live_gets_short = [](CreatureCarrierA* ptr_creature)
	{
		ptr_creature->PlayWarningAnimationIfTimeToLiveDropsBelowThreshold(3000);
	};

	std::function<void(CreatureCarrierA*)> func_manage_main_character_proximity_trigger = [](CreatureCarrierA* ptr_creature)
	{
		ptr_creature->ManageMainCharacterProximityTrigger();
	};
};
#endif // CREATURE_CARRIER_A_HPP