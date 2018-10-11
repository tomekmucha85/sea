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
    static SDL_Rect initial_texture_clip_clawy;

    public:
	//###################
	//Variables
	//###################

	//###################
	//Functions
	//###################
    SpriteClawy(SDL_Rect* my_position = NULL);
    void WalkAnimation();
};

#endif //SPRITECLAWY_HPP
