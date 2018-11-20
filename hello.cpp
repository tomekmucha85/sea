#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <string>
#include <Game.hpp>
#include <Level.hpp>
#include <Maze.hpp>
#include <Screen.hpp>
#include <Texture.hpp>
#include <TextureBank.hpp>
#include <Sprite.hpp>
#include <Creature.hpp>
#include <CommonHeaderCreatures.hpp>
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

	Level* my_level = new Level();
	/*for (int i = 0; i < 10; i++)
	{
		my_level->GenerateRandomObjectOnMap();
	}*/
	my_level->PrintMap();

    Sprite* ptr_blue_background = Sprite::CreateSprite(background);
	/*SDL_Rect smoke_position = { 500,200,0,0 };
	SDL_Rect* ptr_smoke_position = &smoke_position;
	Creature* cre_black_smoke_1 = Creature::SpawnCreature(cre_black_smoke, ptr_smoke_position);
	cre_black_smoke_1->MakeMeNotObstacle();
	*/
	//Dimensions expressed in map blocks

	/*
	SDL_Rect maze_area = { 0, 0, 40, 40 };
	SDL_Rect* ptr_maze_area = &maze_area;
	SDL_Rect maze_area_2 = { 20, 20, 40, 40 };
	SDL_Rect* ptr_maze_area_2 = &maze_area_2;
	Maze* my_maze_a = new Maze(my_level, ptr_maze_area);
	Maze* my_maze_b = new Maze(my_level, ptr_maze_area_2);
	*/


	/*std::vector<std::vector<CreatureType>> hero_and_env = { {cre_none, cre_none, cre_none},
	                                                        {cre_none, cre_none, cre_none}, 
	                                                        {cre_none, cre_none, cre_none} };
	SDL_Rect env_position = {14,12,0,0};
	SDL_Rect* ptr_env_position = &env_position;
	my_level->InsertStructureOntoMap(hero_and_env, ptr_env_position);*/
	SDL_Rect hero_position = { 13,12,0,0 };
	SDL_Rect* ptr_hero_position = &hero_position;
	my_level->InsertCreatureOntoMap(cre_clawy, ptr_hero_position);
	Creature* ptr_hero = my_level->TellPointerToCreatureInSlot(13,12);
	ptr_hero->MakeMeMainCharacter();

    /*SDL_Rect hero_position = {320,240,0,0};
    SDL_Rect* ptr_hero_position = &hero_position;
	Creature* cre_heros = Creature::SpawnCreature(cre_clawy, ptr_hero_position);
	cre_heros->MakeMeMainCharacter();*/
    //my_level->PrintMap();
	//my_level->TestArray();

	/*
	SDL_Rect box_4_position = { 25,25,0,0 };
	SDL_Rect* ptr_box_4_position = &box_4_position;
	Creature* cre_box4 = Creature::SpawnCreature(cre_flying_box,ptr_box_4_position);
	*/
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
                    case SDLK_UP: ptr_hero->MoveForward(); break;
                    case SDLK_DOWN: ptr_hero->MoveBackward(); break;
                    case SDLK_LEFT: ptr_hero->StrafeLeft(); break;
                    case SDLK_RIGHT: ptr_hero->StrafeRight(); break;
                    case SDLK_w: ptr_hero->MoveForward();
					break;
                    case SDLK_s: ptr_hero->MoveBackward();
					break;
                    case SDLK_a: ptr_hero->TurnLeft(); break;
                    case SDLK_d: ptr_hero->TurnRight(); break;
					//case SDLK_1: my_maze_a->ClearMaze(); break;
					//case SDLK_2: my_maze_a->GenerateMaze(); break;
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
		//#TODO napisaæ klasê Level
		Level::RenderAllPresentCreatures();
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
