#include <SpriteClawy.hpp>

//***********************************
//DEFINITIONS OF STATIC CLASS MEMBERS
//***********************************

SDL_Rect SpriteClawy::initial_texture_clip_clawy = {0,0,100,100};

std::vector<SDL_Rect> SpriteClawy::idle_animation_clips_north = Sprite::CalculateAnimationClips({0,500,900,100}, SpriteClawy::initial_texture_clip_clawy.w,
	SpriteClawy::initial_texture_clip_clawy.h);
std::vector<SDL_Rect> SpriteClawy::idle_animation_clips_north_east = Sprite::CalculateAnimationClips({ 0,700,900,100 }, SpriteClawy::initial_texture_clip_clawy.w,
	SpriteClawy::initial_texture_clip_clawy.h);
std::vector<SDL_Rect> SpriteClawy::idle_animation_clips_east = Sprite::CalculateAnimationClips({ 0,400,900,100 }, SpriteClawy::initial_texture_clip_clawy.w,
	SpriteClawy::initial_texture_clip_clawy.h);
std::vector<SDL_Rect> SpriteClawy::idle_animation_clips_south_east = Sprite::CalculateAnimationClips({ 0,200,900,100 }, SpriteClawy::initial_texture_clip_clawy.w,
	SpriteClawy::initial_texture_clip_clawy.h);
std::vector<SDL_Rect> SpriteClawy::idle_animation_clips_south = Sprite::CalculateAnimationClips({ 0,0,900,100 }, SpriteClawy::initial_texture_clip_clawy.w,
	SpriteClawy::initial_texture_clip_clawy.h);
std::vector<SDL_Rect> SpriteClawy::idle_animation_clips_south_west = Sprite::CalculateAnimationClips({ 0,100,900,100 }, SpriteClawy::initial_texture_clip_clawy.w,
	SpriteClawy::initial_texture_clip_clawy.h);
std::vector<SDL_Rect> SpriteClawy::idle_animation_clips_west = Sprite::CalculateAnimationClips({ 0,300,900,100 }, SpriteClawy::initial_texture_clip_clawy.w,
	SpriteClawy::initial_texture_clip_clawy.h);
std::vector<SDL_Rect> SpriteClawy::idle_animation_clips_north_west = Sprite::CalculateAnimationClips({ 0,600,900,100 }, SpriteClawy::initial_texture_clip_clawy.w,
	SpriteClawy::initial_texture_clip_clawy.h);

std::vector<SDL_Rect> SpriteClawy::walk_animation_clips_north = Sprite::CalculateAnimationClips({ 0, 1300, 1000, 100 }, SpriteClawy::initial_texture_clip_clawy.w,
	SpriteClawy::initial_texture_clip_clawy.h);
std::vector<SDL_Rect> SpriteClawy::walk_animation_clips_north_east = Sprite::CalculateAnimationClips({ 0, 1500, 1000, 100 }, SpriteClawy::initial_texture_clip_clawy.w,
	SpriteClawy::initial_texture_clip_clawy.h);
std::vector<SDL_Rect> SpriteClawy::walk_animation_clips_east = Sprite::CalculateAnimationClips({ 0, 1200, 1000, 100 }, SpriteClawy::initial_texture_clip_clawy.w,
	SpriteClawy::initial_texture_clip_clawy.h);
std::vector<SDL_Rect> SpriteClawy::walk_animation_clips_south_east = Sprite::CalculateAnimationClips({ 0, 1000, 1000, 100 }, SpriteClawy::initial_texture_clip_clawy.w,
	SpriteClawy::initial_texture_clip_clawy.h);
std::vector<SDL_Rect> SpriteClawy::walk_animation_clips_south = Sprite::CalculateAnimationClips({0, 800, 1000, 100}, SpriteClawy::initial_texture_clip_clawy.w,
	SpriteClawy::initial_texture_clip_clawy.h);
