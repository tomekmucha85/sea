#ifndef VISUAL_COMPONENT_HPP
#define VISUAL_COMPONENT_HPP
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <Screen.hpp>
#include <CustomDefinedTypes.hpp>

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
    ////#TODO Move to private!
	PreciseRect position = { 0,0,0,0 };


	//###################
	//Functions
	//###################
	VisualComponent(PreciseRect* ptr_my_position);
	VisualComponent(Coordinates* ptr_my_center);
	void Move(double step_x, double step_y);
	void SetPosition(PreciseRect my_position);
	static void SetScreen(Screen* ptr_my_screen);
	static Screen* TellScreen();

	virtual void SetColor(Color my_color);
};
#endif // VISUAL_COMPONENT_HPP
