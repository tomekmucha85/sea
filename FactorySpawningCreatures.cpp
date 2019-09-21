#include <FactorySpawningCreatures.hpp>

Creature* FactorySpawningCreatures::SpawnCreature(CreatureType desired_type, Coordinates* ptr_my_center, int render_layer)
{
	Creature* result = nullptr;
	if (desired_type == cre_clawy)
	{
		//printf("Requested hero creature.\n");
		result = new CreatureClawy(ptr_my_center);
	}
	else if (desired_type == cre_carrier_a)
	{
		//printf("Requested carrier_a creature. \n");
		result = new CreatureCarrierA(ptr_my_center);
	}
	else if (desired_type == cre_blue_bground)
	{
		//printf("Requested blue spr_background creature. \n");
		result = new CreatureBlueBackground(ptr_my_center);
	}
	else if (desired_type == cre_flying_box)
	{
		//printf("Requested flying spr_box creature. \n");
		result = new CreatureFlyingBox(ptr_my_center);
	}
	else if (desired_type == cre_black_smoke)
	{
		//printf("Requested black smoke creature.\n");
		result = new CreatureBlackSmoke(ptr_my_center);
	}
	else if (desired_type == cre_none)
	{
		throw std::invalid_argument("Trying to spawn None creature! This should not happen.\n");
	}
	else if (desired_type == cre_npc)
	{
		result = new CreatureNPC(ptr_my_center);
	}
	else if (desired_type == cre_spell)
	{
		printf("Spell is a base class! Nothing to spawn.\n");
	}
	else if (desired_type == cre_spell_ball)
	{
		result = new CreatureSpellBall(ptr_my_center);
	}
	else if (desired_type == cre_spell_open_doors)
	{
		result = new CreatureSpellOpenDoors(ptr_my_center);
	}
	else if (desired_type == cre_navgrid_node)
	{
		result = new CreatureNavGridNode(ptr_my_center);
	}
	else
	{
		printf("Trying to spawn invalid Creature type!\n");
		throw std::invalid_argument("Trying to spawn invalid Creature type!\n");
	}

	result->SetMyRenderLayer(render_layer);

	return result;

}

Creature* FactorySpawningCreatures::SpawnCreature(CreatureType desired_type,
	Coordinates* ptr_my_upper_left_corner,
	std::string my_text,
	SDL_Color my_color,
	TTF_Font* ptr_my_font,
	int render_layer)
{
	Creature* result = nullptr;
	if (desired_type == cre_writing)
	{
		printf("Factory will deliver a writing creature.\n");
		result = new CreatureWriting(ptr_my_upper_left_corner, my_text, my_color, ptr_my_font);
	}
	else
	{
		printf("Trying to spawn invalid Creature type!\n");
		throw std::invalid_argument("Trying to spawn invalid Creature type!\n");
	}
	result->SetMyRenderLayer(render_layer);
	return result;
}


Creature* FactorySpawningCreatures::SpawnCreature(CreatureType desired_type, PreciseRect* ptr_position,
	std::string my_trigger_signal)
{
	Creature* result = nullptr;


	if (desired_type == cre_event_trigger)
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
	else
	{
		printf("Trying to spawn invalid Creature type!\n");
		throw std::invalid_argument("Trying to spawn invalid Creature type!\n");
	}

	return result;
}