#include <SpriteBlackBox.hpp>

//***********************************
//DEFINITIONS OF STATIC CLASS MEMBERS
//***********************************

SDL_Rect SpriteBlackBox::initial_texture_clip_black_box = { 0,0,64,64 };
const SDL_Rect SpriteBlackBox::texture_clip_wall_a = { 0,0,BOX_WIDTH, BOX_HEIGHT };
const SDL_Rect SpriteBlackBox::texture_clip_wall_b = { BOX_WIDTH,0,BOX_WIDTH, BOX_HEIGHT };
const SDL_Rect SpriteBlackBox::texture_clip_wall_c = { BOX_WIDTH * 2,0,BOX_WIDTH, BOX_HEIGHT };
const SDL_Rect SpriteBlackBox::texture_clip_wall_d = { BOX_WIDTH * 3,0,BOX_WIDTH, BOX_HEIGHT };
const SDL_Rect SpriteBlackBox::texture_clip_wall_e = { BOX_WIDTH * 4,0,BOX_WIDTH, BOX_HEIGHT };
const SDL_Rect SpriteBlackBox::texture_clip_wall_f = { BOX_WIDTH * 5,0,BOX_WIDTH, BOX_HEIGHT };
const SDL_Rect SpriteBlackBox::texture_clip_wall_g = { BOX_WIDTH * 6,0,BOX_WIDTH, BOX_HEIGHT };
const SDL_Rect SpriteBlackBox::texture_clip_wall_h = { BOX_WIDTH * 7,0,BOX_WIDTH, BOX_HEIGHT };
const SDL_Rect SpriteBlackBox::all_texture_clips[SpriteBlackBox::NUMBER_OF_WALL_CLIPS] = 
{ 
	texture_clip_wall_a,
	texture_clip_wall_b,
	texture_clip_wall_c,
	texture_clip_wall_d,
	texture_clip_wall_e,
	texture_clip_wall_f,
	texture_clip_wall_g,
	texture_clip_wall_h 
};



//***********************************
//CONSTRUCTORS
//***********************************

SpriteBlackBox::SpriteBlackBox(Coordinates* ptr_my_center)
	: Sprite(TellTextureBank()->ptr_tex_wall, SelectRandomClip(), ptr_my_center)
{
	//printf("Constructor called for SpriteBlackBox\n");
}



//***********************************
//* FUNCTIONS
//***********************************

SDL_Rect SpriteBlackBox::SelectRandomClip()
{
	unsigned int chosen_clip = rand() % NUMBER_OF_WALL_CLIPS;
	return all_texture_clips[chosen_clip];
}


//***********************************
//OVERLOADED METHODS
//***********************************

/*void SpriteBlackBox::SetClipAccordingToWallType(WallType my_type)
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
}*/

