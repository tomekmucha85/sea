#ifndef SPRITEBLACKSMOKE_HPP
#define SPRITEBLACKSMOKE_HPP
#include <Sprite.hpp>
#include <Texture.hpp>
#include <TextureBank.hpp>

class SpriteBlackSmoke : public Sprite
{
private:

	//###################
	//Variables
	//###################
	static SDL_Rect initial_texture_clip_black_smoke;

public:

	SpriteBlackSmoke(SDL_Rect* my_position = NULL);
	void SmokeAnimation();
};

#endif //SPRITEBLACKSMOKE_HPP
