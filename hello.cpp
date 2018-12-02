#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <string>
#include <Game.hpp>
#include <Level.hpp>
#include <LevelNineMazes.hpp>
#include <Screen.hpp>
#include <Texture.hpp>
#include <TextureBank.hpp>
#include <Sprite.hpp>
#include <Creature.hpp>
#include <LevelComponent.hpp>
#include <CommonHeaderSprites.hpp>

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

	Level* first_level = Game::ptr_levels_factory->SpawnLevel(level_ninemazes);
	Game::SetCurrentLevel(first_level);
    Sprite* ptr_blue_background = Sprite::CreateSprite(background);
	/*SDL_Rect smoke_position = { 500,200,0,0 };
	SDL_Rect* ptr_smoke_position = &smoke_position;
	Creature* cre_black_smoke_1 = Creature::SpawnCreature(cre_black_smoke, ptr_smoke_position);
	cre_black_smoke_1->MakeMeNotObstacle();
	*/
	/*
	SDL_Rect box_4_position = { 25,25,0,0 };
	SDL_Rect* ptr_box_4_position = &box_4_position;
	Creature* cre_box4 = Creature::SpawnCreature(cre_flying_box,ptr_box_4_position);
	*/

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
				    case SDLK_UP: Game::ptr_current_level->ptr_hero->MoveForward(); break;
                    case SDLK_DOWN: Game::ptr_current_level->ptr_hero->MoveBackward(); break;
                    case SDLK_LEFT: Game::ptr_current_level->ptr_hero->StrafeLeft(); break;
                    case SDLK_RIGHT: Game::ptr_current_level->ptr_hero->StrafeRight(); break;
                    case SDLK_w: Game::ptr_current_level->ptr_hero->MoveForward();
					break;
                    case SDLK_s: Game::ptr_current_level->ptr_hero->MoveBackward();
					break;
                    case SDLK_a: Game::ptr_current_level->ptr_hero->TurnLeft(); break;
                    case SDLK_d: Game::ptr_current_level->ptr_hero->TurnRight(); break;
					//case SDLK_1: ptr_maze_a->ClearMaze(); break;
					//case SDLK_2: ptr_maze_a->GenerateMaze(); break;
					//case SDLK_3: my_maze_b->ClearMaze(); break;
					//case SDLK_4: my_maze_b->GenerateMaze(); break;
                }
            }
        }

        //Test moving creatures
		
		/*
        test++;
        if (test <= 300)
        {
            cre_box3->Move(3,0);
        }
        else if (test <= 600)
        {
            cre_box3->Move(-3,0);
        }
        else
        {
            test = 0;
        }
		*/
		//Test background animation
		//#TODO napisaæ wspóln¹ funkcjê dla animacji
		//cre_black_smoke_1->ptr_creature_sprite->SmokeAnimation();

        //Clear screen
        SDL_RenderClear(Game::ptr_screen->renderer);

        //Render texture to screen
		ptr_blue_background->Render();
		//#TODO zrobiæ generator Creature, zadbac o destruktor i konstruktor kopiujacy
		first_level->RenderAllPresentCreatures();
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
