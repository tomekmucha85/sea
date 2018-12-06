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

    //Where the component is rendered on screen
    ////Move to private!
	SDL_Rect position = { 0,0,0,0 };


	//###################
	//Functions
	//###################
	VisualComponent(SDL_Rect* ptr_my_position);
	void Move(int velocity_x, int velocity_y);
	void SetPosition(SDL_Rect my_position);
	static void SetScreen(Screen* ptr_my_screen);
	static Screen* TellScreen();
};
#endif // VISUAL_COMPONENT_HPP
