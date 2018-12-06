#ifndef CREATURE_EVENT_TRIGGER_HPP
#define CREATURE_EVENT_TRIGGER_HPP
#include <Creature.hpp>

class CreatureEventTrigger : public Creature
{
    private:
		std::function<void ()> triggered_event = nullptr;
		bool is_hit = false;
    public:
	    CreatureEventTrigger(SDL_Rect* ptr_area, std::function<void ()> my_event);
		void FireEvent();
};
#endif // CREATURE_EVENT_TRIGGER_HPP
