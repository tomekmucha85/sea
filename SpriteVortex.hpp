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
	static std::vector<SDL_Rect> vortex_animation_clips;

public:

	SpriteVortex(Coordinates* ptr_my_center);
	void VortexAnimation(bool* ptr_did_animation_reach_last_frame = nullptr);
	void IdleAnimation(bool* ptr_did_animation_reach_last_frame = nullptr);
};

#endif //SPRITEVORTEX_HPP
