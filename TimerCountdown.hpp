#ifndef TIMER_COUNTDOWN_HPP
#define TIMER_COUNTDOWN_HPP
#include <TimerGeneral.hpp>

class TimerCountdown : public TimerGeneral
{
private:

public:
	//VARIABLES
	Uint32 time_to_live = 0;

	//FUNCTIONS
	TimerCountdown(Uint32 my_time_to_live);
	bool CheckIfCountdownFinished();
	Uint32 HowManyMilisecondsLeftTillEnd();
	void ResetWithNewTimeToLive(Uint32 new_time_to_live);
};

#endif //TIMER_COUNTDOWN_HPP