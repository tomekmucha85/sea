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

