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

	SpriteBlackSmoke(Coordinates* ptr_my_center);
	void SmokeAnimation();
};

#endif //SPRITEBLACKSMOKE_HPP
