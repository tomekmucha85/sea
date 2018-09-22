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

    Sprite* ptr_blue_background = Sprite::CreateSprite(background);

    SDL_Rect hero_position = {320,240,0,0};
    SDL_Rect* ptr_hero_position = &hero_position;
    Sprite* ptr_spr_clawy = Sprite::CreateSprite(clawy, ptr_hero_position);

    //SDL_Rect box_position = {100,100,0,0};
    //SDL_Rect* ptr_box_position = &box_position;
    //Sprite* ptr_spr_black_box = Sprite::CreateSprite(box, ptr_box_position);

    SDL_Rect box_position_2 = {300,150,0,0};
	SDL_Rect* ptr_box_position_2 = &box_position_2;
	Sprite* ptr_spr_black_box_2 = Sprite::CreateSprite(box, ptr_box_position_2);

    SDL_Rect box_position_3 = {500,150,0,0};
	SDL_Rect* ptr_box_position_3 = &box_position_3;
	Sprite* ptr_spr_black_box_3 = Sprite::CreateSprite(box, ptr_box_position_3);

    //#TODO - connect creatures creation with sprites

    Creature cre_hero(ptr_spr_clawy);
    cre_hero.MakeMeMainCharacter();

    //Creature cre_box(ptr_spr_black_box);
	SDL_Rect box_position = { 100,100,0,0 };
	//SpriteType my_sprite_type, SDL_Rect my_position, int hitbox_margin = 10
	Creature cre_box(box,box_position);
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
		ptr_blue_background->Render();
        //ptr_spr_black_box->Render();
		//#TODO zrobiæ generator Creature, zadbac o destruktor i konstruktor kopiujacy
		//#TODO napisaæ klasê Level
		//#TODO sprawic, ¿eby wszystkie sprite'y obecne w grze renderowa³y siê za pomoc¹ jednej funkcji
		cre_box.ptr_creature_sprite->Render();
        ptr_spr_black_box_2->Render();
        ptr_spr_black_box_3->Render();
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
