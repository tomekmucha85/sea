#ifndef CREATURE_CLAWY_HPP
#define CREATURE_CLAWY_HPP
#include <Creature.hpp>
class CreatureClawy : public Creature
{
private:
	static const SpriteType my_initial_type = spr_clawy;
	//TIMING
    //Timer for measuring how long was the creature remaining in hero's proximity.
	TimerStartStop* ptr_timer_for_prey_proximity = nullptr;
	//How long can the creature survive in presence of main hero
	//Uint32 allowed_time_to_live_in_presence_of_main_character_miliseconds = 5000;
	std::vector<std::function<void(CreatureClawy*)>> cyclic_actions_class_specific = {};
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
	void SetVelocity(double my_velocity);
	void ManagePreyProximityTrigger();
	void PlayWarningAnimationIfHungerRisesAboveThreshold(Uint32 threshold_miliseconds);

	//#################
    // LAMBDAS
    //#################

	std::function<void(CreatureClawy*)> func_warn_if_hunger_rises_above_threshold = [](CreatureClawy* ptr_creature)
	{
		ptr_creature->PlayWarningAnimationIfHungerRisesAboveThreshold(500);
	};

	std::function<void(CreatureClawy*)> func_manage_prey_proximity_trigger = [](CreatureClawy* ptr_creature)
	{
		ptr_creature->ManagePreyProximityTrigger();
	};


};
#endif // CREATURE_CLAWY_HPP
