#ifndef SCREEN_HPP
#define SCREEN_HPP
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>

class Screen
{
    private:
        //Screen dimension constants
        static const int SCREEN_WIDTH = 800;
        static const int SCREEN_HEIGHT = 600;
		//static const int RESOLUTION_W = 640;
		//static const int RESOLUTION_H = 480;

    public:
        //The window itself
        SDL_Window* window = NULL;
        //The window renderer
        SDL_Renderer* renderer = NULL;

        //###################
        //Functions
        //###################
        Screen();
        ~Screen();
        bool Init();
		static int TellScreenWidth();
		static int TellScreenHeight();
};

#endif //SCREEN_HPP
