#ifndef TIMER_GENERAL_HPP
#define TIMER_GENERAL_HPP
#include <stdio.h>
#include <SDL.h>
#include <Timer.hpp>

class TimerGeneral
{
private:

public:
	//VARIABLES
	Uint32 start_time = 0;

	//FUNCTIONS
	TimerGeneral();
	Uint32 HowManyMilisecondsPassedFromStart();
	void ResetTimer();
};

#endif
