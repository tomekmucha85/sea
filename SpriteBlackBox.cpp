#include <SpriteBlackBox.hpp>

//***********************************
//DEFINITIONS OF STATIC CLASS MEMBERS
//***********************************

SDL_Rect SpriteBlackBox::initial_texture_clip_black_box = { 0,0,32,32 };

//***********************************
//CONSTRUCTORS
//***********************************

SpriteBlackBox::SpriteBlackBox(SDL_Rect* ptr_my_position)
	: Sprite(TellTextureBank()->ptr_tex_wall, SpriteBlackBox::initial_texture_clip_black_box, ptr_my_position)
{
	//printf("Constructor called for SpriteBlackBox\n");
}


