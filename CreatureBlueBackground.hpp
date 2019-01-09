#ifndef CREATURE_BLUE_BGROUND_HPP
#define CREATURE_BLUE_BGROUND_HPP
#include <Creature.hpp>
class CreatureBlueBackground : public Creature
{
private:
	static const SpriteType my_initial_type = background;
public:
	CreatureBlueBackground(Coordinates* ptr_my_center, int hitbox_margin = 10, int initial_render_layer = -1);
};
#endif // CREATURE_BLUE_BGROUND_HPP
