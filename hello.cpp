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
#include <LevelTest.hpp>
#include <LevelNineMazes.hpp>
#include <Screen.hpp>
#include <Texture.hpp>
#include <TextureBank.hpp>
#include <Sprite.hpp>
#include <Creature.hpp>
#include <LevelComponent.hpp>
#include <Timer.hpp>
#include <TimerInterval.hpp>
#include <Interface.hpp>

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
	Interface* ptr_game_interface = new Interface();
	ptr_game_interface->SetInterfaceMode(interf_free);

	//EMOTIV

	EmoEngineEventHandle eEvent = IEE_EmoEngineEventCreate();
	EmoStateHandle eState = IEE_EmoStateCreate();
	unsigned int userID = 0;
	const unsigned short composerPort = 1726;
	int state = 0;
	std::string  address = "127.0.0.1";

	bool is_connected_to_emo = false;

	/*if (IEE_EngineConnect() == EDK_OK)
	{
		is_connected_to_emo = true;
		printf("Successfully connected to Emotiv device!\n");
	}
	else*/ if (IEE_EngineRemoteConnect(address.c_str(), composerPort) == EDK_OK) 
	{
		is_connected_to_emo = true;
		printf("Connected to EmoComposer!\n");
	}
	else
	{
		std::string errMsg = "Cannot connect neither to Emotiv device nor to EmoComposer on [" +
			address + "]";
		printf("Cannot connect to EmoComposer!\n");
		//throw std::runtime_error(errMsg.c_str());
		printf("WARNING!\nDID NOT CONNECT TO EMO ENGINE.\n WARNING!\n");
	}

	//EMOTIV_END

	Game::PrepareSingleLevel(level_ninemazes);

	int looped_events = static_cast<int>(Game::ptr_current_level->cyclic_actions.size());
	printf("There are %d actions present in current event loop.\n", looped_events);

	TimerInterval* ptr_emotiv_bands_check = new TimerInterval(2000);

	int cooldown = 0;

    while (!quit)
    {
		//Cooldown decreasing
		if (cooldown > 0)
		{
			if (cooldown == 1)
			{
				printf("COOLDOWN PASSED!\n");
			}
			cooldown--;
		}
		//Timer
		Timer::CalculateLoopDuration();
		//printf("Loop duration: %f.\n", Timer::loop_duration);

		//Perform cyclic actions from current level
		Game::ptr_current_level->PerformCyclicActions();

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
					float timestamp = IS_GetTimeFromStart(eState);
					//printf("Got update at %f!\n", timestamp);
					if (IS_FacialExpressionIsLeftWink(eState) == 1)
					{
						if (cooldown == 0)
						{
							printf("Wink!\n");
							Creature::ptr_current_main_charater->CastSpell(spell_vortex);
							cooldown = 200;
						}
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

				/*printf("Current band values: THETA: %f\n ALPHA: %f\n, LOW BETA: %f\n, HIGH_BETA: %f\n, GAMMA: %f\n.",
					theta, alpha, low_beta, high_beta, gamma);*/
			}
		}
		//EMOTIV_END

        //Handle events on queue
        while(SDL_PollEvent(&event_handler) != 0)
        {
			ptr_game_interface->UseInterface(&event_handler);
            //User requests quit
            if(event_handler.type == SDL_QUIT)
            {
                quit = true;
            }
			else if (event_handler.type == SDL_KEYDOWN && event_handler.key.keysym.sym == SDLK_p && event_handler.key.repeat == 0)
			{
				Game::ptr_current_level->Pause();
			}
			else if (event_handler.type == SDL_KEYDOWN && event_handler.key.keysym.sym == SDLK_l && event_handler.key.repeat == 0)
			{
				Game::ptr_current_level->UnPause();
			}
			if (Game::ptr_current_level->TellIfPaused() == false) //Actions possible to perform only if game is not paused
			{
				if (event_handler.type == SDL_KEYDOWN && event_handler.key.keysym.sym == SDLK_a && event_handler.key.repeat == 0)
				{
					Creature::ptr_current_main_charater->TurnLeft();
				}
				else if (event_handler.type == SDL_KEYDOWN && event_handler.key.keysym.sym == SDLK_d && event_handler.key.repeat == 0)
				{
					Creature::ptr_current_main_charater->TurnRight();
				}
				else if (event_handler.type == SDL_KEYUP && event_handler.key.keysym.sym == SDLK_a && event_handler.key.repeat == 0)
				{
					Creature::ptr_current_main_charater->TurnStop();
				}
				else if (event_handler.type == SDL_KEYUP && event_handler.key.keysym.sym == SDLK_d && event_handler.key.repeat == 0)
				{
					Creature::ptr_current_main_charater->TurnStop();
				}
				else if (event_handler.type == SDL_KEYDOWN && event_handler.key.keysym.sym == SDLK_w && event_handler.key.repeat == 0)
				{
					Creature::ptr_current_main_charater->ThrustForward(200);
				}
				else if (event_handler.type == SDL_KEYDOWN && event_handler.key.keysym.sym == SDLK_s && event_handler.key.repeat == 0)
				{
					Creature::ptr_current_main_charater->ThrustBackward(150);
				}
				else if (event_handler.type == SDL_KEYUP && event_handler.key.keysym.sym == SDLK_w && event_handler.key.repeat == 0)
				{
					Creature::ptr_current_main_charater->SetVelocity(0);
				}
				else if (event_handler.type == SDL_KEYUP && event_handler.key.keysym.sym == SDLK_s && event_handler.key.repeat == 0)
				{
					Creature::ptr_current_main_charater->SetVelocity(0);
				}
				else if (event_handler.type == SDL_KEYDOWN && event_handler.key.keysym.sym == SDLK_q && event_handler.key.repeat == 0)
				{
					Creature::ptr_current_main_charater->CastSpell(spell_vortex);
				}
				else if (event_handler.type == SDL_KEYDOWN && event_handler.key.keysym.sym == SDLK_e && event_handler.key.repeat == 0)
				{
					Creature::ptr_current_main_charater->CastSpell(spell_open_gate);
				}
				else if (event_handler.type == SDL_KEYDOWN && event_handler.key.keysym.sym == SDLK_r && event_handler.key.repeat == 0)
				{
					Game::PrepareSingleLevel(level_ninemazes);
				}
				else if (event_handler.type == SDL_KEYDOWN && event_handler.key.keysym.sym == SDLK_t && event_handler.key.repeat == 0)
				{
					Game::PrepareSingleLevel(level_test);
				}
				else if (event_handler.type == SDL_KEYDOWN && event_handler.key.keysym.sym == SDLK_v && event_handler.key.repeat == 0)
				{
					Game::ptr_current_level->ptr_initial_navgrid_component->SetVisibilityForAllCreatures(true);
				}
				else if (event_handler.type == SDL_KEYDOWN && event_handler.key.keysym.sym == SDLK_b && event_handler.key.repeat == 0)
				{
					Game::ptr_current_level->ptr_initial_navgrid_component->SetVisibilityForAllCreatures(false);
				}
				else if (event_handler.type == SDL_KEYDOWN && event_handler.key.keysym.sym == SDLK_n && event_handler.key.repeat == 0)
				{
					Game::ptr_current_level->should_nodes_be_reconnected = true;
				}
				else if (event_handler.type == SDL_KEYDOWN && event_handler.key.keysym.sym == SDLK_z && event_handler.key.repeat == 0)
				{
					Coordinates test_point = {-1000, -1000};
					Creature::ptr_current_main_charater->SetBehaviorMode(beh_go_towards_fixed_point, &test_point);
				}
				else if (event_handler.type == SDL_KEYDOWN && event_handler.key.keysym.sym == SDLK_x && event_handler.key.repeat == 0)
				{
					Creature::ptr_current_main_charater->SetBehaviorMode(beh_idle);
				}
			}
        }

        //Clear screen
        SDL_RenderClear(Game::ptr_screen->renderer);

		//#TODO zadbac o destruktor i konstruktor kopiujacy
		Game::ptr_current_level->RenderAllPresentCreatures();
		Game::ptr_current_level->RenderGui();
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
