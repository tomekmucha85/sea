#include <Screen.hpp>
#include <TextureBank.hpp>
#ifndef GAME_HPP
#define GAME_HPP

class Game
{
    private:

    public:

        //###################
        //VARIABLES
        //###################

		static int debug_counter;
        static Screen* ptr_screen;
        static TextureBank* ptr_texture_bank;

        //###################
        //FUNCTIONS
        //###################
        static void InitializeGame();
        static void DestroyGame();
        static void CreateScreen();
		static void LoadTextures();

};

#endif //GAME_HPP
