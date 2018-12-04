#include <CreatureEventTrigger.hpp>

CreatureEventTrigger::CreatureEventTrigger(SDL_Rect* ptr_area, TriggeredEvent my_event) :
	Creature(ptr_area)
{
	printf("Spawned an event trigger.\n");
	triggered_event = my_event;
	printf("Assigned event.\n");
	my_type = cre_event_trigger;
	printf("Assigned type.\n");
	MakeMeNotObstacle();
	printf("No obstacle set.\n");
	ptr_creature_vector = new VectorDrawing(*ptr_area);
}

void CreatureEventTrigger::FireEvent()
{
	printf("Will fire event!\n");
	triggered_event;
	printf("Fired event.\n");
}
