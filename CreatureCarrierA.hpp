#ifndef CREATURE_CARRIER_A_HPP
#define CREATURE_CARRIER_A_HPP
#include <Creature.hpp>
class CreatureCarrierA : public Creature
{
private:
	static const SpriteType my_initial_type = spr_snailmage;

public:

    //#####################
    //# VARIABLES
    //#####################

	unsigned int default_time_to_live_seconds = 60;
	bool is_being_followed = false;
	bool was_time_warning_activated = false;

	//#####################
    //# FUNCTIONS
    //#####################

	CreatureCarrierA(Coordinates* ptr_my_center, int hitbox_margin = 30);
	void SetVelocity(double my_velocity);
	void PlayWarningAnimationIfTimeToLiveDropsBelowThreshold(Uint32 threshold_miliseconds);

};
#endif // CREATURE_CARRIER_A_HPP