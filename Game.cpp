#include <Game.hpp>

//***********************************
//DEFINITIONS OF STATIC CLASS MEMBERS
//***********************************

Screen* Game::ptr_screen = nullptr;
TextureBank* Game::ptr_texture_bank = nullptr;
FontBank* Game::ptr_font_bank = nullptr;
int Game::debug_counter;
Level* Game::ptr_current_level = nullptr;
FactorySpawningLevels* Game::ptr_levels_factory;
std::vector <Level*> Game::currently_existing_levels;
std::vector<LevelType> Game::levels_order = { level_ninemazes };
unsigned int Game::current_level_number = 0;

//***********************************
//METHODS
//***********************************

void Game::InitializeGame()
{
    printf("Going to initialize game.\n");
	debug_counter = 10;
    CreateScreen();
    LoadTextures();
	LoadFonts();
	ptr_levels_factory = new FactorySpawningLevels();
	srand(time(NULL));
}

Level* Game::InitializeLevel(LevelType my_type)
{
	Level* ptr_spawned_level = ptr_levels_factory->SpawnLevel(my_type);
	currently_existing_levels.push_back(ptr_spawned_level);
	return ptr_spawned_level;
}

void Game::PrepareSingleLevel(LevelType my_type)
{
	std::vector<Level*> currently_existing_levels_copy = currently_existing_levels;
	for (Level* ptr_existing_level : currently_existing_levels_copy)
	{
		DestroyLevel(ptr_existing_level);
	}
	Level* ptr_prepared_level = Game::InitializeLevel(my_type);
	SetCurrentLevel(ptr_prepared_level);
	printf("Number of existing levels: %d.\n", currently_existing_levels.size());
}

void Game::DestroyLevel(Level* ptr_destroyed_level)
{
	//Removing pointer from my currently existing level
	currently_existing_levels.erase(std::remove(currently_existing_levels.begin(), currently_existing_levels.end(), ptr_destroyed_level), currently_existing_levels.end());
	//Deleting level
	delete ptr_destroyed_level;
}

/*void Game::UnloadLevel(Level* ptr_unloaded_level)
{
	;
}

void Game::LoadLevel(Level* ptr_loaded_level)
{
	if (ptr_loaded_level == ptr_current_level)
	{
		printf("Level %p is already loaded! No need to load it again.\n", ptr_current_level);
	}
	else
	{
		;
	}
}*/

void Game::DestroyGame()
{
	printf("Going to destroy game.\n");
	delete ptr_current_level;
	Sprite::SetTextureBank(nullptr);
	VisualComponent::SetScreen(nullptr);
	delete ptr_texture_bank;
	delete ptr_font_bank;
	delete ptr_screen;
	delete ptr_levels_factory;
}

void Game::CreateScreen()
{
    Game::ptr_screen = new Screen();
    printf("Screen address is: %p\n", Game::ptr_screen);
	VisualComponent::SetScreen(ptr_screen);
}

void Game::LoadTextures()
{
    Game::ptr_texture_bank = new TextureBank(Screen::renderer);
	Sprite::SetTextureBank(ptr_texture_bank);
}

void Game::LoadFonts()
{
	Game::ptr_font_bank = new FontBank();
}

void Game::SetCurrentLevel(Level* ptr_my_current_level)
{
	ptr_current_level = ptr_my_current_level;
}

void Game::SwitchBetweenLevelsIfNeeded()
{
	if (ptr_current_level->TellIfLevelIsFinished() && ptr_current_level->TellLevelEnding() == ending_victory)
	{
		unsigned int next_level_number = current_level_number++;
		//If next level goes beyond total levels number, begin from the start
		if (next_level_number >= levels_order.size())
		{
			next_level_number = 0;
		}
		Logger::Log("Loading next level: " + next_level_number, debug_info);
		current_level_number = next_level_number;
		PrepareSingleLevel(levels_order[current_level_number]);
	}
	else if (ptr_current_level->TellIfLevelIsFinished() && ptr_current_level->TellLevelEnding() == ending_defeat)
	{
		Logger::Log("Restarting a failed level", debug_info);
		PrepareSingleLevel(levels_order[current_level_number]);
	}
	else if (ptr_current_level->TellIfLevelIsFinished() && ptr_current_level->TellLevelEnding() == ending_entering_menu)
	{
		Logger::Log("Going to menu", debug_info);
		PrepareSingleLevel(level_menu);
	}
	else if (ptr_current_level->TellIfLevelIsFinished() && ptr_current_level->TellLevelEnding() == ending_exiting_menu)
	{
		Logger::Log("Exiting menu, recreating current level", debug_info);
		PrepareSingleLevel(levels_order[current_level_number]);
	}
}