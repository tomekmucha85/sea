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

	bool is_connected_to_emo = false;

	if (IEE_EngineRemoteConnect(address.c_str(), composerPort) != EDK_OK) 
	{
		std::string errMsg = "Cannot connect to EmoComposer on [" +
			address + "]";
		printf("Cannot connect to EmoComposer!\n");
		//throw std::runtime_error(errMsg.c_str());
		printf("WARNING!\nDID NOT CONNECT TO EMO ENGINE.\n WARNING!\n");
	}
	else
	{
		is_connected_to_emo = true;
		printf("Connected to EmoComposer!\n");
	}

	//EMOTIV_END

	Level* first_level = Game::ptr_levels_factory->SpawnLevel(level_ninemazes);
	//Level* test_level = Game::ptr_levels_factory->SpawnLevel(level_test);
	Game::SetCurrentLevel(first_level);

	int looped_events = static_cast<int>(Game::ptr_current_level->cyclic_actions.size());
	printf("There are %d actions present in current event loop.\n", looped_events);

	TimerInterval* ptr_emotiv_bands_check = new TimerInterval(2000);

    while (!quit)
    {
		//Timer
		Timer::CalculateLoopDuration();
		//printf("Loop duration: %f.\n", Timer::loop_duration);
        //EMOTIV
		if (is_connected_to_emo)
		{
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

			if (ptr_emotiv_bands_check->CheckIfIntervalPassed())
			{
				IEE_GetAverageBandPowers(userID, IED_AF3, &theta, &alpha, &low_beta, &high_beta, &gamma);

				printf("Current band values: THETA: %f\n ALPHA: %f\n, LOW BETA: %f\n, HIGH_BETA: %f\n, GAMMA: %f\n.",
					theta, alpha, low_beta, high_beta, gamma);
			}
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
			else if (event_handler.type == SDL_KEYDOWN && event_handler.key.keysym.sym == SDLK_a)
			{
				Creature::ptr_current_main_charater->TurnLeft();
			}
			else if (event_handler.type == SDL_KEYDOWN && event_handler.key.keysym.sym == SDLK_d)
			{
				Creature::ptr_current_main_charater->TurnRight();
			}
			else if (event_handler.type == SDL_KEYDOWN && event_handler.key.keysym.sym == SDLK_w && event_handler.key.repeat == 0)
			{
		        Creature::ptr_current_main_charater->ThrustForward();
			}
			else if (event_handler.type == SDL_KEYDOWN && event_handler.key.keysym.sym == SDLK_s && event_handler.key.repeat == 0)
			{
				Creature::ptr_current_main_charater->ThrustBackward();
			}
			else if (event_handler.type == SDL_KEYDOWN && event_handler.key.keysym.sym == SDLK_UP && event_handler.key.repeat == 0)
			{
				Creature::ptr_current_main_charater->ThrustForward();
			}
			else if (event_handler.type == SDL_KEYDOWN && event_handler.key.keysym.sym == SDLK_DOWN && event_handler.key.repeat == 0)
			{
				Creature::ptr_current_main_charater->ThrustBackward();
			}
			else if (event_handler.type == SDL_KEYUP && event_handler.key.keysym.sym == SDLK_w && event_handler.key.repeat == 0)
			{
				Creature::ptr_current_main_charater->SetVelocity(0);
			}
			else if (event_handler.type == SDL_KEYUP && event_handler.key.keysym.sym == SDLK_s && event_handler.key.repeat == 0)
			{
				Creature::ptr_current_main_charater->SetVelocity(0);
			}
			else if (event_handler.type == SDL_KEYUP && event_handler.key.keysym.sym == SDLK_UP && event_handler.key.repeat == 0)
			{
				Creature::ptr_current_main_charater->SetVelocity(0);
			}
			else if (event_handler.type == SDL_KEYUP && event_handler.key.keysym.sym == SDLK_DOWN && event_handler.key.repeat == 0)
			{
				Creature::ptr_current_main_charater->SetVelocity(0);
			}
			else if (event_handler.type == SDL_KEYDOWN && event_handler.key.keysym.sym == SDLK_q && event_handler.key.repeat == 0)
			{
				Creature::ptr_current_main_charater->CastSpell(spell_vortex);
			}
        }

		//Perform cyclic actions from current level
		Game::ptr_current_level->PerformCyclicActions();

        //Clear screen
        SDL_RenderClear(Game::ptr_screen->renderer);

		//#TODO zrobiæ generator Creature, zadbac o destruktor i konstruktor kopiujacy
		Game::ptr_current_level->RenderAllPresentCreatures();
        //Update screen
        SDL_RenderPresent(Game::ptr_screen->renderer);
    }
    //Do cleanup
	delete ptr_emotiv_bands_check;
    Game::DestroyGame();
    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
	//Quit Emotiv subsystem
	if (is_connected_to_emo)
	{
		IEE_EngineDisconnect();
		IEE_EmoStateFree(eState);
		IEE_EmoEngineEventFree(eEvent);
	}
    return 0;
}
