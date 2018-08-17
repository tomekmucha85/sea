#include <Game.hpp>

//***********************************
//DEFINITIONS OF STATIC CLASS MEMBERS
//***********************************

Screen* Game::ptr_screen;
//int Game::ptr_texture_bank;

//***********************************
//METHODS
//***********************************

void Game::InitializeGame()
{
    printf("Going to initialize game.\n");
    CreateScreen();
    //LoadTextures();
}

void Game::DestroyGame()
{
    printf("Going to destroy game.\n");
}

void Game::CreateScreen()
{
    static Screen my_screen;
    Game::ptr_screen = &my_screen;
    printf("Screen address is: %p\n", Game::ptr_screen);
}

//void Game::LoadTextures()
//{
//    TextureBank texture_bank(Game::ptr_screen->renderer);
//    TextureBank* ptr_texture_bank = &texture_bank;
//}
