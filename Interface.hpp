#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <SDL.h>
#include <Iedk.h>
#include <IedkErrorCode.h>
#include <Game.hpp>
#include <CustomDefinedTypes.hpp>
#include <TimerInterval.hpp>
#include <Logger.hpp>

class Interface
{
    private:
		InterfaceMode current_mode = interf_menu;
    public:
		void SetModeAppropriateToLevelType();
		void UseInterface(SDL_Event* ptr_my_event_handler);
		void UseInterface(std::string bci_detection);
		void SetInterfaceMode(InterfaceMode my_mode);
		InterfaceMode TellInterfaceMode();
};


#endif
