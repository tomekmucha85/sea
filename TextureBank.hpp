#ifndef TEXBANK_HPP
#define TEXBANK_HPP
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <texture.hpp>

class TextureBank
{
    private:
        //###################
        //Variables
        //###################
        std::string path_image_bground = "images/maze_bground.png";
        Texture* background = nullptr;
		std::string path_image_clawy = "images/wolfy.png";
        Texture* clawy = nullptr;
        std::string path_image_black_box = "images/black_box.png";
        Texture* black_box = nullptr;
		std::string path_image_black_smoke = "images/black_smoke.png";
		Texture* black_smoke = nullptr;
		std::string path_image_wall = "images/walls_variants.png";
		Texture* purple_vortex = nullptr;
		std::string path_image_purple_vortex = "images/vortex.png";
		Texture* wall = nullptr;
		std::string path_image_exit = "images/exit.png";
		Texture* exit = nullptr;
		//std::string path_image_gui_mana_bar = "images/GUI_mana_bar.png";
		std::string path_image_gui_mana_bar = "images/cutlery.png";
		Texture* gui_mana_bar = nullptr;
		std::string path_image_snailmage = "images/snailmage.png";
		Texture* snailmage = nullptr;
		std::string path_image_implosion = "images/implosion.png";
		Texture* implosion = nullptr;

    public:
        //###################
        //Variables
        //###################
        SDL_Texture* ptr_tex_background = nullptr;
        SDL_Texture* ptr_tex_clawy = nullptr;
        SDL_Texture* ptr_tex_black_box = nullptr;
		SDL_Texture* ptr_tex_black_smoke = nullptr;
		SDL_Texture* ptr_tex_wall = nullptr;
		SDL_Texture* ptr_tex_purple_vortex = nullptr;
		SDL_Texture* ptr_tex_exit = nullptr;
		SDL_Texture* ptr_tex_gui_mana_bar = nullptr;
		SDL_Texture* ptr_tex_snailmage = nullptr;
		SDL_Texture* ptr_tex_implosion = nullptr;
        //###################
        //Functions
        //###################
        TextureBank(SDL_Renderer* renderer);
        ~TextureBank();

};

#endif //TEXBANK_HPP
