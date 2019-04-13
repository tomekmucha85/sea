#include <SpriteSnailmage.hpp>

//***********************************
//DEFINITIONS OF STATIC CLASS MEMBERS
//***********************************

SDL_Rect SpriteSnailmage::initial_texture_clip_snailmage = { 0,0,100,100 };

std::vector<SDL_Rect> SpriteSnailmage::idle_animation_clips_north = Sprite::CalculateAnimationClips({ 0,500,900,100 }, SpriteSnailmage::initial_texture_clip_snailmage.w,
	SpriteSnailmage::initial_texture_clip_snailmage.h);
std::vector<SDL_Rect> SpriteSnailmage::idle_animation_clips_north_east = Sprite::CalculateAnimationClips({ 0,700,900,100 }, SpriteSnailmage::initial_texture_clip_snailmage.w,
	SpriteSnailmage::initial_texture_clip_snailmage.h);
std::vector<SDL_Rect> SpriteSnailmage::idle_animation_clips_east = Sprite::CalculateAnimationClips({ 0,400,900,100 }, SpriteSnailmage::initial_texture_clip_snailmage.w,
	SpriteSnailmage::initial_texture_clip_snailmage.h);
std::vector<SDL_Rect> SpriteSnailmage::idle_animation_clips_south_east = Sprite::CalculateAnimationClips({ 0,200,900,100 }, SpriteSnailmage::initial_texture_clip_snailmage.w,
	SpriteSnailmage::initial_texture_clip_snailmage.h);
std::vector<SDL_Rect> SpriteSnailmage::idle_animation_clips_south = Sprite::CalculateAnimationClips({ 0,0,900,100 }, SpriteSnailmage::initial_texture_clip_snailmage.w,
	SpriteSnailmage::initial_texture_clip_snailmage.h);
std::vector<SDL_Rect> SpriteSnailmage::idle_animation_clips_south_west = Sprite::CalculateAnimationClips({ 0,100,900,100 }, SpriteSnailmage::initial_texture_clip_snailmage.w,
	SpriteSnailmage::initial_texture_clip_snailmage.h);
std::vector<SDL_Rect> SpriteSnailmage::idle_animation_clips_west = Sprite::CalculateAnimationClips({ 0,300,900,100 }, SpriteSnailmage::initial_texture_clip_snailmage.w,
	SpriteSnailmage::initial_texture_clip_snailmage.h);
std::vector<SDL_Rect> SpriteSnailmage::idle_animation_clips_north_west = Sprite::CalculateAnimationClips({ 0,600,900,100 }, SpriteSnailmage::initial_texture_clip_snailmage.w,
	SpriteSnailmage::initial_texture_clip_snailmage.h);

std::vector<SDL_Rect> SpriteSnailmage::walk_animation_clips_north = Sprite::CalculateAnimationClips({ 0, 1300, 900, 100 }, SpriteSnailmage::initial_texture_clip_snailmage.w,
	SpriteSnailmage::initial_texture_clip_snailmage.h);
std::vector<SDL_Rect> SpriteSnailmage::walk_animation_clips_north_east = Sprite::CalculateAnimationClips({ 0, 1500, 900, 100 }, SpriteSnailmage::initial_texture_clip_snailmage.w,
	SpriteSnailmage::initial_texture_clip_snailmage.h);
std::vector<SDL_Rect> SpriteSnailmage::walk_animation_clips_east = Sprite::CalculateAnimationClips({ 0, 1200, 900, 100 }, SpriteSnailmage::initial_texture_clip_snailmage.w,
	SpriteSnailmage::initial_texture_clip_snailmage.h);
std::vector<SDL_Rect> SpriteSnailmage::walk_animation_clips_south_east = Sprite::CalculateAnimationClips({ 0, 900, 1000, 100 }, SpriteSnailmage::initial_texture_clip_snailmage.w,
	SpriteSnailmage::initial_texture_clip_snailmage.h);
