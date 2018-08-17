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
    SDL_Rect texture_clip_clawy = {0,0,45,45};

    public:

    SpriteClawy(SDL_Renderer* ptr_my_renderer, TextureBank* ptr_my_texture_bank, SDL_Rect* my_position = NULL);
    void WalkAnimation();
};

#endif //SPRITECLAWY_HPP
