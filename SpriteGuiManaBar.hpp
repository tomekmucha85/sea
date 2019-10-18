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
	SDL_Rect texture_clip_no_charge = { 0,0,175,150 };
	SDL_Rect texture_clip_one_charge = { 175,0,175,150 };
	SDL_Rect texture_clip_two_charges = { 175*2,0,175,150 };

public:

	SpriteGuiManaBar(Coordinates* ptr_my_center);
	void SetTextureClipWithCharges(int charges);
};

#endif //SPRITEGUIMANABAR_HPP
