#include <VectorDrawing.hpp>

//###############
//CONSTRUCTOR
//###############

//#TODO - uwzglêdniæ kolor przekazywany do konstruktora w fabryce?

VectorDrawing::VectorDrawing(SDL_Rect* ptr_my_area, Color my_vector_fill_color) : VisualComponent(ptr_my_area)
{
	printf("Entered VectorDrawing constructor.\n");
	printf("Constructed VectorDrawing.\n");
	vector_fill_color = my_vector_fill_color;
}

//###############
//FUNCTIONS
//###############

void VectorDrawing::Render()
{
	//printf("Will render vector.\n");
	SDL_Rect* ptr_area = &position;
	SDL_SetRenderDrawColor(TellScreen()->renderer, vector_fill_color.red,
		vector_fill_color.green, 
		vector_fill_color.blue,
		vector_fill_color.alpha);
	SDL_RenderFillRect(TellScreen()->renderer, ptr_area);
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

