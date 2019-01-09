#ifndef TIMER_HPP
#define TIMER_HPP
#include <stdio.h>
#include <SDL.h>

class Timer
{
    private:
    
    public:
		//VARIABLES
		static double loop_duration;
		static double time_last_loop_finished;
		//static std::map<Uint32, Uint32> start_time_vs_interval;

		//FUNCTIONS
		static void CalculateLoopDuration();
		static double GetCurrentTime();
};


#endif //TIMER_HPP
