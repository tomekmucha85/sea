#ifndef CREATURE_NPC_HPP
#define CREATURE_NPC_HPP
#include <Creature.hpp>
class CreatureNPC : public Creature
{
private:
	static const SpriteType my_initial_type = spr_clawy;
public:
	CreatureNPC(Coordinates* ptr_my_center, int hitbox_margin = 10);
};
#endif // CREATURE_NPC_HPP