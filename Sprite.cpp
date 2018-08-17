#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <Sprite.hpp>
#include <SpriteClawy.hpp>
#include <SpriteBlackBox.hpp>
#include <Game.hpp>
#include <stdexcept>
#include <vector>

Sprite::Sprite()
{
    printf("Default constructor called for Sprite.\n");
}

Sprite::Sprite(SDL_Renderer* ref_my_renderer, TextureBank* ref_my_texture_bank)
{
    renderer = ref_my_renderer;
}

Sprite::Sprite(SDL_Renderer* ref_my_renderer, SDL_Texture* ref_my_texture, SDL_Rect* ref_my_texture_clip)
{
    renderer = ref_my_renderer;
    texture = ref_my_texture;
    if (ref_my_texture_clip)
    {
        texture_clip = *ref_my_texture_clip;
        position.w = texture_clip.w;
        position.h = texture_clip.h;
    }
    else
    {
        int w, h;
        SDL_QueryTexture(ref_my_texture, NULL, NULL, &w, &h);
        position.w = w;
        position.h = h;
    }

    position.x = 0;
    position.y = 0;
}

SDL_Rect Sprite::CheckTextureDimensions(SDL_Texture* ptr_my_texture)
{
    int w,h;
    printf("Texture address is %p", ptr_my_texture);
    SDL_QueryTexture(ptr_my_texture, NULL, NULL, &w, &h);
    SDL_Rect result = {0,0,w,h};
    return result;
}

void Sprite::SetTexture(SDL_Texture* ref_my_texture)
{
    texture = ref_my_texture;
}

void Sprite::SetTextureClip(SDL_Rect my_texture_clip)
{
    texture_clip = my_texture_clip;
}

void Sprite::SetPosition(SDL_Rect my_position)
{
    position = my_position;
}

void Sprite::SetPositionX(int new_x)
{
    position.x = new_x;
}

void Sprite::SetPositionY(int new_y)
{
    position.y = new_y;
}

void Sprite::SetPositionW(int new_w)
{
    position.w = new_w;
}

void Sprite::SetPositionH(int new_h)
{
    position.h = new_h;
}

void Sprite::Move(int velocity_x, int velocity_y)
{
    position.x += velocity_x;
    position.y += velocity_y;
    velocity_x = 0;
    velocity_y = 0;
}

void Sprite::Render()
{
    if (texture_clip.w != 0 && texture_clip.h != 0)
    //If texture clip dimensions were set, apply them.
    {
        SDL_RenderCopyEx(renderer, texture, &texture_clip, &position, angle, center, flip);
    }
    else
    //If texture clip dimensions were not set, show whole texture.
    {
        SDL_RenderCopyEx(renderer, texture, NULL, &position, angle, center, flip);
    }
}

std::vector <SDL_Rect> Sprite::CalculateAnimationClips( SDL_Rect area, int clip_w, int clip_h)
{
    //Protection against arguments == zero
    if (area.w * area.h * clip_w * clip_h == 0)
    {
        throw std::invalid_argument("Incorrect arguments passed to Sprite::CalculateAnimationClips(). 0 clips would be generated.");
    }
    //How many elements will array hold?
    int elem_count = (area.w / clip_w) * (area.h / clip_h);
    //printf("Animation element count is: %d\n", elem_count);

    //Declare the array. Vector is used for getting size in an easy way later on.
    //SDL_Rect * clips = new SDL_Rect[elem_count];
    std::vector<SDL_Rect> clips(elem_count);
    int current_x = 0;
    int current_y = 0;
    //Filling in the array of texture clips
    for (int i = 0; i < elem_count; i++)
    {

        clips[i] = {current_x, current_y, clip_w, clip_h};
        if (current_x < (area.w - clip_w))
        {
            current_x += clip_w;
        }
        else
        {
            current_x = 0;
            current_y += clip_h;
        }
    }
    return clips;
}

void Sprite::PlayAnimation(std::vector <SDL_Rect> animation_clips, int delay_between_frames)
{
    int animation_frames_count = animation_clips.size();
    texture_clip = animation_clips[animation_frame/delay_between_frames];
    Sprite::Render();
    animation_frame++;
    if( (animation_frame / delay_between_frames) >= animation_frames_count )
    {
        animation_frame = 0;
    }
}

void Sprite::ResetAnimationFrame()
{
    animation_frame = 0;
}

//#####################
//Tell methods
//#####################

SDL_Rect Sprite::TellSpritePosition()
{
    return position;
}

SDL_Rect Sprite::TellTextureClip()
{
    return texture_clip;
}

//#####################
//Dummy virtual methods
//#####################

void Sprite::WalkAnimation()
{
    printf("Walk animation called for Sprite\n");
}

//#####################
//Method for spawning sprites
//#####################

Sprite* Sprite::CreateSprite(SpriteType desired_type, TextureBank* ptr_texture_bank, SDL_Rect* ptr_position)
{
    if (desired_type == clawy)
    {
        printf("Requested clawy object.\n");
        SpriteClawy* ptr_my_clawy = new SpriteClawy(Game::ptr_screen->renderer,ptr_texture_bank,ptr_position);
        return ptr_my_clawy;
    }
    else if (desired_type == box)
    {
        printf("Requested box object.\n");
        SpriteBlackBox* ptr_my_box = new SpriteBlackBox(Game::ptr_screen->renderer,ptr_texture_bank,ptr_position);
        return ptr_my_box;
    }
    else
    {
        printf("Requested some other object.\n");
    }
}

