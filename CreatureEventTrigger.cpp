#include <CreatureEventTrigger.hpp>

CreatureEventTrigger::CreatureEventTrigger(SDL_Rect* ptr_area, std::string my_trigger_signal) :
	Creature(ptr_area)
{
	printf("Spawned an event trigger.\n");
	trigger_signal = my_trigger_signal;
	printf("Assigned event.\n");
	my_type = cre_event_trigger;
	printf("Assigned type.\n");
	MakeMeNotObstacle();
	printf("No obstacle set.\n");
	ptr_creature_vector = new VectorDrawing(ptr_area);
	printf("Vector %p assigned to trigger %p.\n", ptr_creature_vector, this);
}

std::string CreatureEventTrigger::GiveSignal()
{
	if (AmIArmed())
	{
		return trigger_signal;
	}
	else
	{
		return std::string("");
	}
}

void CreatureEventTrigger::ArmTrigger()
{
	is_armed = true;
}

void CreatureEventTrigger::DisarmTrigger()
{
	is_armed = false;
}

bool CreatureEventTrigger::AmIArmed()
{
	return is_armed;
}