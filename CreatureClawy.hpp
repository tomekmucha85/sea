#ifndef CREATURE_CLAWY_HPP
#define CREATURE_CLAWY_HPP
#include <Creature.hpp>
class CreatureClawy : public Creature
{
private:
	static const SpriteType my_initial_type = clawy;
	static const int default_velocity = 8;
public:
	CreatureClawy(Coordinates* ptr_my_center, int hitbox_margin = 10);
};
#endif // CREATURE_CLAWY_HPP
