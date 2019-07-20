#ifndef SPRITESNAILMAGE_HPP
#define SPRITESNAILMAGE_HPP
#include <Sprite.hpp>
#include <Texture.hpp>
#include <TextureBank.hpp>

class SpriteSnailmage : public Sprite
{
private:

	//###################
	//Variables
	//###################
	static SDL_Rect initial_texture_clip_snailmage;
	static std::vector<SDL_Rect> idle_animation_clips_north;
	static std::vector<SDL_Rect> idle_animation_clips_north_east;
	static std::vector<SDL_Rect> idle_animation_clips_east;
	static std::vector<SDL_Rect> idle_animation_clips_south_east;
	static std::vector<SDL_Rect> idle_animation_clips_south;
	static std::vector<SDL_Rect> idle_animation_clips_south_west;
	static std::vector<SDL_Rect> idle_animation_clips_west;
	static std::vector<SDL_Rect> idle_animation_clips_north_west;
	static std::vector<SDL_Rect> walk_animation_clips_north;
	static std::vector<SDL_Rect> walk_animation_clips_north_east;
	static std::vector<SDL_Rect> walk_animation_clips_east;
	static std::vector<SDL_Rect> walk_animation_clips_south_east;
	static std::vector<SDL_Rect> walk_animation_clips_south;
	static std::vector<SDL_Rect> walk_animation_clips_south_west;
	static std::vector<SDL_Rect> walk_animation_clips_west;
	static std::vector<SDL_Rect> walk_animation_clips_north_west;

public:
	//###################
	//Variables
	//###################

	//###################
	//Functions
	//###################
	SpriteSnailmage(Coordinates* ptr_my_center);
	void WalkAnimation(bool* ptr_did_animation_reach_last_frame = nullptr);
	void IdleAnimation(bool* ptr_did_animation_reach_last_frame = nullptr);
};

#endif //SPRITESNAILMAGE_HPP
