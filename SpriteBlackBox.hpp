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

    public:

        SpriteBlackBox(Coordinates* ptr_my_center);

};

#endif //SPRITEBLACKBOX_HPP
