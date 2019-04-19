#ifndef TIMER_INTERVAL_HPP
#define TIMER_INTERVAL_HPP
#include <TimerGeneral.hpp>

class TimerInterval : public TimerGeneral
{
private:

public:
	//VARIABLES
	Uint32 interval = 0;
	Uint32 intervals_passed_during_last_check = 0;

	//FUNCTIONS
	TimerInterval(Uint32 my_interval);
	int HowManyIntervalsPassedFromStart();
	bool CheckIfIntervalPassed();
	Uint32 HowManyMilisecondsLeftTillNextIntervalHit();
};


#endif //TIMER_INTERVAL_HPP
