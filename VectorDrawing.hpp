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
	Color vector_fill_color;
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
	VectorDrawing(PreciseRect* ptr_my_area, Color my_vector_fill_color = {255,255,255,0});
	//#TODO - uwzglêdniæ ten konstruktor w VisualComponent?
	VectorDrawing(Coordinates* ptr_my_center, double my_width = 10, double my_height = 10, Color my_vector_fill_color = { 255,255,255,0 });
	PreciseRect CalculatePositionAroundCenter();
	Coordinates CalculateCenterOfCurrentPosition();
	void SetColor(Color my_color);
	void Render();
	void Move(double step_x, double step_y);
};
#endif // VECTOR_DRAWING_HPP
