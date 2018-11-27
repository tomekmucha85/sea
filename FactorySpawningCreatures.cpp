#include <FactorySpawningCreatures.hpp>

Creature* FactorySpawningCreatures::SpawnCreature(CreatureType desired_type, SDL_Rect* ptr_position)
{
	Creature* result = NULL;

	if (desired_type == CreatureType::cre_clawy)
	{
		printf("Requested hero creature.\n");
		result = new CreatureClawy(ptr_position);
	}
	else if (desired_type == CreatureType::cre_flying_box)
	{
		printf("Requested flying box creature. \n");
		result = new CreatureFlyingBox(ptr_position);
	}
	else if (desired_type == CreatureType::cre_black_smoke)
	{
		printf("Requested black smoke creature.\n");
		result = new CreatureBlackSmoke(ptr_position);
	}
	else if (desired_type == CreatureType::cre_none)
	{
		throw std::invalid_argument("Trying to spawn None creature! This should not happen.\n");
	}
	else
	{
		printf("Requested some other creature.\n");
	}

	return result;
}