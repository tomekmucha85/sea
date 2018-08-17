#include <SpriteBackground.hpp>

SpriteBackground::SpriteBackground(SDL_Renderer* ptr_my_renderer, TextureBank* ptr_my_texture_bank, SDL_Rect* my_position)
                              : Sprite(ptr_my_renderer, ptr_my_texture_bank)
{
    printf("Constructor called for SpriteBackground\n");
    SDL_Texture* ptr_texture = ptr_my_texture_bank->ptr_tex_background;
    SetTexture(ptr_texture);
    printf("Texture set for SpriteBackground\n");
    SDL_Rect texture_dimensions = CheckTextureDimensions(ptr_texture);
    texture_clip_background.w = texture_dimensions.w;
    texture_clip_background.h = texture_dimensions.h;
    SetTextureClip(texture_clip_background);
    printf("Texture clip set for SpriteBackground\n");
    SDL_Rect current_texture_clip = TellTextureClip();
    if (my_position == NULL)
    {
        SetPositionX(0);
        SetPositionY(0);
        SetPositionW(current_texture_clip.w);
        SetPositionH(current_texture_clip.h);
    }
    else
    {
        SetPositionX(my_position->x);
        SetPositionY(my_position->y);
        SetPositionW(current_texture_clip.w);
        SetPositionH(current_texture_clip.h);
    }
}
