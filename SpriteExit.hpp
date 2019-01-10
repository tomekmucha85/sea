#ifndef SPRITEEXIT_HPP
#define SPRITEEXIT_HPP
#include <Sprite.hpp>
#include <Texture.hpp>
#include <TextureBank.hpp>

class SpriteExit : public Sprite
{
private:

	//###################
	//Variables
	//###################
	static SDL_Rect initial_texture_clip_vortex;

public:

	SpriteExit(Coordinates* ptr_my_center);
};

#endif //SPRITEEXIT_HPP
