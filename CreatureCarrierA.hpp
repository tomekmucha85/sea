#ifndef CREATURE_CARRIER_A_HPP
#define CREATURE_CARRIER_A_HPP
#include <Creature.hpp>
class CreatureCarrierA : public Creature
{
private:
	static const SpriteType my_initial_type = spr_snailmage;
public:
	CreatureCarrierA(Coordinates* ptr_my_center, int hitbox_margin = 30);
	void SetVelocity(double my_velocity);
};
#endif // CREATURE_CARRIER_A_HPP