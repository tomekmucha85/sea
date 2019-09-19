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
	std::string current_text = "";
	SDL_Texture* ptr_writing_texture = nullptr;
	TTF_Font* ptr_font = nullptr;
	SDL_Rect texture_clip = {0,0,0,0};
	PreciseRect position = { 0,0,0,0 };
	Coordinates upper_left_corner = { 0,0 };
	Coordinates center = {0,0};
	SDL_Color color = {0,0,0,0};
	int angle = 0;
	//SDL_Color font_fill_color;
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
	TrueTypeWriting(std::string text, 
		Coordinates* ptr_upper_left_corner, 
		TTF_Font* ptr_my_font,
		SDL_Color color);
	~TrueTypeWriting();
	void Render();
	void Move(double step_x, double step_y);
	Coordinates TellCenter();
	void SetCenter(Coordinates my_center);
	void SetText(std::string my_text);
	std::string TellText();
	void SetFont(TTF_Font* ptr_my_font);
	void SetColor(SDL_Color my_color);
	void SetPosition(PreciseRect my_position);
	PreciseRect TellPosition();
	Coordinates TellUpperLeftCorner();
};
#endif // TRUE_TYPE_WRITING_HPP
