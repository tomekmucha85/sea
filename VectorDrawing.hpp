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
		VectorDrawing(SDL_Rect* ptr_my_area);
		void Render();
};
#endif // VECTOR_DRAWING_HPP
