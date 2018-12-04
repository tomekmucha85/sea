#include <SpriteBlackBox.hpp>

//***********************************
//DEFINITIONS OF STATIC CLASS MEMBERS
//***********************************

SDL_Rect SpriteBlackBox::initial_texture_clip_black_box = { 0,0,40,40 };

//***********************************
//CONSTRUCTORS
//***********************************

SpriteBlackBox::SpriteBlackBox(SDL_Rect* ptr_my_position)
	: Sprite(TellTextureBank()->ptr_tex_black_box, SpriteBlackBox::initial_texture_clip_black_box, ptr_my_position)
{
	//printf("Constructor called for SpriteBlackBox\n");
}


