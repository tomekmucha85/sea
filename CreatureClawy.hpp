#ifndef CREATURE_CLAWY_HPP
#define CREATURE_CLAWY_HPP
#include <Creature.hpp>
class CreatureClawy : public Creature
{
private:
	//###############
	// VARIABLES
	//###############
	static const SpriteType my_initial_type = spr_clawy;
	static const int RADIUS_FOR_PREY_PROXIMITY_CHECKS = 100;
	static const double DEFAULT_CLAWY_VELOCITY;
	static const double DEFAULT_CLAWY_TURBO_VELOCITY;
	//TIMING
    //Timer for measuring how long was the creature remaining in its potential prey proximity.
	TimerStartStop* ptr_timer_for_prey_proximity = nullptr;
	std::vector<std::function<void(CreatureClawy*)>> cyclic_actions_class_specific = {};
	//How much time spent in prey proximity elevates hunger by one step (miliseconds)
	int time_spent_in_prey_proximity_that_elevates_hunger = 3000;
	//Threshold above which character is likely to attack preys
	int hunger_threshold = 3;
	//Time when last attack occured
	int last_attack_time = 0;

	//#################
    // FUNCTIONS
    //#################
	void PerformCyclicActionsClassSpecific();

public:
	//###############
	// VARIABLES
	//###############
	bool was_prey_presence_trigger_activated = false;
	bool was_hunger_warning_activated = false;

	//#################
	// FUNCTIONS
	//#################
	CreatureClawy(Coordinates* ptr_my_center, int hitbox_margin = 30);
	~CreatureClawy();
	double TellDefaultVelocity();
	void SetVelocityDependentAnimation(double previous_velocity, double new_velocity);
	void ManagePreyProximityTrigger();
	void PlayWarningAnimationIfTimeSpentInPreyProximityRisesAboveThreshold(Uint32 threshold_miliseconds);
	//#TODO - uwzglêdniæ prawdopodobieñstwo
	void AutoAttack(double likehood=0.5);
	Uint32 TellWhenCreatureAutoAttackedForTheLastTime();
	void ReactForReceivedConversationalMessage(ConversationalMessage my_message);

	//#################
    // LAMBDAS
    //#################

	std::function<void(CreatureClawy*)> func_warn_if_hunger_rises_above_threshold = [](CreatureClawy* ptr_creature)
	{
		ptr_creature->PlayWarningAnimationIfTimeSpentInPreyProximityRisesAboveThreshold(500);
	};

	std::function<void(CreatureClawy*)> func_manage_prey_proximity_trigger = [](CreatureClawy* ptr_creature)
	{
		ptr_creature->ManagePreyProximityTrigger();
	};

	std::function<void(CreatureClawy*)> func_auto_attack_trigger = [](CreatureClawy* ptr_creature)
	{
		ptr_creature->AutoAttack();
	};
};
#endif // CREATURE_CLAWY_HPP
