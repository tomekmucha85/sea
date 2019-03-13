#include <TimerInterval.hpp>

TimerInterval::TimerInterval(Uint32 my_interval)
{
	start_time = SDL_GetTicks();
	interval = my_interval; //miliseconds
}


Uint32 TimerInterval::HowMuchTimePassedFromStart()
{
	Uint32 current_time = SDL_GetTicks();
	Uint32 result = current_time - start_time;
	return result;
}



bool TimerInterval::CheckIfIntervalPassed()
{
	Uint32 current_time = SDL_GetTicks();
	Uint32 intervals_passed_so_far = Uint32((current_time - start_time) / interval);
	if (intervals_passed_so_far > intervals_passed_during_last_check)
	{
		intervals_passed_during_last_check = intervals_passed_so_far;
		return true;
	}
	else
	{
		return false;
	}
}