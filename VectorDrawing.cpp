#include <VectorDrawing.hpp>

//###############
//CONSTRUCTOR
//###############

//#TODO - uwzglêdniæ kolor przekazywany do konstruktora w fabryce?

VectorDrawing::VectorDrawing(PreciseRect* ptr_my_area, Color my_vector_fill_color) : VisualComponent()
{
	SetMyType(visco_vector);
	//printf("Entered VectorDrawing constructor.\n");
	vector_fill_color = my_vector_fill_color;
	SetPosition(*ptr_my_area);
	SetCenter(CalculateCenterOfGivenPosition(*ptr_my_area));
}

VectorDrawing::VectorDrawing(Coordinates* ptr_my_center, double my_height, double my_width, Color my_vector_fill_color) : VisualComponent()
{
	SetMyType(visco_vector);
	//printf("Entered VectorDrawing constructor.\n");
	vector_fill_color = my_vector_fill_color;
	SetCenter(*ptr_my_center);
	SetPositionW(static_cast<int>(my_width));
	SetPositionH(static_cast<int>(my_height));
	SetPosition(CalculatePositionAroundCenter(center, my_width, my_height));
	//printf("Center at x: %f, y: %f\n", ptr_my_center->x, ptr_my_center->y);
	//printf("Position: x: %f y: %f w: %f h: %f\n", position.x, position.y, position.w, position.h);
}

//###############
//FUNCTIONS
//###############

void VectorDrawing::Move(double step_x, double step_y)
{

	center.x += step_x;
	center.y += step_y;
	position = CalculatePositionAroundCenter(center, position.w, position.h);
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
	//printf("Vector rendered at: x: %f y: %f\n", position_int.x, position_int.y);
	//Reset renderer color back to white
	SDL_SetRenderDrawColor(TellScreen()->renderer, 255, 255, 255, 255);
}

void VectorDrawing::SetPosition(PreciseRect my_position)
{
	position = my_position;
}

void VectorDrawing::SetPositionX(int new_x)
{
	position.x = new_x;
}

void VectorDrawing::SetPositionY(int new_y)
{
	position.y = new_y;
}

void VectorDrawing::SetPositionW(int new_w)
{
	position.w = new_w;
}

void VectorDrawing::SetPositionH(int new_h)
{
	position.h = new_h;
}

void VectorDrawing::SetCenter(Coordinates my_center)
{
	center = my_center;
}

PreciseRect VectorDrawing::TellPosition()
{
	return position;
}

Coordinates VectorDrawing::TellCenter()
{
	return center;
}

void VectorDrawing::SetAngleDegrees(int my_angle)
{
	angle = Angle::NormalizeAngle(my_angle);
}

void VectorDrawing::TurnByAngleDegrees(int my_angle)
{
	angle += my_angle;
	angle = Angle::NormalizeAngle(angle);
}

double VectorDrawing::TellAngleDegrees()
{
	return angle;
}

void VectorDrawing::SetColor(Color my_color)
{
	vector_fill_color = my_color;
}
