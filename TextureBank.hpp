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
        std::string path_image_bground = "images/bground.png";
        Texture* background = NULL;
        std::string path_image_clawy = "images/guy.png";
        Texture* clawy = NULL;
        std::string path_image_black_box = "images/black_box.png";
        Texture* black_box = NULL;

    public:
        //###################
        //Variables
        //###################
        SDL_Texture* ptr_tex_background = NULL;
        SDL_Texture* ptr_tex_clawy = NULL;
        SDL_Texture* ptr_tex_black_box = NULL;
        //###################
        //Functions
        //###################
        TextureBank(SDL_Renderer* renderer);
        ~TextureBank();

};

#endif //TEXBANK_HPP
