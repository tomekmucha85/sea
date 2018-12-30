#include <FactorySpawningCreatures.hpp>

Creature* FactorySpawningCreatures::SpawnCreature(CreatureType desired_type, CenterCoordinates* ptr_my_center,
	std::string my_trigger_signal)
{
	Creature* result = nullptr;
	if (desired_type == cre_spell_ball)
	{
	    result = new CreatureSpellBall(ptr_my_center);
	}
		else
	{
	printf("Requested some other creature.\n");
	}

	return result;

}



Creature* FactorySpawningCreatures::SpawnCreature(CreatureType desired_type, SDL_Rect* ptr_position,
	std::string my_trigger_signal)
{
	Creature* result = nullptr;

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
		if (my_trigger_signal.empty())
		{
			throw std::invalid_argument("No trigger signal! String is empty.\n");
		}
		else
		{
			result = new CreatureEventTrigger(ptr_position, my_trigger_signal);
		}
	}
	else if (desired_type == cre_vector_mask)
	{
		result = new CreatureVectorMask(ptr_position);
	}
	else if (desired_type == cre_npc)
	{
		result = new CreatureNPC(ptr_position);
	}
	else if (desired_type == cre_spell)
	{
		printf("Spell is a base class! Nothing to spawn.\n");
	}
	else if (desired_type == cre_spell_ball)
	{
		result = new CreatureSpellBall(ptr_position);
	}
	else
	{
		printf("Requested some other creature.\n");
	}

	return result;
}