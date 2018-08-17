#ifndef SPRITEBGROUND_HPP
#define SPRITEBGROUND_HPP
#include <Sprite.hpp>
#include <Texture.hpp>
#include <TextureBank.hpp>

class SpriteBackground : public Sprite
{
    private:

    //###################
    //Variables
    //###################
    SDL_Rect texture_clip_background = {0,0,0,0};

    public:

    SpriteBackground(SDL_Renderer* ptr_my_renderer, TextureBank* ptr_my_texture_bank, SDL_Rect* my_position = NULL);

};

#endif //SPRITEBGROUND_HPP
