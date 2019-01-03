#ifndef TIMER_HPP
#define TIMER_HPP
#include <stdio.h>
#include <SDL.h>

class Timer
{
    private:
    
    public:
		//VARIABLES
		static Uint32 loop_duration;
		static Uint32 time_last_loop_finished;
		//static std::map<Uint32, Uint32> start_time_vs_interval;

		//FUNCTIONS
		static void CalculateLoopDuration();
		static Uint32 GetCurrentTime();
		static bool CheckIfIntervalSinceGivenTimePassed(Uint32 given_time, Uint32 interval);
};


#endif //TIMER_HPP
