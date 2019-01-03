#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <string>
#include "Iedk.h"
#include <IedkErrorCode.h>
#include <Game.hpp>
#include <Level.hpp>
#include <LevelNineMazes.hpp>
#include <Screen.hpp>
#include <Texture.hpp>
#include <TextureBank.hpp>
#include <Sprite.hpp>
#include <Creature.hpp>
#include <LevelComponent.hpp>
#include <Timer.hpp>
#include <TimerInterval.hpp>

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

	//EMOTIV

	EmoEngineEventHandle eEvent = IEE_EmoEngineEventCreate();
	EmoStateHandle eState = IEE_EmoStateCreate();
	unsigned int userID = 0;
	const unsigned short composerPort = 1726;
	int option = 0;
	int state = 0;
	std::string  address = "127.0.0.1";

	if (IEE_EngineRemoteConnect(address.c_str(), composerPort) != EDK_OK) 
	{
		std::string errMsg = "Cannot connect to EmoComposer on [" +
			address + "]";
		printf("Cannot connect to EmoComposer!\n");
		throw std::runtime_error(errMsg.c_str());
	}
	else
	{
		printf("Connected to EmoComposer!\n");
	}

	//EMOTIV_END



	Level* first_level = Game::ptr_levels_factory->SpawnLevel(level_ninemazes);
	Game::SetCurrentLevel(first_level);

	int looped_events = Game::ptr_current_level->cyclic_actions.size();
	printf("There are %d actions present in current event loop.\n", looped_events);

	//unsigned int timer = 0;
	TimerInterval* ptr_emotiv_bands_check = new TimerInterval(2000);

    while (!quit)
    {
		//Timer
		Timer::CalculateLoopDuration();

        //EMOTIV
		state = IEE_EngineGetNextEvent(eEvent);

		// New event needs to be handled
		if (state == EDK_OK)
		{
			IEE_Event_t eventType = IEE_EmoEngineEventGetType(eEvent);
			IEE_EmoEngineEventGetUserId(eEvent, &userID);
			// Log the EmoState if it has been updated
			if (eventType == IEE_EmoStateUpdated)
			{
				IEE_EmoEngineEventGetEmoState(eEvent, eState);
				const float timestamp = IS_GetTimeFromStart(eState);
				printf("Got update at %f!\n", timestamp);
				if (IS_FacialExpressionIsBlink(eState) == 1)
				{
					printf("Blink!\n");
				}
			}
		}
		else if (state != EDK_NO_EVENT) 
		{
			std::cout << "Internal error in Emotiv Engine!" << std::endl;
			break;
		}
		double theta = 0;
		double alpha = 0;
		double low_beta = 0;
		double high_beta = 0;
		double gamma = 0;

		if(ptr_emotiv_bands_check->CheckIfIntervalPassed())
		{
			IEE_GetAverageBandPowers(userID, IED_AF3, &theta, &alpha, &low_beta, &high_beta, &gamma);

			printf("Current band values: THETA: %d\n ALPHA: %d\n, LOW BETA: %d\n, HIGH_BETA: %d\n, GAMMA: %d\n.",
				theta, alpha, low_beta, high_beta, gamma);
		}

		//EMOTIV_END

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
				    case SDLK_UP: Creature::ptr_current_main_charater->MoveForward(); break;
                    case SDLK_DOWN: Creature::ptr_current_main_charater->MoveBackward(); break;
                    case SDLK_LEFT: Creature::ptr_current_main_charater->StrafeLeft(); break;
                    case SDLK_RIGHT: Creature::ptr_current_main_charater->StrafeRight(); break;
                    case SDLK_w: Creature::ptr_current_main_charater->MoveForward();
					break;
                    case SDLK_s: Creature::ptr_current_main_charater->MoveBackward();
					break;
                    case SDLK_a: Creature::ptr_current_main_charater->TurnLeft(); break;
                    case SDLK_d: Creature::ptr_current_main_charater->TurnRight(); break;
					//case SDLK_1: ptr_maze_a->ClearMaze(); break;
					//case SDLK_2: ptr_maze_a->GenerateMaze(); break;
					//case SDLK_3: my_maze_b->ClearMaze(); break;
					//case SDLK_4: my_maze_b->GenerateMaze(); break;
					case SDLK_q: Creature::ptr_current_main_charater->CastSpell(spell_vortex); break;
                }
            }
        }

		//Test background animation
		//#TODO napisaæ wspóln¹ funkcjê dla animacji

		//Perform cyclic actions from current level
		Game::ptr_current_level->PerformCyclicActions();
        //Clear screen
        SDL_RenderClear(Game::ptr_screen->renderer);

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
	IEE_EngineDisconnect();
	IEE_EmoStateFree(eState);
	IEE_EmoEngineEventFree(eEvent);

    return 0;
}
