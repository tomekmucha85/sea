#ifndef TIMER_STARTSTOP_HPP
#define TIMER_STARTSTOP_HPP
#include <TimerGeneral.hpp>

class TimerStartStop : public TimerGeneral
{
private:
	Uint32 stop_time = 0;
	Uint32 aggregated_time = 0;

public:

	//FUNCTIONS
	TimerStartStop();
	void ContinueMeasurement();
	void Stop();
	Uint32 ReadAggregatedTime();

};

#endif //TIMER_STARTSTOP_HPP