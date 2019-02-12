#include <SpriteExit.hpp>

//***********************************
//DEFINITIONS OF STATIC CLASS MEMBERS
//***********************************

SDL_Rect SpriteExit::initial_texture_clip_vortex = { 0,0,96,48 };

//***********************************
//CONSTRUCTORS
//***********************************

SpriteExit::SpriteExit(Coordinates* ptr_my_center)
	: Sprite(TellTextureBank()->ptr_tex_exit, SpriteExit::initial_texture_clip_vortex, ptr_my_center)
{
	;
}

//***********************************
//OVERLOADED VIRTUAL METHODS
//***********************************

void SpriteExit::SetClipAccordingToDoorState(DoorState my_state)
{
	if (my_state == door_closed)
	{
		SetTextureClip(texture_clip_door_closed);
	}
	else if (my_state == door_open)
	{
		SetTextureClip(texture_clip_door_open);
	}
	else if (my_state == door_open_lit)
	{
		SetTextureClip(texture_clip_door_open_lit);
	}
	else
	{
		printf("Unknown door state.\n");
		throw std::invalid_argument("Unknown door state.\n");
	}
}