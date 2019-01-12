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
	VisualComponentType type = visco_undetermined;

public:
	//###################
	//Variables
	//###################

    //Where the component is rendered on screen
    ////#TODO Move to private!
	PreciseRect position = { 0,0,0,0 };
	Coordinates center = {0,0};
	double angle = 0;

	//###################
	//Functions
	//###################
	VisualComponent(PreciseRect* ptr_my_position);
	VisualComponent(Coordinates* ptr_my_center);
	VisualComponentType TellMyType();
	void SetMyType(VisualComponentType my_type);
	void Move(double step_x, double step_y);
	void SetPosition(PreciseRect my_position);
	void SetPositionX(int new_x);
	void SetPositionY(int new_y);
	void SetPositionW(int new_w);
	void SetPositionH(int new_h);
	void SetCenter(Coordinates my_center);
	virtual void Render() = 0;
	static void SetScreen(Screen* ptr_my_screen);
	virtual void SetColor(Color my_color);

	static Screen* TellScreen();
	static SDL_Rect ConvertPreciseRectToSdlRect(PreciseRect my_rect);
	static SDL_Point ConvertCoordinatesToSdlPoint(Coordinates my_coordinates);
	static PreciseRect ConvertSdlRectToPreciseRect(SDL_Rect my_rect);
};
#endif // VISUAL_COMPONENT_HPP
