#include <VectorDrawing.hpp>

//###############
//CONSTRUCTOR
//###############

VectorDrawing::VectorDrawing(SDL_Rect* ptr_my_area) : VisualComponent(ptr_my_area)
{
	printf("Entered VectorDrawing constructor.\n");
	printf("Constructed VectorDrawing.\n");
}

//###############
//FUNCTIONS
//###############

void VectorDrawing::Render()
{
	//printf("Will render vector.\n");
	SDL_Rect* ptr_area = &position;
	SDL_RenderDrawRect(TellScreen()->renderer, ptr_area);
}

//###############
//STATIC METHODS
//###############

