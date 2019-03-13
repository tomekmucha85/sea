#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <SDL.h>
#include <Iedk.h>
#include <IedkErrorCode.h>
#include <Game.hpp>
#include <CustomDefinedTypes.hpp>
#include <TimerInterval.hpp>

class Interface
{
    private:
		InterfaceMode current_mode = interf_none;

		int double_wink_timeout = 400; //in miliseconds
		TimerInterval* ptr_double_wink_timer = nullptr;
    public:
		//Interface();
		void UseInterface(SDL_Event* ptr_my_event_handler);
		void UseInterface(EmoEngineEventHandle my_eEvent);
		void SetInterfaceMode(InterfaceMode my_mode);
		InterfaceMode TellInterfaceMode();

};


#endif
