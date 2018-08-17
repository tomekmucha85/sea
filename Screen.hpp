#include <SDL.h>
#include <SDL_image.h>
#ifndef SCREEN_HPP
#define SCREEN_HPP
class Screen
{
    private:
        //Screen dimension constants
        const int SCREEN_WIDTH = 640;
        const int SCREEN_HEIGHT = 480;
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
};

#endif //SCREEN_HPP
