#ifndef CREATURE_BLUE_BGROUND_HPP
#define CREATURE_BLUE_BGROUND_HPP
#include <Creature.hpp>
class CreatureBlueBackground : public Creature
{
private:
	static const SpriteType my_initial_type = spr_background;
public:
	CreatureBlueBackground(Coordinates* ptr_my_center, int hitbox_margin = 10);
};
#endif // CREATURE_BLUE_BGROUND_HPP
