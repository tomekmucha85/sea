#ifndef GAME_HPP
#define GAME_HPP
#include <ctime>
#include <algorithm>
#include <Screen.hpp>
#include <TextureBank.hpp>
#include <FontBank.hpp>
#include <Level.hpp>
#include <FactorySpawningLevels.hpp>
#include <VisualComponent.hpp>
#include <Sprite.hpp>

class Game
{
    private:
		static std::vector <Level*> currently_existing_levels;
		static std::vector<LevelType> levels_order;
		static unsigned int current_level_number;

    public:

        //###################
        //VARIABLES
        //###################

		static int debug_counter;
        static Screen* ptr_screen;
        static TextureBank* ptr_texture_bank;
		static FontBank* ptr_font_bank;
		static Level* ptr_current_level;
		static FactorySpawningLevels* ptr_levels_factory;

        //###################
        //FUNCTIONS
        //###################
        static void InitializeGame();
		static Level* InitializeLevel(LevelType my_type);
		static void DestroyLevel(Level* ptr_destroyed_level);
		static void PrepareSingleLevel(LevelType my_type);
		static void SwitchBetweenLevelsIfNeeded();
		static void UnloadLevel(Level* ptr_unloaded_level);
		static void LoadLevel(Level* ptr_loaded_level);
        static void DestroyGame();
        static void CreateScreen();
		static void LoadTextures();
		static void LoadFonts();
		static void SetCurrentLevel(Level* ptr_my_current_level);

};

#endif //GAME_HPP
