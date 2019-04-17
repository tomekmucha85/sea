#include <Sprite.hpp>

//***********************************
//DEFINITIONS OF STATIC CLASS MEMBERS
//***********************************

TextureBank* Sprite::ptr_texture_bank = nullptr;


/*SetPositionX(static_cast<int>(center.x - (current_texture_clip.w / 2)));
SetPositionY(static_cast<int>(center.y - (current_texture_clip.h / 2)));
SetPositionW(current_texture_clip.w);
SetPositionH(current_texture_clip.h);*/


//***********************************
//CONSTRUCTOR
//***********************************

Sprite::Sprite(SDL_Texture* ptr_my_texture, SDL_Rect my_texture_clip, Coordinates* ptr_my_center) : VisualComponent(ptr_my_center)
{
	SetMyType(visco_sprite);
	//Constructor building sprite around specified center coordinates
	//#TODO - validate arguments?
	SetTexture(ptr_my_texture);
	//printf("Texture set.");
	if (my_texture_clip.w == 0 && my_texture_clip.h == 0)
	{
		//If there are no width and height params set for texture clip, whole texture is shown.
		//#TODO pass texture clip by pointer.
		SDL_Rect dimensions = Sprite::CheckTextureDimensions(ptr_my_texture);
		my_texture_clip.w = dimensions.w;
		my_texture_clip.h = dimensions.h;
	}
	SetTextureClip(my_texture_clip);
	//printf("Texture clip set.");
	SetCenter(*ptr_my_center);
}

void Sprite::SetCenter(Coordinates my_center)
{
	VisualComponent::SetCenter(my_center);
	position = CalculatePositionAroundCenter();
}

void Sprite::Move(double step_x, double step_y)
{
	center.x += step_x;
	center.y += step_y;
	position = CalculatePositionAroundCenter();
}

void Sprite::SetAngleDegrees(int my_angle)
{
	VisualComponent::SetAngleDegrees(my_angle);
	SetDirectionFromEightPossibilities(TellAngleDegrees());
}

void Sprite::TurnByAngleDegrees(int my_angle)
{
	VisualComponent::TurnByAngleDegrees(my_angle);
	SetDirectionFromEightPossibilities(TellAngleDegrees());
}

void Sprite::SetDirectionFromEightPossibilities(double angle_degrees)
{
	double quant_degrees = 45;
	double half_quant_degrees = quant_degrees / 2;
	double full_circle_degrees = 360;
	//printf("Will select direction from 8 possibilities.\n");

	if (angle_degrees >= 0 && angle_degrees < half_quant_degrees * 1)
	{
		current_direction = north;
	}
	else if (angle_degrees >= half_quant_degrees * 1 &&
		angle_degrees < half_quant_degrees + quant_degrees * 1)
	{
		current_direction = north_east;
	}
	else if (angle_degrees >= half_quant_degrees + quant_degrees * 1 &&
		angle_degrees < half_quant_degrees + quant_degrees * 2)
	{
		current_direction = east;
	}
	else if (angle_degrees >= half_quant_degrees + quant_degrees * 2 &&
		angle_degrees < half_quant_degrees + quant_degrees * 3)
	{
		current_direction = south_east;
	}
	else if (angle_degrees >= half_quant_degrees + quant_degrees * 3 &&
		angle_degrees < half_quant_degrees + quant_degrees * 4)
	{
		current_direction = south;
	}
	else if (angle_degrees >= half_quant_degrees + quant_degrees * 4 &&
		angle_degrees < half_quant_degrees + quant_degrees * 5)
	{
		current_direction = south_west;
	}
	else if (angle_degrees >= half_quant_degrees + quant_degrees * 5 &&
		angle_degrees < half_quant_degrees + quant_degrees * 6)
	{
		current_direction = west;
	}
	else if (angle_degrees >= half_quant_degrees + quant_degrees * 6 &&
		angle_degrees < half_quant_degrees + quant_degrees * 7)
	{
		current_direction = north_west;
	}
	else if (angle_degrees >= half_quant_degrees + quant_degrees * 7 &&
		angle_degrees <= full_circle_degrees)
	{
		current_direction = north;
	}
	else
	{
		printf("Unexpected angle: %f.\n", angle_degrees);
	}
}


PreciseRect Sprite::CalculatePositionAroundCenter()
{
	SDL_Rect current_texture_clip = TellTextureClip();
	double calculated_position_x = static_cast<int>(center.x - (current_texture_clip.w / 2));
	double calculated_position_y = static_cast<int>(center.y - (current_texture_clip.h / 2));
	double calculated_position_w = current_texture_clip.w;
	double calculated_position_h = current_texture_clip.h;
	return {calculated_position_x, calculated_position_y, calculated_position_w, calculated_position_h} ;
}

SDL_Rect Sprite::CheckTextureDimensions(SDL_Texture* ptr_my_texture)
{
    int w,h;
    //printf("Texture address is %p", ptr_my_texture);
    SDL_QueryTexture(ptr_my_texture, NULL, NULL, &w, &h);
    SDL_Rect result = {0,0,w,h};
    return result;
}

void Sprite::SetTexture(SDL_Texture* ref_my_texture)
{
    texture = ref_my_texture;
}