std::vector<SDL_Rect> SpriteSnailmage::walk_animation_clips_south = Sprite::CalculateAnimationClips({ 0, 800, 900, 100 }, SpriteSnailmage::initial_texture_clip_snailmage.w,
	SpriteSnailmage::initial_texture_clip_snailmage.h);
std::vector<SDL_Rect> SpriteSnailmage::walk_animation_clips_south_west = Sprite::CalculateAnimationClips({ 0, 900, 900, 100 }, SpriteSnailmage::initial_texture_clip_snailmage.w,
	SpriteSnailmage::initial_texture_clip_snailmage.h);
std::vector<SDL_Rect> SpriteSnailmage::walk_animation_clips_west = Sprite::CalculateAnimationClips({ 0, 1100, 900, 100 }, SpriteSnailmage::initial_texture_clip_snailmage.w,
	SpriteSnailmage::initial_texture_clip_snailmage.h);
std::vector<SDL_Rect> SpriteSnailmage::walk_animation_clips_north_west = Sprite::CalculateAnimationClips({ 0, 1400, 900, 100 }, SpriteSnailmage::initial_texture_clip_snailmage.w,
	SpriteSnailmage::initial_texture_clip_snailmage.h);

//***********************************
//CONSTRUCTORS
//***********************************

SpriteSnailmage::SpriteSnailmage(Coordinates* ptr_my_center)
	: Sprite(TellTextureBank()->ptr_tex_snailmage, SpriteSnailmage::initial_texture_clip_snailmage, ptr_my_center)
{
	//printf("Constructor called for SpriteSnailmage\n");
}

//***********************************
//OVERLOADED VIRTUAL METHODS
//***********************************

void SpriteSnailmage::IdleAnimation()
{
	Directions direction = TellCurrentDirection();
	if (direction == north)
	{
		Sprite::PlayAnimation(idle_animation_clips_north);
	}
	else if (direction == north_east)
	{
		Sprite::PlayAnimation(idle_animation_clips_north_east);
	}
	else if (direction == east)
	{
		Sprite::PlayAnimation(idle_animation_clips_east);
	}
	else if (direction == south_east)
	{
		Sprite::PlayAnimation(idle_animation_clips_south_east);
	}
	else if (direction == south)
	{
		Sprite::PlayAnimation(idle_animation_clips_south);
	}
	else if (direction == south_west)
	{
		Sprite::PlayAnimation(idle_animation_clips_south_west);
	}
	else if (direction == west)
	{
		Sprite::PlayAnimation(idle_animation_clips_west);
	}
	else if (direction == north_west)
	{
		Sprite::PlayAnimation(idle_animation_clips_north_west);
	}
	else
	{
		printf("Unexpected direction in animation idle for snailmage.\n");
	}
}

void SpriteSnailmage::WalkAnimation()
{
	Directions direction = TellCurrentDirection();
	if (direction == north)
	{
		Sprite::PlayAnimation(walk_animation_clips_north);
	}
	else if (direction == north_east)
	{
		Sprite::PlayAnimation(walk_animation_clips_north_east);
	}
	else if (direction == east)
	{
		Sprite::PlayAnimation(walk_animation_clips_east);
	}
	else if (direction == south_east)
	{
		Sprite::PlayAnimation(walk_animation_clips_south_east);
	}
	else if (direction == south)
	{
		Sprite::PlayAnimation(walk_animation_clips_south);
	}
	else if (direction == south_west)
	{
		Sprite::PlayAnimation(walk_animation_clips_south_west);
	}
	else if (direction == west)
	{
		Sprite::PlayAnimation(walk_animation_clips_west);
	}
	else if (direction == north_west)
	{
		Sprite::PlayAnimation(walk_animation_clips_north_west);
	}
	else
	{
		printf("Unexpected direction in animation walk for snailmage.\n");
	}
}

