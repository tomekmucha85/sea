#ifndef FACTORY_SPAWNING_CREATURES_HPP
#define FACTORY_SPAWNING_CREATURES_HPP
#include <SDL.h>
#include <Creature.hpp>
#include <CreatureFlyingBox.hpp>
#include <CreatureClawy.hpp>
#include <CreatureBlackSmoke.hpp>
#include <CreatureEventTrigger.hpp>

class FactorySpawningCreatures
{
    private:

    public:
		Creature* SpawnCreature(CreatureType desired_type, SDL_Rect* ptr_position, 
			TriggeredEvent my_event = nullptr);
};

#endif //FACTORY_SPAWNING_CREATURES_HPP
