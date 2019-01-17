#include <Creature.hpp>

//How much does each spell cost?
std::map<SpellName, int> Magic::spell_name_vs_mana_cost = 
{
	{spell_none, 0 },
	{spell_vortex, 1 },
	{spell_open_gate, 2 }
};

bool Magic::CastSpell(SpellName desired_name, Creature* ptr_wizard)
{
	bool was_casting_succesfull = true;

	if (!DoIHaveEnoughMana(desired_name, ptr_wizard))
	{
		was_casting_succesfull = false;
		return was_casting_succesfull;
	}
	else
	{
		PaySpellCost(desired_name, ptr_wizard);
		if (desired_name == spell_vortex)
		{
			CreatureSpawnRequest vortex_request;
			//Spawn request #1
			vortex_request.type = cre_spell_ball;
			vortex_request.initial_behavior_mode = beh_projectile;
			vortex_request.mode = center_coordinates;
			int desired_distance = 100;
			Coordinates center_coordinates = ptr_wizard->CalculatePointInGivenDistanceFromCreatureCenter(desired_distance);
			vortex_request.initial_center_cooridnates = { center_coordinates.x, center_coordinates.y };
			vortex_request.initial_angle_degree = ptr_wizard->TellCurrentAngleDegree();
			vortex_request.insertion_mode = merge;
			//Placing spawn request #1
			ptr_wizard->PushIntoSpawnRequests(vortex_request);
		}
		else if (desired_name == spell_open_gate)
		{
			CreatureSpawnRequest gate_request;
			//Spawn request #1
			gate_request.type = cre_spell_open_doors;
			gate_request.initial_behavior_mode = beh_idle;
			gate_request.mode = center_coordinates;
			int desired_distance = 100;
			Coordinates center_coordinates = ptr_wizard->CalculatePointInGivenDistanceFromCreatureCenter(desired_distance);
			gate_request.initial_center_cooridnates = { center_coordinates.x, center_coordinates.y };
			gate_request.initial_angle_degree = ptr_wizard->TellCurrentAngleDegree();
			gate_request.insertion_mode = merge;
			//Placing spawn request #1
			ptr_wizard->PushIntoSpawnRequests(gate_request);
		}
		else
		{
			printf("Wrong spell name!\n");
			throw std::invalid_argument("Wrong spell name!\n");
		}
	}
	return was_casting_succesfull;
}


bool Magic::DoIHaveEnoughMana(SpellName desired_name, Creature* ptr_wizard)
{
	bool result = false;
	int available_mana = ptr_wizard->TellManaLevel();
	if (available_mana >= spell_name_vs_mana_cost[desired_name])
	{
		result = true;
	}
	return result;
}

void Magic::PaySpellCost(SpellName desired_name, Creature* ptr_wizard)
{
	ptr_wizard->ChangeManaLevel(spell_name_vs_mana_cost[desired_name]*-1);
	printf("Current mana level: %d.\n", ptr_wizard->TellManaLevel());
}