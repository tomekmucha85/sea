#include <SpriteBackground.hpp>
#include <Game.hpp>

//***********************************
//DEFINITIONS OF STATIC CLASS MEMBERS
//***********************************

SDL_Rect SpriteBackground::initial_texture_clip_background = { 0,0,0,0 };
//Zero width and height will make Sprite Class show whole texture.

//***********************************
//CONSTRUCTORS
//***********************************

SpriteBackground::SpriteBackground(SDL_Rect* ptr_my_position)
	: Sprite(Game::ptr_texture_bank->ptr_tex_background, SpriteBackground::initial_texture_clip_background, ptr_my_position)
{
	printf("Constructor called for SpriteBackground\n");
}
