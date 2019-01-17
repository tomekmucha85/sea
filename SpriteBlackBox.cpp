#include <SpriteBlackBox.hpp>

//***********************************
//DEFINITIONS OF STATIC CLASS MEMBERS
//***********************************

SDL_Rect SpriteBlackBox::initial_texture_clip_black_box = { 336,0,48,48 };

//***********************************
//CONSTRUCTORS
//***********************************

SpriteBlackBox::SpriteBlackBox(Coordinates* ptr_my_center)
	: Sprite(TellTextureBank()->ptr_tex_wall, SpriteBlackBox::initial_texture_clip_black_box, ptr_my_center)
{
	//printf("Constructor called for SpriteBlackBox\n");
}


//***********************************
//OVERLOADED METHODS
//***********************************

void SpriteBlackBox::SetClipAccordingToWallType(WallType my_type)
{
	if (my_type == wall_bottom)
	{
		SetTextureClip(texture_clip_wall_bottom);
	}
	else if (my_type == wall_corner_bottom_left_inward)
	{
		SetTextureClip(texture_clip_wall_corner_bottom_left_inward);
	}
	else if (my_type == wall_corner_bottom_left_outward)
	{
		SetTextureClip(texture_clip_wall_corner_bottom_left_outward);
	}
	else if (my_type == wall_corner_bottom_right_inward)
	{
		SetTextureClip(texture_clip_wall_corner_bottom_right_inward);
	}
	else if (my_type == wall_corner_bottom_right_outward)
	{
		SetTextureClip(texture_clip_wall_corner_bottom_right_outward);
	}
	else if (my_type == wall_corner_upper_left_inward)
	{
		SetTextureClip(texture_clip_wall_corner_upper_left_inward);
	}
	else if (my_type == wall_corner_upper_left_outward)
	{
		SetTextureClip(texture_clip_wall_corner_upper_left_outward);
	}
	else if (my_type == wall_corner_upper_right_inward)
	{
		SetTextureClip(texture_clip_wall_corner_upper_right_inward);
	}
	else if (my_type == wall_corner_upper_right_outward)
	{
		SetTextureClip(texture_clip_wall_corner_upper_right_outward);
	}
	else if (my_type == wall_left)
	{
		SetTextureClip(texture_clip_wall_left);
	}
	else if (my_type == wall_right)
	{
		SetTextureClip(texture_clip_wall_right);
	}
	else if (my_type == wall_top)
	{
		SetTextureClip(texture_clip_wall_top);
	}
	else if (my_type == wall_top_ending_left)
	{
		SetTextureClip(texture_clip_wall_top_ending_left);
	}
	else if (my_type == wall_top_ending_right)
	{
		SetTextureClip(texture_clip_wall_top_ending_right);
	}
	else
	{
		printf("Unknown wall type!\n");
		throw std::invalid_argument("Unknown wall type.\n");
	}
}

