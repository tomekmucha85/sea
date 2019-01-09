#include <VisualComponent.hpp>

//####################################
// DEFINITIONS OF STATIC CLASS MEMBERS
//####################################

Screen* VisualComponent::ptr_screen = nullptr;

//#######################
// CONSTRUCTOR
//#######################

VisualComponent::VisualComponent(PreciseRect* ptr_my_position)
{
	SetPosition(*ptr_my_position);
}

VisualComponent::VisualComponent(Coordinates* ptr_my_center)
{
	//#TODO - co z t¹ funkcj¹?
	//SetPosition(*ptr_my_position);
	;
}

//#####################
// STATIC METHODS
//#####################

Screen* VisualComponent::TellScreen()
{
	return ptr_screen;
}

void VisualComponent::SetScreen(Screen *ptr_my_screen)
{
	ptr_screen = ptr_my_screen;
}

void VisualComponent::SetPosition(PreciseRect my_position)
{
	position = my_position;
}

void VisualComponent::Move(double step_x, double step_y)
{
	position.x += step_x;
	position.y += step_y;
	/*printf("Moved %p by x: %f y: %f. Current position: x: %f, y: %f, w: %f, h: %f.\n",
		this, step_x, step_y, position.x, position.y, position.w, position.h);
	printf("Casting position to int: x: %d, y: %d, w: %d, h: %d.\n",
		static_cast<int>(position.x),
		static_cast<int>(position.y), 
		static_cast<int>(position.w), 
		static_cast<int>(position.h));*/
}

//#####################
// VIRTUAL METHODS
//#####################

void VisualComponent::SetColor(Color my_color)
{
	printf("Default implementation of SetColor called!\n");
}