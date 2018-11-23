#ifndef CREATURE_FBOX_HPP
#define CREATURE_FBOX_HPP
#include <Sprite.hpp>
#include <Creature.hpp>
class CreatureFlyingBox : public Creature
{
private:
	static const SpriteType my_initial_type = box;
public:
	CreatureFlyingBox(SDL_Rect* ptr_my_position, int hitbox_margin = 10);
};
#endif // CREATURE_FBOX_HPP
