#ifndef SPRITEGUIMANABAR_HPP
#define SPRITEGUIMANABAR_HPP
#include <Sprite.hpp>
#include <Texture.hpp>
#include <TextureBank.hpp>

class SpriteGuiManaBar : public Sprite
{
private:

	//###################
	//Variables
	//###################
	static SDL_Rect initial_texture_clip_gui_mana_bar;
	SDL_Rect texture_clip_no_charge = { 0,0,80,9 };
	SDL_Rect texture_clip_one_charge = { 80,0,80,9 };
	SDL_Rect texture_clip_two_charges = { 160,0,80,9 };

public:

	SpriteGuiManaBar(Coordinates* ptr_my_center);
	void SetTextureClipWithCharges(int charges);
};

#endif //SPRITEGUIMANABAR_HPP
