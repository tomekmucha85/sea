#include <TimerCountdown.hpp>

TimerCountdown::TimerCountdown(Uint32 my_time_to_live) : TimerGeneral()
{
	time_to_live = my_time_to_live;
}


void TimerCountdown::ResetWithNewTimeToLive(Uint32 new_time_to_live)
{
	time_to_live = new_time_to_live;
	TimerGeneral::ResetStartTime();
}

bool TimerCountdown::CheckIfCountdownFinished()
{
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