#include <VisualComponent.hpp>

//####################################
// DEFINITIONS OF STATIC CLASS MEMBERS
//####################################

Screen* VisualComponent::ptr_screen = nullptr;

//#######################
// CONSTRUCTOR
//#######################

VisualComponent::VisualComponent(SDL_Rect* ptr_my_position)
{
	SetPosition(*ptr_my_position);
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

void VisualComponent::SetPosition(SDL_Rect my_position)
{
	position = my_position;
}

void VisualComponent::Move(int velocity_x, int velocity_y)
{
	position.x += velocity_x;
	position.y += velocity_y;
	velocity_x = 0;
	velocity_y = 0;
}