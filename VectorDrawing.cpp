#include <VectorDrawing.hpp>

//###############
//CONSTRUCTOR
//###############

//#TODO - uwzglêdniæ kolor przekazywany do konstruktora w fabryce?

VectorDrawing::VectorDrawing(PreciseRect* ptr_my_area, Color my_vector_fill_color) : VisualComponent(ptr_my_area)
{
	SetMyType(visco_vector);
	//printf("Entered VectorDrawing constructor.\n");
	vector_fill_color = my_vector_fill_color;
	SetCenter(CalculateCenterOfCurrentPosition());
}

VectorDrawing::VectorDrawing(Coordinates* ptr_my_center, double my_height, double my_width, Color my_vector_fill_color) : VisualComponent(ptr_my_center)
{
	SetMyType(visco_vector);
	//printf("Entered VectorDrawing constructor.\n");
	vector_fill_color = my_vector_fill_color;
	SetPositionW(static_cast<int>(my_width));
	SetPositionH(static_cast<int>(my_height));
	SetPosition(CalculatePositionAroundCenter());
}

//###############
//FUNCTIONS
//###############

void VectorDrawing::Move(double step_x, double step_y)
{

	center.x += step_x;
	center.y += step_y;
	position = CalculatePositionAroundCenter();
}

PreciseRect VectorDrawing::CalculatePositionAroundCenter()
{
	//#TODO - ucywilizowaæ
	double calculated_position_x = static_cast<int>(center.x - (position.w / 2));
	double calculated_position_y = static_cast<int>(center.y - (position.h / 2));
	double calculated_position_w = position.w;
	double calculated_position_h = position.h;
	return { calculated_position_x, calculated_position_y, calculated_position_w, calculated_position_h };
}

Coordinates VectorDrawing::CalculateCenterOfCurrentPosition()
{
	double calculated_center_x = static_cast<int>(position.x + (position.w / 2));
	double calculated_center_y = static_cast<int>(position.y + (position.h / 2));
	return { calculated_center_x, calculated_center_y };
}

void VectorDrawing::Render()
{
	//printf("Will render vector.\n");
	SDL_Rect position_int = ConvertPreciseRectToSdlRect(position);
	SDL_SetRenderDrawColor(TellScreen()->renderer, vector_fill_color.red,
		vector_fill_color.green, 
		vector_fill_color.blue,
		vector_fill_color.alpha);
	SDL_RenderFillRect(TellScreen()->renderer, &position_int);
	//Reset renderer color back to white
	SDL_SetRenderDrawColor(TellScreen()->renderer, 255, 255, 255, 255);
}

void VectorDrawing::SetColor(Color my_color)
{
	vector_fill_color = my_color;
}

//###############
//STATIC METHODS
//###############

