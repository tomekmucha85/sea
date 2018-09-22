#include <Sprite.hpp>
#include <Creature.hpp>
#include <Game.hpp>
#ifndef CREATURE_FBOX_HPP
#define CREATURE_FBOX_HPP
class CreatureFlyingBox : public Creature
{
private:
	Sprite * ptr_my_sprite = NULL;

public:
	CreatureFlyingBox(Sprite *ptr_my_sprite, int hitbox_margin = 10);
};
#endif // CREATURE_FBOX_HPP
