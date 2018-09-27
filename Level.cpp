#include <Level.hpp>
#include <Creature.hpp>

void Level::RenderAllPresentCreatures()
{
	for (Creature* ptr_creature : Creature::class_instances)
	{
		ptr_creature->ptr_creature_sprite->Render();
	}

}
