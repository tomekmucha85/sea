#include <Timer.hpp>

//***********************************
//DEFINITIONS OF STATIC CLASS MEMBERS
//***********************************
Uint32 Timer::loop_duration = 0;
Uint32 Timer::time_last_loop_finished = NULL;


//***********************************
//FUNCTIONS
//***********************************

Uint32 Timer::GetCurrentTime()
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
		Uint32 current_time = GetCurrentTime();
		loop_duration = current_time - time_last_loop_finished;
		time_last_loop_finished = current_time;
	}
}


