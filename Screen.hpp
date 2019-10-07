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
        static const int SCREEN_WIDTH;
        static const int SCREEN_HEIGHT;
		//static const int RESOLUTION_W = 640;
		//static const int RESOLUTION_H = 480;

    public:
		static bool should_be_fullscreen;
        //The window itself
        static SDL_Window* window;
        //The window renderer
        static SDL_Renderer* renderer;

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
