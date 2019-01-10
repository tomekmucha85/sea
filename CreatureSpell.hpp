#ifndef CREATURE_SPELL_HPP
#define CREATURE_SPELL_HPP
#include <Creature.hpp>
class CreatureSpell : public Creature
{
private:
	static const SpriteType my_initial_type = spr_black_smoke;
	//Who cast the spell?
	Creature* wizard = nullptr;
public:
	CreatureSpell(SpriteType my_initial_type, Coordinates* ptr_my_center, int hitbox_margin = 10);

	//###################
    //COMMON LAMBDAS
    //###################

	std::function<void(Creature*)> func_destroy_hit_object = [](Creature* ptr_creature)
	{
		//printf("Do I collide?\n");
		std::vector<Creature*> hit_creatures = ptr_creature->WhichNeighborsDoICollideWith();
		if (hit_creatures.size() > 0)
		{
			for (Creature* ptr_hit_creature : hit_creatures)
			{
				printf("Going to destroy %p !\n", ptr_hit_creature);
				ptr_hit_creature->Kill();
			}
		}
	};
};
#endif // CREATURE_SPELL_HPP