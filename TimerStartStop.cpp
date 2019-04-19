#include <TimerStartStop.hpp>

TimerStartStop::TimerStartStop() : TimerGeneral()
{
	;
}

void TimerStartStop::ContinueMeasurement()
{
	;
}

void TimerStartStop::Stop()
{
	aggregated_time += HowManyMilisecondsPassedFromStart();
}

Uint32 TimerStartStop::ReadAggregatedTime()
{
	return aggregated_time;
}