std::vector<SDL_Rect> SpriteClawy::walk_animation_clips_south_west = Sprite::CalculateAnimationClips({ 0, 900, 1000, 100 }, SpriteClawy::initial_texture_clip_clawy.w,
	SpriteClawy::initial_texture_clip_clawy.h);
std::vector<SDL_Rect> SpriteClawy::walk_animation_clips_west = Sprite::CalculateAnimationClips({ 0, 1100, 1000, 100 }, SpriteClawy::initial_texture_clip_clawy.w,
	SpriteClawy::initial_texture_clip_clawy.h);
std::vector<SDL_Rect> SpriteClawy::walk_animation_clips_north_west = Sprite::CalculateAnimationClips({ 0, 1400, 1000, 100 }, SpriteClawy::initial_texture_clip_clawy.w,
	SpriteClawy::initial_texture_clip_clawy.h);

std::vector<SDL_Rect> SpriteClawy::attack_animation_clips_north = Sprite::CalculateAnimationClips({ 0, 2100, 700, 100 }, SpriteClawy::initial_texture_clip_clawy.w,
	SpriteClawy::initial_texture_clip_clawy.h);
std::vector<SDL_Rect> SpriteClawy::attack_animation_clips_north_east = Sprite::CalculateAnimationClips({ 0, 2300, 700, 100 }, SpriteClawy::initial_texture_clip_clawy.w,
	SpriteClawy::initial_texture_clip_clawy.h);
std::vector<SDL_Rect> SpriteClawy::attack_animation_clips_east = Sprite::CalculateAnimationClips({ 0, 2000, 700, 100 }, SpriteClawy::initial_texture_clip_clawy.w,
	SpriteClawy::initial_texture_clip_clawy.h);
std::vector<SDL_Rect> SpriteClawy::attack_animation_clips_south_east = Sprite::CalculateAnimationClips({ 0, 1800, 700, 100 }, SpriteClawy::initial_texture_clip_clawy.w,
	SpriteClawy::initial_texture_clip_clawy.h);
std::vector<SDL_Rect> SpriteClawy::attack_animation_clips_south = Sprite::CalculateAnimationClips({ 0, 1600, 700, 100 }, SpriteClawy::initial_texture_clip_clawy.w,
	SpriteClawy::initial_texture_clip_clawy.h);
std::vector<SDL_Rect> SpriteClawy::attack_animation_clips_south_west = Sprite::CalculateAnimationClips({ 0, 1700, 700, 100 }, SpriteClawy::initial_texture_clip_clawy.w,
	SpriteClawy::initial_texture_clip_clawy.h);
std::vector<SDL_Rect> SpriteClawy::attack_animation_clips_west = Sprite::CalculateAnimationClips({ 0, 1900, 700, 100 }, SpriteClawy::initial_texture_clip_clawy.w,
	SpriteClawy::initial_texture_clip_clawy.h);
std::vector<SDL_Rect> SpriteClawy::attack_animation_clips_north_west = Sprite::CalculateAnimationClips({ 0, 2200, 700, 100 }, SpriteClawy::initial_texture_clip_clawy.w,
	SpriteClawy::initial_texture_clip_clawy.h);

//***********************************
//CONSTRUCTORS
//***********************************

SpriteClawy::SpriteClawy(Coordinates* ptr_my_center)
	: Sprite(TellTextureBank()->ptr_tex_clawy, SpriteClawy::initial_texture_clip_clawy, ptr_my_center)
{
    //printf("Constructor called for SpriteClawy\n");
}

//***********************************
//OVERLOADED VIRTUAL METHODS
//***********************************

