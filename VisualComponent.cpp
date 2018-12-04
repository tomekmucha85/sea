#include <VisualComponent.hpp>

//####################################
// DEFINITIONS OF STATIC CLASS MEMBERS
//####################################

Screen* VisualComponent::ptr_screen = nullptr;

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