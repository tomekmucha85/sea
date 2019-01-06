#ifndef CREATURE_EVENT_TRIGGER_HPP
#define CREATURE_EVENT_TRIGGER_HPP
#include <Creature.hpp>

class CreatureEventTrigger : public Creature
{
    private:
		std::string trigger_signal = "";
		bool is_hit = false;
		bool is_armed = true;
		//bool should_be_destroyed_after_use = true;
    public:
	    CreatureEventTrigger(PreciseRect* ptr_my_area, std::string my_trigger_signal);
		std::string GiveSignal();
		void ArmTrigger();
		void DisarmTrigger();
		bool AmIArmed();
		/*void MakeDisposable();
		void MakePermanent();
		bool AmIDisposable();*/
};
#endif // CREATURE_EVENT_TRIGGER_HPP
