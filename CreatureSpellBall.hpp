#ifndef CREATURE_SPELL_BALL_HPP
#define CREATURE_SPELL_BALL_HPP
#include <CreatureSpell.hpp>
class CreatureSpellBall : public CreatureSpell
{
private:
	static const SpriteType my_initial_type = vortex;
public:
	CreatureSpellBall(Coordinates* ptr_my_center, int hitbox_margin = 10);
	void DealDamageInRadius(int radius);
	bool Move(int x, int y);
};
#endif // CREATURE_SPELL_BALL_HPP