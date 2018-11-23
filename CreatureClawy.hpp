#ifndef CREATURE_CLAWY_HPP
#define CREATURE_CLAWY_HPP
#include <Sprite.hpp>
#include <Creature.hpp>
class CreatureClawy : public Creature
{
private:
	static const SpriteType my_initial_type = clawy;
public:
	CreatureClawy(SDL_Rect* ptr_my_position, int hitbox_margin = 10);
};
#endif // CREATURE_CLAWY_HPP
