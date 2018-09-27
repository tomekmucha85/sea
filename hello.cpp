#include <CommonHeader.hpp>

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

	SDL_Rect smoke_position = { 500,200,0,0 };
	SDL_Rect* ptr_smoke_position = &smoke_position;
	Creature* cre_black_smoke_1 = Creature::SpawnCreature(cre_black_smoke, ptr_smoke_position);

    SDL_Rect hero_position = {320,240,0,0};
    SDL_Rect* ptr_hero_position = &hero_position;
	Creature* cre_heros = Creature::SpawnCreature(cre_clawy, ptr_hero_position);
	cre_heros->MakeMeMainCharacter();

	SDL_Rect box_position = { 100,100,0,0 };
	SDL_Rect* ptr_box_position = &box_position;
	Creature* cre_box = Creature::SpawnCreature(cre_flying_box, ptr_box_position);

	SDL_Rect box_position_2 = { 300,150,0,0 };
	SDL_Rect* ptr_box_position_2 = &box_position_2;
	Creature* cre_box2 = Creature::SpawnCreature(cre_flying_box, ptr_box_position_2);

	SDL_Rect box_position_3 = { 500,150,0,0 };
	SDL_Rect* ptr_box_position_3 = &box_position_3;
	Creature* cre_box3 = Creature::SpawnCreature(cre_flying_box, ptr_box_position_3);

	SDL_Rect box_4_position = { 25,25,0,0 };
	SDL_Rect* ptr_box_4_position = &box_4_position;
	Creature* cre_box4 = Creature::SpawnCreature(cre_flying_box,ptr_box_4_position);


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
                    case SDLK_UP: cre_heros->MoveForward(); break;
                    case SDLK_DOWN: cre_heros->MoveBackward(); break;
                    case SDLK_LEFT: cre_heros->StrafeLeft(); break;
                    case SDLK_RIGHT: cre_heros->StrafeRight(); break;

                    case SDLK_w: cre_heros->MoveForward(); break;
                    case SDLK_s: cre_heros->MoveBackward(); break;
                    case SDLK_a: cre_heros->TurnLeft(); break;
                    case SDLK_d: cre_heros->TurnRight(); break;
                }
            }
        }

        //Test moving creatures
		
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
		
		//Test background animation
		//#TODO napisaæ wspóln¹ funkcjê dla animacji
		//#TODO napisaæ wspólne nag³ówki dla d³ugich list include
		cre_black_smoke_1->ptr_creature_sprite->SmokeAnimation();

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
