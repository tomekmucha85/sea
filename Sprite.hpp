#ifndef SPRITE_HPP
#define SPRITE_HPP
#include <vector>
#include <TextureBank.hpp>

//###################
//Data types
//###################

enum SpriteType   {clawy, background, box};

class Sprite
{
    private:

        //###################
        //Variables
        //###################
        SDL_Renderer* renderer;
        int animation_frame = 0;

        //What part of the texture is used
        SDL_Rect texture_clip = {0,0,0,0};

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
        //int velocity_val = 5;

        //Where the sprite is rendered on screen
        ////Move to private!
        SDL_Rect position = {0,0,0,0};
        //###################
        //Functions
        //###################
        Sprite(SDL_Renderer* ref_my_renderer, SDL_Texture* ref_my_texture, SDL_Rect* ref_my_texture_clip);
        Sprite(SDL_Renderer* ref_my_renderer, TextureBank* ref_my_texture_bank);
        Sprite();
        void Move(int velocity_x, int velocity_y);
        void Render();
        std::vector <SDL_Rect> CalculateAnimationClips( SDL_Rect area, int clip_w, int clip_h);
        void PlayAnimation(std::vector <SDL_Rect> animation_clips, int delay_between_frames);
        void ResetAnimationFrame();

        //Animations (dummy virtual methods)
        virtual void WalkAnimation();

        //Utilities
        SDL_Rect CheckTextureDimensions(SDL_Texture* ptr_my_texture);
        SDL_Rect TellSpritePosition();
        SDL_Rect TellTextureClip();
        void SetTexture(SDL_Texture* ref_my_texture);
        void SetTextureClip(SDL_Rect my_texture_clip);
        void SetPosition(SDL_Rect my_position);
        void SetPositionX(int new_x);
        void SetPositionY(int new_y);
        void SetPositionW(int new_w);
        void SetPositionH(int new_h);

        //Method for spawning sprites
        static Sprite* CreateSprite(SpriteType desired_type, TextureBank* ptr_texture_bank, SDL_Rect* ptr_position);

};
#endif // SPRITE_HPP
