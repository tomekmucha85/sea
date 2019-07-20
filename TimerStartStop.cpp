#include <TimerStartStop.hpp>

TimerStartStop::TimerStartStop() : TimerGeneral()
{
	;
}

void TimerStartStop::Start()
{
	ResetStartTime();
	is_counting = true;
}

void TimerStartStop::Stop()
{
	aggregated_time += HowManyMilisecondsPassedFromStart();
	is_counting = false;
}

Uint32 TimerStartStop::Read()
{
	if (is_counting == false)
	{
		return aggregated_time;
	}
	else
	{
		return HowManyMilisecondsPassedFromStart() + aggregated_time;
	}
}

void TimerStartStop::ResetTimer()
{
	//Resets aggregated time and starts counting.
	aggregated_time = 0;
	Start();
}

void TimerStartStop::StopAndErase()
{
	aggregated_time = 0;
	is_counting = false;
}

bool TimerStartStop::IsTicking()
{
	return is_counting;
}