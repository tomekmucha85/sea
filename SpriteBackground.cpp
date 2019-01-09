#include <SpriteBackground.hpp>

//***********************************
//DEFINITIONS OF STATIC CLASS MEMBERS
//***********************************

SDL_Rect SpriteBackground::initial_texture_clip_background = { 0,0,691,461 };
//Zero width and height will make Sprite Class show whole texture.

//***********************************
//CONSTRUCTORS
//***********************************

SpriteBackground::SpriteBackground(Coordinates* ptr_my_center)
	: Sprite(TellTextureBank()->ptr_tex_background, SpriteBackground::initial_texture_clip_background, ptr_my_center)
{
	//printf("Constructor called for SpriteBackground\n");
}
