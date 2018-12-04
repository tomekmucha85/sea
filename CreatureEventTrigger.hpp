#ifndef CREATURE_EVENT_TRIGGER_HPP
#define CREATURE_EVENT_TRIGGER_HPP
#include <Creature.hpp>

class CreatureEventTrigger : public Creature
{
    private:
		TriggeredEvent triggered_event = nullptr;
		bool is_hit = false;
    public:
	    CreatureEventTrigger(SDL_Rect* ptr_area, TriggeredEvent my_event);
		void FireEvent();
};
#endif // CREATURE_EVENT_TRIGGER_HPP
