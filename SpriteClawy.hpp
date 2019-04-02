#ifndef SPRITECLAWY_HPP
#define SPRITECLAWY_HPP
#include <Sprite.hpp>
#include <Texture.hpp>
#include <TextureBank.hpp>

class SpriteClawy : public Sprite
{
    private:

    //###################
    //Variables
    //###################
    static SDL_Rect initial_texture_clip_clawy;
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
    SpriteClawy(Coordinates* ptr_my_center);
    void WalkAnimation();
	void IdleAnimation();
};

#endif //SPRITECLAWY_HPP
