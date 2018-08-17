#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <Screen.hpp>

Screen::Screen()
{
    printf("Initialization begun\n");
    Screen::Init();
    printf("Initialization finished\n");
}

Screen::~Screen()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

bool Screen::Init()
{
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        //Set texture filtering to linear
		if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}
        //Create window
        window = SDL_CreateWindow("Sea", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if(window == NULL)
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            //Create renderer for window
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if(renderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if(!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
        }
    }

    return success;
}

//*****************
//CAMERA CORRECTION
//*****************

void Screen::SetXCorrection(int x)
{
    printf("Dupa.\n");
}

void Screen::SetYCorrection(int y)
{
    printf("Dupa.\n");
}