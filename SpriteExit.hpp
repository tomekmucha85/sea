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
	SDL_Rect texture_clip_door_closed =   { 0,      0, 96, 48 };
	SDL_Rect texture_clip_door_open =     { 96,     0, 96, 48 };
	SDL_Rect texture_clip_door_open_lit = { 96 * 2, 0 ,96, 48 };

public:

	SpriteExit(Coordinates* ptr_my_center);
	void SetClipAccordingToDoorState(DoorState my_state);
};

#endif //SPRITEEXIT_HPP
