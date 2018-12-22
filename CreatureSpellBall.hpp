#ifndef CREATURE_SPELL_BALL_HPP
#define CREATURE_SPELL_BALL_HPP
#include <CreatureSpell.hpp>
class CreatureSpellBall : public CreatureSpell
{
private:
	static const SpriteType my_initial_type = black_smoke;
public:
	CreatureSpellBall(SDL_Rect* ptr_my_position, int hitbox_margin = 10);
};
#endif // CREATURE_SPELL_BALL_HPP