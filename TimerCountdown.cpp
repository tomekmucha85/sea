#include <TimerCountdown.hpp>

TimerCountdown::TimerCountdown(Uint32 my_time_to_live) : TimerGeneral()
{
	time_to_live = my_time_to_live;
}

bool TimerCountdown::CheckIfCountdownFinished()
{
	Uint32 current_time = SDL_GetTicks();
	if (HowManyMilisecondsLeftTillEnd() > 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

Uint32 TimerCountdown::HowManyMilisecondsLeftTillEnd()
{
	Uint32 current_time = SDL_GetTicks();
	Sint32 result = time_to_live - (current_time - start_time);
	if (result < 0)
	{
		result = 0;
	}
	//printf("Time left: %d.\n", result);
	return result;
}