#include <Texture.hpp>

Texture::Texture(std::string path, SDL_Renderer* renderer)
{
    texture = Texture::LoadTexture(path, renderer);
}

Texture::~Texture()
{
    SDL_DestroyTexture(texture);
}

SDL_Texture* Texture::LoadTexture(std::string path, SDL_Renderer* renderer)
{
    printf("Path of texture to be loaded: %s\n", path.c_str());

	//The final texture
	SDL_Texture* new_texture = NULL;

	//Load image at specified path
	SDL_Surface* loaded_surface = IMG_Load(path.c_str());
	if(loaded_surface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Create texture from surface pixels
        new_texture = SDL_CreateTextureFromSurface(renderer, loaded_surface);
		if(new_texture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loaded_surface);
	}
    printf("Texture loaded.\n");
	return new_texture;
}

