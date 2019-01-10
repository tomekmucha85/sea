#ifndef FACTORY_SPAWNING_CREATURES_HPP
#define FACTORY_SPAWNING_CREATURES_HPP
#include <SDL.h>
#include <Creature.hpp>
#include <CreatureFlyingBox.hpp>
#include <CreatureClawy.hpp>
#include <CreatureBlackSmoke.hpp>
#include <CreatureBlueBackground.hpp>
#include <CreatureEventTrigger.hpp>
#include <CreatureVectorMask.hpp>
#include <CreatureNPC.hpp>
#include <CreatureSpell.hpp>
#include <CreatureSpellBall.hpp>
#include <CreatureSpellOpenDoors.hpp>

class FactorySpawningCreatures
{
    private:

    public:
		Creature* SpawnCreature(CreatureType desired_type, PreciseRect* ptr_position,
			std::string my_trigger_signal = nullptr);
		Creature* SpawnCreature(CreatureType desired_type, Coordinates* ptr_my_center, int render_layer = 0);
};

#endif //FACTORY_SPAWNING_CREATURES_HPP
