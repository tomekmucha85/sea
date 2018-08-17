#include <TextureBank.hpp>

TextureBank::TextureBank(SDL_Renderer* renderer)
{
    printf("Loading textures\n");
    Texture* background = new Texture(path_image_bground,renderer);
    ptr_tex_background = background->texture;
    Texture* clawy = new Texture(path_image_clawy,renderer);
    ptr_tex_clawy = clawy->texture;
    Texture* black_box = new Texture(path_image_black_box,renderer);
    ptr_tex_black_box = black_box->texture;
    printf("Finished loading textures\n");
}


TextureBank::~TextureBank()
{
    delete background;
    delete clawy;
    delete black_box;
}
