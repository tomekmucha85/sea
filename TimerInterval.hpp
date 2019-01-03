#ifndef TIMER_PERSONAL_HPP
#define IMER_PERSONAL_HPP
#include <stdio.h>
#include <SDL.h>
#include <Timer.hpp>

class TimerInterval
{
private:

public:
	//VARIABLES
	Uint32 start_time = 0;
	Uint32 interval = 0;
	int intervals_passed_during_last_check = 0;

	//FUNCTIONS
	TimerInterval(Uint32 my_interval);
	Uint32 HowMuchTimePassedFromStart();
	bool CheckIfIntervalPassed();
};


#endif //TIMER_HPP
