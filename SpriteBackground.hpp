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
	static SDL_Rect initial_texture_clip_background;

    public:

    SpriteBackground(Coordinates* ptr_my_center);

};

#endif //SPRITEBGROUND_HPP
