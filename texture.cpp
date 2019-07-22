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
	SDL_Texture* new_texture = nullptr;

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

SDL_Texture* Texture::LoadTextureFromRenderedText(std::string text, SDL_Renderer* renderer, TTF_Font* ptr_font, SDL_Color color)
{
	//The final texture
	SDL_Texture* ptr_new_texture = nullptr;

	SDL_Surface* ptr_text_surface = TTF_RenderText_Solid(ptr_font, text.c_str(), color);
	if (ptr_text_surface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		//Create texture from surface pixels
		ptr_new_texture = SDL_CreateTextureFromSurface(renderer, ptr_text_surface);
		if (ptr_new_texture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", text.c_str(), SDL_GetError());
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(ptr_text_surface);
	}

	return ptr_new_texture;
}