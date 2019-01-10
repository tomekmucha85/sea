#ifndef CREATURE_FBOX_HPP
#define CREATURE_FBOX_HPP
#include <Creature.hpp>
class CreatureFlyingBox : public Creature
{
private:
	static const SpriteType my_initial_type = spr_box;
public:
	CreatureFlyingBox(Coordinates* ptr_my_center, int hitbox_margin = 10);
};
#endif // CREATURE_FBOX_HPP
