#include <SpriteGuiManaBar.hpp>

//***********************************
//DEFINITIONS OF STATIC CLASS MEMBERS
//***********************************

SDL_Rect SpriteGuiManaBar::initial_texture_clip_gui_mana_bar = { 0,0,175,150 };

//***********************************
//CONSTRUCTORS
//***********************************

SpriteGuiManaBar::SpriteGuiManaBar(Coordinates* ptr_my_center)
	: Sprite(TellTextureBank()->ptr_tex_gui_mana_bar, SpriteGuiManaBar::initial_texture_clip_gui_mana_bar, ptr_my_center)
{
	;
}

//***********************************
//SPRITE SPECIFIC METHODS
//***********************************

void SpriteGuiManaBar::SetTextureClipWithCharges(int charges)
{
	if (charges <= 0)
	{
		SetTextureClip(texture_clip_no_charge);
	}
	else if (charges == 1)
	{
		SetTextureClip(texture_clip_one_charge);
	}
	else
	{
		SetTextureClip(texture_clip_two_charges);
	}
}


//***********************************
//OVERLOADED VIRTUAL METHODS
//***********************************

