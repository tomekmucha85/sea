#include <Timer.hpp>

//***********************************
//DEFINITIONS OF STATIC CLASS MEMBERS
//***********************************
double Timer::loop_duration = 0;
double Timer::time_last_loop_finished = NULL;


//***********************************
//FUNCTIONS
//***********************************

double Timer::GetCurrentTime()
{
	return SDL_GetTicks();
}

void Timer::CalculateLoopDuration()
{
	if (time_last_loop_finished == NULL)
	{
		//First time calculation
		time_last_loop_finished = GetCurrentTime();
		loop_duration = 0;
	}
	else
	{
		double current_time = GetCurrentTime();
		loop_duration = current_time - time_last_loop_finished;
		time_last_loop_finished = current_time;
	}
}


