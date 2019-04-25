#ifndef CREATURE_CARRIER_A_HPP
#define CREATURE_CARRIER_A_HPP
#include <Creature.hpp>
class CreatureCarrierA : public Creature
{
private:
	static const SpriteType my_initial_type = spr_snailmage;
	static const double RADIUS_FOR_MAIN_CHARACTER_PROXIMITY_CHECKS;
	//Contains actions associated to specific Creature Carrier A which will be performed during every game loop / every n game loops.
	std::vector<std::function<void(CreatureCarrierA*)>> cyclic_actions_class_specific = {};

	//TIMING
    //Timer for measuring how long was the creature remaining in hero's proximity.
	TimerStartStop* ptr_timer_for_hero_proximity = nullptr;
	//How long can the creature survive in presence of main hero
	Uint32 allowed_time_to_remain_close_to_main_characte_seconds = 15;

public:

    //#####################
    //# VARIABLES
    //#####################

	unsigned int default_time_to_live_seconds = 15;
	bool was_time_warning_activated = false;

	//#####################
    //# FUNCTIONS
    //#####################

	CreatureCarrierA(Coordinates* ptr_my_center, int hitbox_margin = 30);
	~CreatureCarrierA();
	void PerformCyclicActionsClassSpecific();
	void SetVelocity(double my_velocity);
	void PlayWarningAnimationIfTimeToLiveDropsBelowThreshold(Uint32 threshold_miliseconds);
	bool AmIWithinMainCharacterProximityRadius();
	void MeasureTimeSpentInMainCharacterProximity();
	Uint32 TellTimeSpentInHeroProximity();

	std::function<void(CreatureCarrierA*)> func_warn_if_time_to_live_gets_short = [](CreatureCarrierA* ptr_creature)
	{
		ptr_creature->PlayWarningAnimationIfTimeToLiveDropsBelowThreshold(3000);
	};
};
#endif // CREATURE_CARRIER_A_HPP