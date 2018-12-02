#ifndef GAME_HPP
#define GAME_HPP
#include <Screen.hpp>
#include <TextureBank.hpp>
#include <Level.hpp>
#include <Creature.hpp>
#include <FactorySpawningLevels.hpp>
#include <ctime>

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
		static Level* ptr_current_level;
		static FactorySpawningLevels* ptr_levels_factory;

        //###################
        //FUNCTIONS
        //###################
        static void InitializeGame();
        static void DestroyGame();
        static void CreateScreen();
		static void LoadTextures();
		static void SetCurrentLevel(Level* ptr_my_current_level);

};

#endif //GAME_HPP
