#ifndef VECTOR_DRAWING_HPP
#define VECTOR_DRAWING_HPP
#include <SDL.h>
#include <Screen.hpp>
#include <VisualComponent.hpp>

class VectorDrawing : public VisualComponent
{
private:

	//###################
	//Variables
	//###################
	SDL_Color vector_fill_color;
	PreciseRect position = { 0,0,0,0 };
	Coordinates center = { 0,0 };
	int angle = 0;
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
	VectorDrawing(PreciseRect* ptr_my_area, SDL_Color my_vector_fill_color = {255,255,255,0});
	//#TODO - uwzglêdniæ ten konstruktor w VisualComponent?
	VectorDrawing(Coordinates* ptr_my_center, double my_width = 10, double my_height = 10, SDL_Color my_vector_fill_color = { 255,255,255,0 });
	void SetPosition(PreciseRect my_position);
    void SetPositionX(int new_x);
    void SetPositionY(int new_y);
    void SetPositionW(int new_w);
    void SetPositionH(int new_h);
    void SetCenter(Coordinates my_center);
    PreciseRect TellPosition();
    Coordinates TellCenter();
    void SetAngleDegrees(int my_angle);
    void TurnByAngleDegrees(int my_angle);
    double TellAngleDegrees();
	void SetColor(SDL_Color my_color);
	void Render();
	void Move(double step_x, double step_y);
};
#endif // VECTOR_DRAWING_HPP