void SpriteClawy::IdleAnimation(bool* ptr_did_animation_reach_last_frame)
{
	Directions direction = TellCurrentDirection();
	if (direction == north)
	{
		Sprite::PlayAnimation(idle_animation_clips_north,
			ptr_did_animation_reach_last_frame);
	}
	else if (direction == north_east)
	{
		Sprite::PlayAnimation(idle_animation_clips_north_east,
			ptr_did_animation_reach_last_frame);
	}
	else if (direction == east)
	{
		Sprite::PlayAnimation(idle_animation_clips_east,
			ptr_did_animation_reach_last_frame);
	}
	else if (direction == south_east)
	{
		Sprite::PlayAnimation(idle_animation_clips_south_east,
			ptr_did_animation_reach_last_frame);
	}
	else if (direction == south)
	{
		Sprite::PlayAnimation(idle_animation_clips_south,
			ptr_did_animation_reach_last_frame);
	}
	else if (direction == south_west)
	{
		Sprite::PlayAnimation(idle_animation_clips_south_west,
			ptr_did_animation_reach_last_frame);
	}
	else if (direction == west)
	{
		Sprite::PlayAnimation(idle_animation_clips_west,
			ptr_did_animation_reach_last_frame);
	}
	else if (direction == north_west)
	{
		Sprite::PlayAnimation(idle_animation_clips_north_west,
			ptr_did_animation_reach_last_frame);
	}
	else
	{
		printf("Unexpected direction in animation idle for clawy.\n");
	}
}

void SpriteClawy::WalkAnimation(bool* ptr_did_animation_reach_last_frame)
{
	Directions direction = TellCurrentDirection();
	if (direction == north)
	{
		Sprite::PlayAnimation(walk_animation_clips_north,
			ptr_did_animation_reach_last_frame);
	}
	else if (direction == north_east)
	{
		Sprite::PlayAnimation(walk_animation_clips_north_east,
			ptr_did_animation_reach_last_frame);
	}
	else if (direction == east)
	{
		Sprite::PlayAnimation(walk_animation_clips_east,
			ptr_did_animation_reach_last_frame);
	}
	else if (direction == south_east)
	{
		Sprite::PlayAnimation(walk_animation_clips_south_east,
			ptr_did_animation_reach_last_frame);
	}
	else if (direction == south)
	{
		Sprite::PlayAnimation(walk_animation_clips_south,
			ptr_did_animation_reach_last_frame);
	}
	else if (direction == south_west)
	{
		Sprite::PlayAnimation(walk_animation_clips_south_west,
			ptr_did_animation_reach_last_frame);
	}
	else if (direction == west)
	{
		Sprite::PlayAnimation(walk_animation_clips_west,
			ptr_did_animation_reach_last_frame);
	}
	else if (direction == north_west)
	{
		Sprite::PlayAnimation(walk_animation_clips_north_west,
			ptr_did_animation_reach_last_frame);
	}
	else
	{
		printf("Unexpected direction in animation walk for clawy.\n");
	}
}

void SpriteClawy::AttackAnimation(bool* ptr_did_animation_reach_last_frame)
{
	Directions direction = TellCurrentDirection();
	if (direction == north)
	{
		Sprite::PlayAnimation(attack_animation_clips_north,
			ptr_did_animation_reach_last_frame);
	}
	else if (direction == north_east)
	{
		Sprite::PlayAnimation(attack_animation_clips_north_east,
			ptr_did_animation_reach_last_frame);
	}
	else if (direction == east)
	{
		Sprite::PlayAnimation(attack_animation_clips_east,
			ptr_did_animation_reach_last_frame);
	}
	else if (direction == south_east)
	{
		Sprite::PlayAnimation(attack_animation_clips_south_east,
			ptr_did_animation_reach_last_frame);
	}
	else if (direction == south)
	{
		Sprite::PlayAnimation(attack_animation_clips_south,
			ptr_did_animation_reach_last_frame);
	}
	else if (direction == south_west)
	{
		Sprite::PlayAnimation(attack_animation_clips_south_west,
			ptr_did_animation_reach_last_frame);
	}
	else if (direction == west)
	{
		Sprite::PlayAnimation(attack_animation_clips_west,
			ptr_did_animation_reach_last_frame);
	}
	else if (direction == north_west)
	{
		Sprite::PlayAnimation(attack_animation_clips_north_west,
			ptr_did_animation_reach_last_frame);
	}
	else
	{
		printf("Unexpected direction in animation attack for clawy.\n");
	}
}
