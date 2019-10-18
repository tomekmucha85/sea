#include <SpriteBackground.hpp>

//***********************************
//DEFINITIONS OF STATIC CLASS MEMBERS
//***********************************

SDL_Rect SpriteBackground::initial_texture_clip_background = { 0,0,1856,1856 };
SDL_Rect SpriteBackground::grassy_texture_clip_background = { 1856,0,1856,1856 };

//Zero width and height will make Sprite Class show whole texture.

//***********************************
//CONSTRUCTORS
//***********************************

SpriteBackground::SpriteBackground(Coordinates* ptr_my_center)
	: Sprite(TellTextureBank()->ptr_tex_background, SpriteBackground::initial_texture_clip_background, ptr_my_center)
{
	//printf("Constructor called for SpriteBackground\n");
}

//***********************************
//FUNCTIONS
//***********************************

void SpriteBackground::SetGrassyClip()
{
	SetTextureClip(grassy_texture_clip_background);
}

void SpriteBackground::SetInitialClip()
{
	SetTextureClip(initial_texture_clip_background);
}