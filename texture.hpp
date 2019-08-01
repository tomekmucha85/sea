#ifndef TEXTURE_HPP
#define TEXTURE_HPP
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <Logger.hpp>

class Texture
{

    private:
        //###################
        //Functions
        //###################
        SDL_Texture* LoadTexture(std::string path, SDL_Renderer* renderer);

    public:
        //###################
        //Variables
        //###################
        SDL_Texture* texture = NULL; //#TODO - czy nie nullptr?!

        //###################
        //Functions
        //###################
        Texture(std::string path, SDL_Renderer* renderer);
        ~Texture();
		static SDL_Texture* LoadTextureFromRenderedText(std::string text, 
			SDL_Renderer* renderer, 
			TTF_Font* ptr_font, 
			SDL_Color color);
};
#endif // TEXTURE_HPP
