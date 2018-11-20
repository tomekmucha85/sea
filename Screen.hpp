#include <SDL.h>
#include <SDL_image.h>
#ifndef SCREEN_HPP
#define SCREEN_HPP
class Screen
{
    private:
        //Screen dimension constants
        static const int SCREEN_WIDTH = 800;
        static const int SCREEN_HEIGHT = 600;
        //"Camera" - used for following character
        static int x_correction;
        static int y_correction;

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
        static void SetXCorrection(int x);
        static void SetYCorrection(int y);
		static int TellScreenWidth();
		static int TellScreenHeight();
};

#endif //SCREEN_HPP
