#ifndef CREATURE_BLACKSMOKE_HPP
#define CREATURE_BLACKSMOKE_HPP
#include <Creature.hpp>
class CreatureBlackSmoke : public Creature
{
private:
	static const SpriteType my_initial_type = spr_black_smoke;
public:
	CreatureBlackSmoke(Coordinates* ptr_my_center, int hitbox_margin = 10);
};
#endif // CREATURE_BLACKSMOKE_HPP
