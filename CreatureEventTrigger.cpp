#include <CreatureEventTrigger.hpp>

CreatureEventTrigger::CreatureEventTrigger(PreciseRect* ptr_area, std::string my_trigger_signal) :
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
	printf("Going to disarm trigger %p.\n", this);
	is_armed = false;
	printf("Disarmed.\n");
}

bool CreatureEventTrigger::AmIArmed()
{
	return is_armed;
}

/*void CreatureEventTrigger::MakeDisposable()
{
	should_be_destroyed_after_use = true;
}

void CreatureEventTrigger::MakePermanent()
{
	should_be_destroyed_after_use = false;
}

bool CreatureEventTrigger::AmIDisposable()
{
	return should_be_destroyed_after_use;
}*/
