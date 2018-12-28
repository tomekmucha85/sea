#ifndef SPRITEVORTEX_HPP
#define SPRITEVORTEX_HPP
#include <Sprite.hpp>
#include <Texture.hpp>
#include <TextureBank.hpp>

class SpriteVortex : public Sprite
{
private:

	//###################
	//Variables
	//###################
	static SDL_Rect initial_texture_clip_vortex;

public:

	SpriteVortex(SDL_Rect* my_position = NULL);
	void VortexAnimation();
};

#endif //SPRITEVORTEX_HPP
