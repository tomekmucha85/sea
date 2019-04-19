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
	Texture* black_smoke = new Texture(path_image_black_smoke, renderer);
	ptr_tex_black_smoke = black_smoke->texture;
	Texture* wall = new Texture(path_image_wall, renderer);
	ptr_tex_wall = wall->texture;
	Texture* purple_vortex = new Texture(path_image_purple_vortex, renderer);
	ptr_tex_purple_vortex = purple_vortex->texture;
	Texture* exit = new Texture(path_image_exit, renderer);
	ptr_tex_exit = exit->texture;
	Texture* gui_mana_bar = new Texture(path_image_gui_mana_bar, renderer);
	ptr_tex_gui_mana_bar = gui_mana_bar->texture;
	Texture* snailmage = new Texture(path_image_snailmage, renderer);
	ptr_tex_snailmage = snailmage->texture;
	Texture* implosion = new Texture(path_image_implosion, renderer);
	ptr_tex_implosion = implosion->texture;
    printf("Finished loading textures\n");
}


TextureBank::~TextureBank()
{
    delete background;
    delete clawy;
    delete black_box;
	delete wall;
	delete purple_vortex;
	delete exit;
	delete gui_mana_bar;
	delete snailmage;
	delete implosion;
}
