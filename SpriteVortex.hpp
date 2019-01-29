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

	SpriteVortex(Coordinates* ptr_my_center);
	void VortexAnimation();
	void IdleAnimation();
};

#endif //SPRITEVORTEX_HPP
