#ifndef SPRITEBLACKBOX_HPP
#define SPRITEBLACKBOX_HPP
#include <Sprite.hpp>
#include <Texture.hpp>
#include <TextureBank.hpp>

class SpriteBlackBox : public Sprite
{
    private:

    //###################
    //Variables
    //###################
	    static SDL_Rect initial_texture_clip_black_box;
		SDL_Rect texture_clip_wall_bottom                      = { 0,0,48,48 };
		SDL_Rect texture_clip_wall_corner_bottom_left_inward   = { 48,0,48,48 };
		SDL_Rect texture_clip_wall_corner_bottom_left_outward  = { 48*2,0,48,48 };
		SDL_Rect texture_clip_wall_corner_bottom_right_inward  = { 48*3,0,48,48 };
		SDL_Rect texture_clip_wall_corner_bottom_right_outward = { 48*4,0,48,48 };
		SDL_Rect texture_clip_wall_left                        = { 48*5,0,48,48 };
		SDL_Rect texture_clip_wall_right                       = { 48*6,0,48,48 };
		SDL_Rect texture_clip_wall_top                         = { 48*7,0,48,48 };
		SDL_Rect texture_clip_wall_top_ending_left             = { 48*8,0,48,48 };
		SDL_Rect texture_clip_wall_top_ending_right            = { 48*9,0,48,48 };
		SDL_Rect texture_clip_wall_corner_upper_left_inward    = { 48*10,0,48,48 };
		SDL_Rect texture_clip_wall_corner_upper_left_outward   = { 48*11,0,48,48 };
		SDL_Rect texture_clip_wall_corner_upper_right_inward   = { 48*12,0,48,48 };
		SDL_Rect texture_clip_wall_corner_upper_right_outward  = { 48*13,0,48,48 };

    public:

        SpriteBlackBox(Coordinates* ptr_my_center);
		void SetClipAccordingToWallType(WallType my_type);
};

#endif //SPRITEBLACKBOX_HPP
