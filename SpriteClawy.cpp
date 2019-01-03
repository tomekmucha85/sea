#include <SpriteClawy.hpp>

//***********************************
//DEFINITIONS OF STATIC CLASS MEMBERS
//***********************************

SDL_Rect SpriteClawy::initial_texture_clip_clawy = {0,0,45,45};

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

void SpriteClawy::WalkAnimation()
{
    SDL_Rect walk_animation_area = {0,0,90,45};
    SDL_Rect current_texture_clip = TellTextureClip();
    int timeout_between_frames = 10;
    int clip_w = current_texture_clip.w;
    int clip_h = current_texture_clip.h;
    std::vector <SDL_Rect> walk_clips = Sprite::CalculateAnimationClips(walk_animation_area, clip_w, clip_h);
    Sprite::PlayAnimation(walk_clips, timeout_between_frames);
//    printf("Walk_clip 0: x: %d y:%d w:%d h:%d\n", walk_clips[0].x, walk_clips[0].y, walk_clips[0].w, walk_clips[0].h);
//    printf("Walk_clip 1: x: %d y:%d w:%d h:%d\n", walk_clips[1].x, walk_clips[1].y, walk_clips[1].w, walk_clips[1].h);
//    printf("Current clip: x: %d y:%d w:%d h:%d\n", texture_clip.x, texture_clip.y, texture_clip.w, texture_clip.h);
//    printf("Animation frame is: %d\n", animation_frame);
}
