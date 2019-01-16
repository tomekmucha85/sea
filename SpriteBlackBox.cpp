#include <SpriteBlackBox.hpp>

//***********************************
//DEFINITIONS OF STATIC CLASS MEMBERS
//***********************************

SDL_Rect SpriteBlackBox::initial_texture_clip_black_box = { 336,0,48,48 };

//***********************************
//CONSTRUCTORS
//***********************************

SpriteBlackBox::SpriteBlackBox(Coordinates* ptr_my_center)
	: Sprite(TellTextureBank()->ptr_tex_wall, SpriteBlackBox::initial_texture_clip_black_box, ptr_my_center)
{
	//printf("Constructor called for SpriteBlackBox\n");
}


