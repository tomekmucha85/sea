#include <SpriteImplosion.hpp>

//***********************************
//DEFINITIONS OF STATIC CLASS MEMBERS
//***********************************

SDL_Rect SpriteImplosion::initial_texture_clip_implosion = { 0,0,50,50 };
std::vector<SDL_Rect> SpriteImplosion::implosion_animation_clips = Sprite::CalculateAnimationClips({ 0,0,6000,50 }, 
	SpriteImplosion::initial_texture_clip_implosion.w,
	SpriteImplosion::initial_texture_clip_implosion.h);

//***********************************
//CONSTRUCTORS
//***********************************

SpriteImplosion::SpriteImplosion(Coordinates* ptr_my_center)
	: Sprite(TellTextureBank()->ptr_tex_implosion, 
		SpriteImplosion::initial_texture_clip_implosion, 
		ptr_my_center)
{
	;
}

//***********************************
//OVERLOADED VIRTUAL METHODS
//***********************************

void SpriteImplosion::IdleAnimation(bool* ptr_did_animation_reach_last_frame)
{
	ImplosionAnimation();
}

void SpriteImplosion::ImplosionAnimation(bool* ptr_did_animation_reach_last_frame)
{
	Sprite::PlayAnimation(implosion_animation_clips, ptr_did_animation_reach_last_frame, 1);
}
