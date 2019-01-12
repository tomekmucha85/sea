#include <Sprite.hpp>

//***********************************
//DEFINITIONS OF STATIC CLASS MEMBERS
//***********************************

TextureBank* Sprite::ptr_texture_bank = nullptr;


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
	SDL_Rect current_texture_clip = TellTextureClip();

	SetPositionX(static_cast<int>(center.x - (current_texture_clip.w / 2)));
	SetPositionY(static_cast<int>(center.y - (current_texture_clip.h / 2)));
	SetPositionW(current_texture_clip.w);
	SetPositionH(current_texture_clip.h);

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
	if (texture_clip.w != 0 && texture_clip.h != 0)
	{
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
    //How many elements will array hold?
    int elem_count = ((area.w - area.x) / clip_w) * ((area.h - area.y) / clip_h);
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
    int animation_frames_count = static_cast<int>(animation_clips.size());
    texture_clip = animation_clips[animation_frame/delay_between_frames];
    Sprite::Render();
    animation_frame++;
    if( (animation_frame / delay_between_frames) >= animation_frames_count )
    {
        animation_frame = 0;
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
