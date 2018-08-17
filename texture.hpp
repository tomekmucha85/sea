#ifndef TEXTURE_HPP
#define TEXTURE_HPP
#include <string>
#include <SDL.h>
#include <SDL_image.h>

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
        SDL_Texture* texture = NULL;
        //###################
        //Functions
        //###################
        Texture(std::string path, SDL_Renderer* renderer);
        ~Texture();
};
#endif // TEXTURE_HPP
