#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <SDL.h>
#include <Game.hpp>
#include <CustomDefinedTypes.hpp>
#include <Iedk.h>
#include <IedkErrorCode.h>

class Interface
{
    private:
		InterfaceMode current_mode = interf_none;
    public:
		//Interface();
		void UseInterface(SDL_Event* ptr_my_event_handler);
		void UseInterface(EmoEngineEventHandle my_eEvent);
		void SetInterfaceMode(InterfaceMode my_mode);
		InterfaceMode TellInterfaceMode();

};


#endif
