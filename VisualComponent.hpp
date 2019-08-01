#ifndef VISUAL_COMPONENT_HPP
#define VISUAL_COMPONENT_HPP
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <Screen.hpp>
#include <texture.hpp>
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


	//###################
	//Functions
	//###################

	//#TODO - przeorganizowaæ po dodaniu linii!

	VisualComponent();

	virtual ~VisualComponent();

	VisualComponentType TellMyType();
	void SetMyType(VisualComponentType my_type);
	static void SetScreen(Screen* ptr_my_screen);

	//PURE VIRTUAL FUNCTIONS
	virtual void Move(double step_x, double step_y) = 0;
	virtual void Render() = 0;
	virtual Coordinates TellCenter() = 0;
	virtual void SetCenter(Coordinates my_center) = 0;

	//VIRTUAL FUNCTIONS
	virtual void SetAngleDegrees(int my_angle);
	virtual void TurnByAngleDegrees(int my_angle);

	virtual void SetColor(SDL_Color my_color);
	virtual void SetClipAccordingToWallType(WallType my_type);
	virtual void PlayCurrentAnimation();
	virtual void SetCurrentAnimation(AnimationType my_animation_type);
	virtual void SetInterruptingAnimation(AnimationType my_animation_type,
		int times_to_be_played=1);

	//STATIC HELPERS       
	static SDL_Rect CheckTextureDimensions(SDL_Texture* ptr_my_texture);
	static Screen* TellScreen();
	static SDL_Rect ConvertPreciseRectToSdlRect(PreciseRect my_rect);
	static SDL_Point ConvertCoordinatesToSdlPoint(Coordinates my_coordinates);
	static PreciseRect ConvertSdlRectToPreciseRect(SDL_Rect my_rect);
	PreciseRect CalculatePositionAroundCenter(Coordinates my_center,
		double width,
		double height);
	Coordinates CalculateCenterOfGivenPosition(PreciseRect my_position);
};
#endif // VISUAL_COMPONENT_HPP
