#ifndef CREATURE_BLACKSMOKE_HPP
#define CREATURE_BLACKSMOKE_HPP
#include <Sprite.hpp>
#include <Creature.hpp>
class CreatureBlackSmoke : public Creature
{
private:
	static const SpriteType my_initial_type = black_smoke;
public:
	CreatureBlackSmoke(SDL_Rect* ptr_my_position, int hitbox_margin = 10, int initial_render_layer = -1);
};
#endif // CREATURE_BLACKSMOKE_HPP
