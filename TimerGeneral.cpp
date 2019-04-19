#include <TimerGeneral.hpp>

TimerGeneral ::TimerGeneral()
{
	start_time = SDL_GetTicks();
}

Uint32 TimerGeneral::HowManyMilisecondsPassedFromStart()
{
	Uint32 current_time = SDL_GetTicks();
	Uint32 result = current_time - start_time;
	return result;
}

void TimerGeneral::ResetTimer()
{
	Uint32 current_time = SDL_GetTicks();
	start_time = current_time;
}