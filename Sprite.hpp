#ifndef SPRITE_HPP
#define SPRITE_HPP
#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <vector>
#include <stdexcept>
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
		Directions current_direction = north;

        //What part of the texture is used
        SDL_Rect texture_clip = {0,0,0,0};
		static TextureBank* ptr_texture_bank;
		AnimationType current_animation = anim_none;

        //###################
        //Functions
        //###################

    public:

        //###################
        //Variables
        //###################
        SDL_Texture* texture = NULL;
        SDL_RendererFlip flip = SDL_FLIP_NONE;

        //###################
        //Functions
        //###################
		Sprite(SDL_Texture* ptr_my_texture, SDL_Rect my_texture_clip, Coordinates* ptr_my_center);
		void Move(double step_x, double step_y);
		void SetAngleDegrees(int my_angle);
		void TurnByAngleDegrees(int my_angle);
		void SetDirectionFromEightPossibilities(double angle_degrees);
		Directions TellCurrentDirection();
        void Render();
        void PlayAnimation(std::vector <SDL_Rect> animation_clips, 
			int delay_between_frames=3,
			bool* ptr_has_animation_reached_last_frame=nullptr);
        void ResetAnimationFrame();

        //Animations (including dummy virtual methods)
		void PlayCurrentAnimation();
		void SetCurrentAnimation(AnimationType my_animation_type);
		virtual void IdleAnimation();
        virtual void WalkAnimation();
		virtual void SmokeAnimation();
		virtual void VortexAnimation();
		//Other virtual methods
		virtual void SetClipAccordingToWallType(WallType my_type);

        //Utilities
        SDL_Rect CheckTextureDimensions(SDL_Texture* ptr_my_texture);
        PreciseRect TellSpritePosition();
        SDL_Rect TellTextureClip();
		static void SetTextureBank(TextureBank* ptr_my_texture_bank);
		static TextureBank* TellTextureBank();
		static std::vector <SDL_Rect> CalculateAnimationClips(SDL_Rect area, int clip_w, int clip_h);
        void SetTexture(SDL_Texture* ref_my_texture);
        void SetTextureClip(SDL_Rect my_texture_clip);
		void SetCenter(Coordinates my_center);
		PreciseRect CalculatePositionAroundCenter();
};
#endif // SPRITE_HPP
