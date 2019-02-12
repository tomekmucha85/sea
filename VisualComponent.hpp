#ifndef VISUAL_COMPONENT_HPP
#define VISUAL_COMPONENT_HPP
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <Screen.hpp>
#include <CustomDefinedTypes.hpp>
#include <CommonMath.hpp>

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
	Coordinates start = {0,0};
	Coordinates end = {0, 0};
	PreciseRect position = { 0,0,0,0 };
	Coordinates center = {0,0};
	int angle = 0;

	//###################
	//Functions
	//###################

	//#TODO - przeorganizowaæ po dodaniu linii!

	VisualComponent(PreciseRect* ptr_my_position);
	VisualComponent(Coordinates* ptr_my_center);
	VisualComponent(Coordinates* ptr_my_start, Coordinates* ptr_my_end);

	virtual ~VisualComponent();

	VisualComponentType TellMyType();
	void SetMyType(VisualComponentType my_type);
	virtual void Move(double step_x, double step_y) = 0;
	void SetPosition(PreciseRect my_position);
	void SetPositionX(int new_x);
	void SetPositionY(int new_y);
	void SetPositionW(int new_w);
	void SetPositionH(int new_h);
	void SetStart(Coordinates my_start);
	void SetEnd(Coordinates my_end);
	virtual void SetCenter(Coordinates my_center);
	PreciseRect TellPosition();
	Coordinates TellCenter();
	void SetAngleDegrees(int my_angle);
	void TurnByAngleDegrees(int my_angle);
	virtual void Render() = 0;
	static void SetScreen(Screen* ptr_my_screen);

	//VIRTUAL FUNCTIONS
	virtual void SetColor(Color my_color);
	virtual void SetClipAccordingToWallType(WallType my_type);

	//STATIC HELPERS
	static Screen* TellScreen();
	static SDL_Rect ConvertPreciseRectToSdlRect(PreciseRect my_rect);
	static SDL_Point ConvertCoordinatesToSdlPoint(Coordinates my_coordinates);
	static PreciseRect ConvertSdlRectToPreciseRect(SDL_Rect my_rect);
};
#endif // VISUAL_COMPONENT_HPP
