#ifndef TIMER_STARTSTOP_HPP
#define TIMER_STARTSTOP_HPP
#include <TimerGeneral.hpp>

class TimerStartStop : public TimerGeneral
{
private:
	bool is_counting = true;
	Uint32 aggregated_time = 0;

public:

	//FUNCTIONS
	TimerStartStop();
	void Start();
	void Stop();
	Uint32 Read();
	bool IsTicking();
	void ResetTimer();

};

#endif //TIMER_STARTSTOP_HPP