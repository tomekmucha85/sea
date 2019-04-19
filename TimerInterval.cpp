#include <TimerInterval.hpp>

TimerInterval::TimerInterval(Uint32 my_interval) : TimerGeneral()
{
	interval = my_interval; //miliseconds
}

int TimerInterval::HowManyIntervalsPassedFromStart()
{
	Uint32 current_time = SDL_GetTicks();
	Uint32 intervals_passed_so_far = Uint32(HowManyMilisecondsPassedFromStart() / interval);
	return intervals_passed_so_far;
}

bool TimerInterval::CheckIfIntervalPassed()
{
	Uint32 current_time = SDL_GetTicks();
	Uint32 intervals_passed_so_far = HowManyIntervalsPassedFromStart();
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

Uint32 TimerInterval::HowManyMilisecondsLeftTillNextIntervalHit()
{
	Uint32 time_passed_from_start = HowManyMilisecondsPassedFromStart();
	int intervals_passed_from_start = HowManyIntervalsPassedFromStart();
	Uint32 result = time_passed_from_start % (intervals_passed_from_start * interval);
	return result;
}
