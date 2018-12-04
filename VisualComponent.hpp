#ifndef VISUAL_COMPONENT_HPP
#define VISUAL_COMPONENT_HPP
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <Screen.hpp>

class VisualComponent
{

private:
	//###################
	//Functions
	//###################
	static Screen* ptr_screen;

public:
	//###################
	//Variables
	//###################


	//###################
	//Functions
	//###################

	static void SetScreen(Screen* ptr_my_screen);
	static Screen* TellScreen();
};
#endif // VISUAL_COMPONENT_HPP
