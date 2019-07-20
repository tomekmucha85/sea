#ifndef SPRITEIMPLOSION_HPP
#define SPRITEIMPLOSION_HPP
#include <Sprite.hpp>
#include <Texture.hpp>
#include <TextureBank.hpp>

class SpriteImplosion : public Sprite
{
private:

	//###################
	//Variables
	//###################
	static SDL_Rect initial_texture_clip_implosion;
	static std::vector<SDL_Rect> implosion_animation_clips;

public:

	SpriteImplosion(Coordinates* ptr_my_center);
	void ImplosionAnimation(bool* ptr_did_animation_reach_last_frame = nullptr);
	void IdleAnimation(bool* ptr_did_animation_reach_last_frame = nullptr);
};

#endif //SPRITEIMPLOSION_HPP
