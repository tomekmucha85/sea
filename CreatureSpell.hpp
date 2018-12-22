#ifndef CREATURE_SPELL_HPP
#define CREATURE_SPELL_HPP
#include <Creature.hpp>
class CreatureSpell : public Creature
{
private:
	static const SpriteType my_initial_type = black_smoke;
public:
	CreatureSpell(SDL_Rect* ptr_my_position, int hitbox_margin = 10);

	//###################
    //COMMON LAMBDAS
    //###################

	std::function<void(Creature*)> func_destroy_after_hit = [](Creature* ptr_creature)
	{
		printf("Do I collide?\n");
		if (ptr_creature->DoICollideWithNeighbors())
		{
			printf("I collide!\n");
			ptr_creature->Kill();
		}
	};
};
#endif // CREATURE_SPELL_HPP