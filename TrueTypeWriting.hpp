#ifndef TRUE_TYPE_WRITING_HPP
#define TRUE_TYPE_WRITING_HPP
#include <SDL.h>
#include <SDL_ttf.h>
#include <Screen.hpp>
#include <FontBank.hpp>
#include <VisualComponent.hpp>

class TrueTypeWriting : public VisualComponent
{
private:

	//###################
	//Variables
	//###################
	SDL_Texture* ptr_writing_texture = nullptr;

	PreciseRect position = { 0,0,0,0 };
	Coordinates upper_left_corner = { 0,0 };
	Coordinates center = {0,0};
	int angle = 0;
	Color font_fill_color;
	//###################
	//Functions
	//###################

public:

	//###################
	//Variables
	//###################


	//###################
	//Functions
	//###################
	//#TODO konwersja Color na SDL_Color
	TrueTypeWriting(std::string text, 
		Coordinates* ptr_upper_left_corner, 
		TTF_Font* ptr_font = *FontBank::ptr_ptr_default_font,
		Color color = {255,0,0});
	void SetColor(Color my_color);
	void Render();
	void Move(double step_x, double step_y);
	Coordinates TellCenter();
	void SetCenter(Coordinates my_center);
	void SetPosition(PreciseRect my_position);
	PreciseRect TellPosition();
};
#endif // TRUE_TYPE_WRITING_HPP