void Sprite::SetTextureClip(SDL_Rect my_texture_clip)
{
    texture_clip = my_texture_clip;
}

void Sprite::Render()
{
	SDL_Rect position_int = ConvertPreciseRectToSdlRect(position);
	//printf("Position int: x: %d y: %d w: %d h: %d.\n", position_int.x, position_int.y, position_int.w, position_int.h);
	if (texture_clip.w != 0 && texture_clip.h != 0)
	{
		//Angle is always 0, because sprite should not be rotated. Only sprite clips may change when angle changes.
		int angle = 0;
		SDL_Point relative_center = {static_cast<int>(position.w)/2, static_cast<int>(position.h)/2};
		SDL_RenderCopyEx(TellScreen()->renderer, texture, &texture_clip, &position_int, angle, &relative_center, flip);
		//printf("RENDER: Sprite center: x: %d, y: %d.\n", center.x, center.y);
		//printf("RENDER: Sprite position: x: %d, y: %d, w: %d, h: %d\n", position.x, position.y, position.w, position.h);
		//printf("RENDER: Sprite angle: %f.\n", angle);
	}
    else
    //If texture clip dimensions were not set, throw an exception.
    {
		printf("Texture clip w and h set to zero!\n");
		throw std::invalid_argument("Texture clip w and h set to zero!\n");
    }
}

std::vector <SDL_Rect> Sprite::CalculateAnimationClips( SDL_Rect area, int clip_w, int clip_h)
{
    //Protection against arguments == zero
    if (area.w * area.h * clip_w * clip_h == 0)
    {
        throw std::invalid_argument("Incorrect arguments passed to Sprite::CalculateAnimationClips(). 0 clips would be generated.");
    }
	//printf("Passed arguments: x: %d, y: %d, w: %d, h: %d, clip w: %d, clip h: %d\n",
	//	area.x, area.y, area.w, area.h, clip_w, clip_h);
    //How many elements will array hold?
    int elem_count = (area.w / clip_w) * (area.h / clip_h);
    //printf("Animation element count is: %d\n", elem_count);

    //Declare the array. Vector is used for getting size in an easy way later on.
    //SDL_Rect * clips = new SDL_Rect[elem_count];
    std::vector<SDL_Rect> clips(elem_count);
    int current_x = area.x;
    int current_y = area.y;
    //Filling in the array of texture clips
    for (int i = 0; i < elem_count; i++)
    {

        clips[i] = {current_x, current_y, clip_w, clip_h};
        if (current_x < (area.w - clip_w))
        {
            current_x += clip_w;
        }
        else
        {
            current_x = 0;
            current_y += clip_h;
        }
    }
    return clips;
}

void Sprite::PlayAnimation(std::vector <SDL_Rect> animation_clips, int delay_between_frames)
{
	//printf("Play animation called for sprite!\n");
    int animation_frames_count = static_cast<int>(animation_clips.size());
    texture_clip = animation_clips[static_cast<int>(animation_frame/delay_between_frames)];
	//printf("Animation frames count: %d, animation frame: %d, texture clip: %d.\n", animation_frames_count, animation_frame, static_cast<int>(animation_frame / delay_between_frames));
    animation_frame++;
    if( (static_cast<int>(animation_frame / delay_between_frames)) >= animation_frames_count)
    {
        ResetAnimationFrame();
    }
}

void Sprite::SetCurrentAnimation(AnimationType my_animation_type)
{
	ResetAnimationFrame();
	current_animation = my_animation_type;
}

void Sprite::PlayCurrentAnimation()
{
	if (current_animation == anim_none)
	{
		//DO NOTHING;
	}
	else if (current_animation == anim_idle)
	{
		IdleAnimation();
	}
	else if (current_animation == anim_walk)
	{
		WalkAnimation();
	}
	else if (current_animation == anim_vortex)
	{
		VortexAnimation();
	}
	else
	{
		printf("Unknown animation type!\n");
	}
}

void Sprite::ResetAnimationFrame()
{
    animation_frame = 0;
}

//#####################
//Tell methods
//#####################

PreciseRect Sprite::TellSpritePosition()
{
    return position;
}

SDL_Rect Sprite::TellTextureClip()
{
    return texture_clip;
}

Directions Sprite::TellCurrentDirection()
{
	return current_direction;
}

//###############
//STATIC METHODS
//###############

void Sprite::SetTextureBank(TextureBank* ptr_my_texture_bank)
{
	ptr_texture_bank = ptr_my_texture_bank;
}

TextureBank* Sprite::TellTextureBank()
{
	return ptr_texture_bank;
}

//#####################
//Dummy virtual methods
//#####################

void Sprite::IdleAnimation()
{
	printf("Idle animation called for Sprite\n");
}

void Sprite::WalkAnimation()
{
    printf("Walk animation called for Sprite\n");
}

void Sprite::SmokeAnimation()
{
	printf("Smoke animation called for Sprite\n");
}

void Sprite::VortexAnimation()
{
	printf("Vortex animation called for Sprite\n");
}

void Sprite::SetClipAccordingToWallType(WallType my_type)
{
	printf("Setting wall clip called for Sprite\n");
}