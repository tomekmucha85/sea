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

	SetPositionX(ptr_my_center->x - (current_texture_clip.w / 2));
	SetPositionY(ptr_my_center->y - (current_texture_clip.h / 2));
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

void Sprite::SetPositionX(int new_x)
{
    position.x = new_x;
}

void Sprite::SetPositionY(int new_y)
{
    position.y = new_y;
}

void Sprite::SetPositionW(int new_w)
{
    position.w = new_w;
}

void Sprite::SetPositionH(int new_h)
{
    position.h = new_h;
}

void Sprite::Render()
{
	SDL_Rect position_int = ConvertPreciseRectToSdlRect(position);
    if (texture_clip.w != 0 && texture_clip.h != 0)
    //If texture clip dimensions were set, apply them.
    {
		SDL_RenderCopyEx(TellScreen()->renderer, texture, &texture_clip, &position_int, angle, center, flip);
    }
    else
    //If texture clip dimensions were not set, show whole texture.
    {
        SDL_RenderCopyEx(TellScreen()->renderer, texture, NULL, &position_int, angle, center, flip);
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
    int animation_frames_count = animation_clips.size();
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
	PreciseRect result = {position.x, position.y, position.w, position.h};
    return result;
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

SDL_Rect Sprite::ConvertPreciseRectToSdlRect(PreciseRect my_rect)
{
	SDL_Rect result = { int(my_rect.x), int(my_rect.y), int(my_rect.w), int(my_rect.h) };
	return result;
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
