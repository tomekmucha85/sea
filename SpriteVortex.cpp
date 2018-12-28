#include <SpriteVortex.hpp>

//***********************************
//DEFINITIONS OF STATIC CLASS MEMBERS
//***********************************

SDL_Rect SpriteVortex::initial_texture_clip_vortex = { 0,0,50,50 };

//***********************************
//CONSTRUCTORS
//***********************************

SpriteVortex::SpriteVortex(SDL_Rect* ptr_my_position)
	: Sprite(TellTextureBank()->ptr_tex_purple_vortex, SpriteVortex::initial_texture_clip_vortex, ptr_my_position)
{
	//printf("Constructor called for SpriteBlackSmoke\n");
}


//***********************************
//OVERLOADED VIRTUAL METHODS
//***********************************

void SpriteVortex::VortexAnimation()
{
	SDL_Rect vortex_animation_area = { 2000,0,6000,50 };
	SDL_Rect current_texture_clip = TellTextureClip();
	int timeout_between_frames = 1;
	int clip_w = current_texture_clip.w;
	int clip_h = current_texture_clip.h;
	std::vector <SDL_Rect> vortex_clips = Sprite::CalculateAnimationClips(vortex_animation_area, clip_w, clip_h);
	Sprite::PlayAnimation(vortex_clips, timeout_between_frames);
}