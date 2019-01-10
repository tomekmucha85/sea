#ifndef CREATURE_SPELL_OPEN_DOORS_HPP
#define CREATURE_SPELL_OPEN_DOORS_HPP
#include <CreatureSpell.hpp>
class CreatureSpellOpenDoors : public CreatureSpell
{
private:
	static const SpriteType my_initial_type = spr_exit;
public:
	CreatureSpellOpenDoors(Coordinates* ptr_my_center, int hitbox_margin = 10);
};
#endif // CREATURE_SPELL_OPEN_DOORS_HPP