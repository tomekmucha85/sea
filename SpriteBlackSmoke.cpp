#include <SpriteBlackSmoke.hpp>
#include <Game.hpp>

//***********************************
//DEFINITIONS OF STATIC CLASS MEMBERS
//***********************************

SDL_Rect SpriteBlackSmoke::initial_texture_clip_black_smoke = { 0,0,50,47 };

//***********************************
//CONSTRUCTORS
//***********************************

SpriteBlackSmoke::SpriteBlackSmoke(SDL_Rect* ptr_my_position)
	: Sprite(Game::ptr_texture_bank->ptr_tex_black_smoke, SpriteBlackSmoke::initial_texture_clip_black_smoke, ptr_my_position)
{
	printf("Constructor called for SpriteBlackSmoke\n");
}


//***********************************
//OVERLOADED VIRTUAL METHODS
//***********************************

void SpriteBlackSmoke::SmokeAnimation()
{
	SDL_Rect smoke_animation_area = { 0,0,900,47 };
	SDL_Rect current_texture_clip = TellTextureClip();
	int timeout_between_frames = 5;
	int clip_w = current_texture_clip.w;
	int clip_h = current_texture_clip.h;
	std::vector <SDL_Rect> smoke_clips = Sprite::CalculateAnimationClips(smoke_animation_area, clip_w, clip_h);
	Sprite::PlayAnimation(smoke_clips, timeout_between_frames);
}