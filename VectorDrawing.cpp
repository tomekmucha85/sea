#include <VectorDrawing.hpp>

//###############
//CONSTRUCTOR
//###############

VectorDrawing::VectorDrawing(SDL_Rect my_area)
{
	printf("Entered VectorDrawing constructor.\n");
	area = my_area;
	printf("Constructed VectorDrawing.\n");
}

//###############
//FUNCTIONS
//###############

void VectorDrawing::Render()
{
	//printf("Will render vector.\n");
	SDL_Rect* ptr_area = &area;
	SDL_RenderDrawRect(TellScreen()->renderer, ptr_area);
}

//###############
//STATIC METHODS
//###############

