#ifndef SPRITE_HPP
#define SPRITE_HPP
#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <vector>
#include <stdexcept>
#include <vector>
#include <TextureBank.hpp>
#include <Screen.hpp>
#include <VisualComponent.hpp>

//###################
//Data types
//###################

class Sprite : public VisualComponent
{
    private:

        //###################
        //Variables
        //###################
        int animation_frame = 0;

        //What part of the texture is used
        SDL_Rect texture_clip = {0,0,0,0};

		static TextureBank* ptr_texture_bank;

        //###################
        //Functions
        //###################

    public:

        //###################
        //Variables
        //###################
        SDL_Texture* texture = NULL;
        SDL_Point* center = NULL;
        SDL_RendererFlip flip = SDL_FLIP_NONE;
        double angle = 0;

        //###################
        //Functions
        //###################
		Sprite(SDL_Texture* ptr_my_texture, SDL_Rect my_texture_clip, Coordinates* ptr_my_center);

        void Render();
        std::vector <SDL_Rect> CalculateAnimationClips( SDL_Rect area, int clip_w, int clip_h);
        void PlayAnimation(std::vector <SDL_Rect> animation_clips, int delay_between_frames);
        void ResetAnimationFrame();

        //Animations (dummy virtual methods)
        virtual void WalkAnimation();
		virtual void SmokeAnimation();
		virtual void VortexAnimation();

        //Utilities
        SDL_Rect CheckTextureDimensions(SDL_Texture* ptr_my_texture);
        PreciseRect TellSpritePosition();
        SDL_Rect TellTextureClip();
		static void SetTextureBank(TextureBank* ptr_my_texture_bank);
		static TextureBank* TellTextureBank();
        void SetTexture(SDL_Texture* ref_my_texture);
        void SetTextureClip(SDL_Rect my_texture_clip);
        void SetPositionX(double new_x);
        void SetPositionY(double new_y);
        void SetPositionW(double new_w);
        void SetPositionH(double new_h);
		static SDL_Rect ConvertPreciseRectToSdlRect(PreciseRect my_rect);
};
#endif // SPRITE_HPP
