#include <SpriteVortex.hpp>

//***********************************
//DEFINITIONS OF STATIC CLASS MEMBERS
//***********************************

SDL_Rect SpriteVortex::initial_texture_clip_vortex = { 0,0,50,50 };
std::vector<SDL_Rect> SpriteVortex::vortex_animation_clips = Sprite::CalculateAnimationClips({ 2000,0,6000,50 }, 
	SpriteVortex::initial_texture_clip_vortex.w,
	SpriteVortex::initial_texture_clip_vortex.h);

//***********************************
//CONSTRUCTORS
//***********************************

SpriteVortex::SpriteVortex(Coordinates* ptr_my_center)
	: Sprite(TellTextureBank()->ptr_tex_purple_vortex, 
		SpriteVortex::initial_texture_clip_vortex, 
		ptr_my_center)
{
	;
}

//***********************************
//OVERLOADED VIRTUAL METHODS
//***********************************

void SpriteVortex::IdleAnimation(bool* ptr_did_animation_reach_last_frame)
{
	VortexAnimation(ptr_did_animation_reach_last_frame);
}

void SpriteVortex::VortexAnimation(bool* ptr_did_animation_reach_last_frame)
{
	Sprite::PlayAnimation(vortex_animation_clips,
		ptr_did_animation_reach_last_frame,
		1);
}
