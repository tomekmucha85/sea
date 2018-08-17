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
    SDL_Rect texture_clip_black_box = {0,0,40,40};

    public:

    SpriteBlackBox(SDL_Renderer* ptr_my_renderer, TextureBank* ptr_my_texture_bank, SDL_Rect* my_position = NULL);

};

#endif //SPRITEBLACKBOX_HPP
