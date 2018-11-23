#include <Game.hpp>

//***********************************
//DEFINITIONS OF STATIC CLASS MEMBERS
//***********************************

Screen* Game::ptr_screen;
TextureBank* Game::ptr_texture_bank;
int Game::debug_counter;
Level* Game::ptr_current_level;

//***********************************
//METHODS
//***********************************

void Game::InitializeGame()
{
    printf("Going to initialize game.\n");
	debug_counter = 10;
    CreateScreen();
    LoadTextures();
}

void Game::DestroyGame()
{
	printf("Going to destroy game.\n");
	delete ptr_texture_bank;
	delete ptr_screen;
}

void Game::CreateScreen()
{
    Game::ptr_screen = new Screen();
    printf("Screen address is: %p\n", Game::ptr_screen);
}

void Game::LoadTextures()
{
    Game::ptr_texture_bank = new TextureBank(Game::ptr_screen->renderer);
}

void Game::SetCurrentLevel(Level* ptr_my_current_level)
{
	ptr_current_level = ptr_my_current_level;
}
