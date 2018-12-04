#include <Game.hpp>

//***********************************
//DEFINITIONS OF STATIC CLASS MEMBERS
//***********************************

Screen* Game::ptr_screen;
TextureBank* Game::ptr_texture_bank;
int Game::debug_counter;
Level* Game::ptr_current_level;
FactorySpawningLevels* Game::ptr_levels_factory;

//***********************************
//METHODS
//***********************************

void Game::InitializeGame()
{
    printf("Going to initialize game.\n");
	debug_counter = 10;
    CreateScreen();
    LoadTextures();
	ptr_levels_factory = new FactorySpawningLevels();
	srand(time(NULL));
}

void Game::DestroyGame()
{
	printf("Going to destroy game.\n");
	delete ptr_current_level;
	Sprite::SetTextureBank(nullptr);
	VisualComponent::SetScreen(nullptr);
	delete ptr_texture_bank;
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
    Game::ptr_texture_bank = new TextureBank(Game::ptr_screen->renderer);
	Sprite::SetTextureBank(ptr_texture_bank);
}

void Game::SetCurrentLevel(Level* ptr_my_current_level)
{
	ptr_current_level = ptr_my_current_level;
}
