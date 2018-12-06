#include <FactorySpawningCreatures.hpp>

Creature* FactorySpawningCreatures::SpawnCreature(CreatureType desired_type, SDL_Rect* ptr_position,
	std::function<void()> my_event)
{
	Creature* result = NULL;

	if (desired_type == cre_clawy)
	{
		//printf("Requested hero creature.\n");
		result = new CreatureClawy(ptr_position);
	}
	else if (desired_type == cre_flying_box)
	{
		//printf("Requested flying box creature. \n");
		result = new CreatureFlyingBox(ptr_position);
	}
	else if (desired_type == cre_black_smoke)
	{
		//printf("Requested black smoke creature.\n");
		result = new CreatureBlackSmoke(ptr_position);
	}
	else if (desired_type == cre_none)
	{
		throw std::invalid_argument("Trying to spawn None creature! This should not happen.\n");
	}
	else if (desired_type == cre_event_trigger)
	{
		printf("Requested an event trigger creature.\n");
		if (my_event == nullptr)
		{
			throw std::invalid_argument("No event to be triggered! Pointer is empty");
		}
		else
		{
			result = new CreatureEventTrigger(ptr_position, my_event);
		}
	}
	else
	{
		printf("Requested some other creature.\n");
	}

	return result;
}