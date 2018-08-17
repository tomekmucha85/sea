#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <string>
#include <Game.hpp>
#include <Screen.hpp>
#include <Texture.hpp>
#include <TextureBank.hpp>
#include <Sprite.hpp>
#include <SpriteClawy.hpp>
#include <SpriteBackground.hpp>
#include <SpriteBlackBox.hpp>
#include <Creature.hpp>



//###################
//Function prototypes
//###################


//###################
//Variables
//###################
int test = 0;

int main(int argc, char* args[])
{
    //Main loop flag
    bool quit = false;

    //Event handler
    SDL_Event event_handler;

    Game::InitializeGame();

    //Load textures
    printf("Address of screen before loading textures: %p\n", Game::ptr_screen);
    TextureBank texture_bank(Game::ptr_screen->renderer);
    TextureBank* ptr_texture_bank = &texture_bank;
    /* Address of texture bank should go to a static class variable.
       Sprite creator will check if the value is set.
    */

    //Create sprites - TODO needs a method
    /*Takes: position
             kind of sprite
      Returns:
              sprite object
      A dictionary connects sprite kind to texture bank? Or maybe sprite sub class known the right bank by itself?

    */

    SpriteBackground blue_background(Game::ptr_screen->renderer, ptr_texture_bank);

    SDL_Rect hero_position = {320,240,0,0};
    SDL_Rect* ptr_hero_position = &hero_position;
    Sprite* ptr_spr_clawy = Sprite::CreateSprite(clawy, ptr_texture_bank, ptr_hero_position);
    //Sprite our_hero = *(ptr_spr_clawy);


    //SDL_Rect hero_position = {320,240,0,0};
    //SDL_Rect* ptr_hero_position = &hero_position;
    //SpriteClawy our_hero(Game::ptr_screen->renderer, ptr_texture_bank, ptr_hero_position);
    //SpriteClawy* ptr_spr_clawy = &our_hero;

    SDL_Rect box_position = {100,100,0,0};
    SDL_Rect* ptr_box_position = &box_position;
    SpriteBlackBox black_box(Game::ptr_screen->renderer, ptr_texture_bank, ptr_box_position);
    SpriteBlackBox* ptr_spr_black_box = &black_box;

    SDL_Rect box_position_2 = {300,150,0,0};
    SDL_Rect* ptr_box_position_2 = &box_position_2;
    SpriteBlackBox black_box_2(Game::ptr_screen->renderer, ptr_texture_bank, ptr_box_position_2);
    SpriteBlackBox* ptr_spr_black_box_2 = &black_box_2;

    SDL_Rect box_position_3 = {500,150,0,0};
    SDL_Rect* ptr_box_position_3 = &box_position_3;
    SpriteBlackBox black_box_3(Game::ptr_screen->renderer, ptr_texture_bank, ptr_box_position_3);
    SpriteBlackBox* ptr_spr_black_box_3 = &black_box_3;

    //TODO - connect creatures creation with sprites

    Creature cre_hero(ptr_spr_clawy);
    cre_hero.MakeMeMainCharacter();

    Creature cre_box(ptr_spr_black_box);
    Creature cre_box2(ptr_spr_black_box_2);
    Creature cre_box3(ptr_spr_black_box_3);

    std::cout << "Obstacles present: ";
    std::cout << Creature::TellObstaclesCount() << std::endl;
    std::cout << "Creatures present: ";
    std::cout << Creature::TellInstancesCount() << std::endl;

    while (!quit)
    {
        //Handle events on queue
        while(SDL_PollEvent(&event_handler) != 0)
        {
            //User requests quit
            if(event_handler.type == SDL_QUIT)
            {
                quit = true;
            }
            else if(event_handler.type == SDL_KEYDOWN)
            {
                switch( event_handler.key.keysym.sym )
                {
                    case SDLK_UP: cre_hero.MoveForward(); break;
                    case SDLK_DOWN: cre_hero.MoveBackward(); break;
                    case SDLK_LEFT: cre_hero.StrafeLeft(); break;
                    case SDLK_RIGHT: cre_hero.StrafeRight(); break;

                    case SDLK_w: cre_hero.MoveForward(); break;
                    case SDLK_s: cre_hero.MoveBackward(); break;
                    case SDLK_a: cre_hero.TurnLeft(); break;
                    case SDLK_d: cre_hero.TurnRight(); break;
                }
            }
        }

        //Test moving creatures
        test++;
        if (test <= 300)
        {
            cre_box3.Move(3,0);
        }
        else if (test <= 600)
        {
            cre_box3.Move(-3,0);
        }
        else
        {
            test = 0;
        }

        //Clear screen
        SDL_RenderClear(Game::ptr_screen->renderer);


        //Render texture to screen
        blue_background.Render();
        black_box.Render();
        black_box_2.Render();
        black_box_3.Render();
        ptr_spr_clawy->Render();

        //Update screen
        SDL_RenderPresent(Game::ptr_screen->renderer);
    }
    //Do cleanup
    Game::DestroyGame();
    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();

    return 0;
}
