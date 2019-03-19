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
	//const unsigned short composerPort = 1726;
	int state = 0;
	std::string  address = "127.0.0.1";

	bool is_connected_to_emo = false;

	/*if (IEE_EngineConnect() == EDK_OK)
	{
		is_connected_to_emo = true;
		printf("Successfully connected to Emotiv device!\n");
	}*/
	/*else if (IEE_EngineRemoteConnect(address.c_str(), composerPort) == EDK_OK) 
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
	}*/
	//IEE_FacialExpressionGetTrainedSignatureAvailable()

	

	//EMOTIV_END

	Game::PrepareSingleLevel(level_ninemazes);

	int looped_events = static_cast<int>(Game::ptr_current_level->cyclic_actions.size());
	printf("There are %d actions present in current event loop.\n", looped_events);
	printf("Main hero: %p.\n", Creature::ptr_current_main_charater);
	TimerInterval* ptr_emotiv_bands_check = new TimerInterval(2000);

	int cooldown = 0;

    while (!quit)
    {
		//Cooldown decreasing
		/*if (cooldown > 0)
		{
			if (cooldown == 1)
			{
				printf("COOLDOWN PASSED!\n");
			}
			cooldown--;
		}*/
		//Timer
		Timer::CalculateLoopDuration();
		//printf("Loop duration: %f.\n", Timer::loop_duration);

		//Perform cyclic actions from current level
		Game::ptr_current_level->PerformCyclicActions();

        //EMOTIV
		if (is_connected_to_emo)
		{
			//printf("Connected to emo.\n");
			state = IEE_EngineGetNextEvent(eEvent);
			if (state != EDK_NO_EVENT)
			{
				if (state == EDK_OK)
				{
					IEE_Event_t eventType = IEE_EmoEngineEventGetType(eEvent);
					//printf("Got event!\n");
					//IEE_EmoEngineEventGetUserId(eEvent, &userID);
					if (eventType == IEE_EmoStateUpdated)
					{
						IEE_EmoEngineEventGetEmoState(eEvent, eState);
						float upperFaceAmp = IS_FacialExpressionGetUpperFaceActionPower(eState);
						float lowerFaceAmp = IS_FacialExpressionGetLowerFaceActionPower(eState);
						//printf("State updated.\n");
						IEE_FacialExpressionAlgo_t upperFaceType =
							IS_FacialExpressionGetUpperFaceAction(eState);
						IEE_FacialExpressionAlgo_t lowerFaceType =
							IS_FacialExpressionGetLowerFaceAction(eState);

						if (lowerFaceAmp > 0)
						{
							if (lowerFaceType == FE_SMIRK_LEFT)
							{
								printf("Left smirk!\n");
							}
							else if (lowerFaceType == FE_SMIRK_RIGHT)
							{
								printf("Right smirk!\n");
							}
							else if (lowerFaceType == FE_SMILE)
							{
								printf("Smile\n");
							}
						}

					}
					//
					else if (eventType == IEE_UserAdded || eventType == IEE_UserRemoved)
					{
						printf("Got user related event.\n");
					}
					else if (eventType == IEE_InternalStateChanged)
					{
						printf("Internal state change.\n");
					}
					else if (eventType == IEE_ProfileEvent)
					{
						printf("Profile event.\n");
					}
					else if (eventType == IEE_MentalCommandEvent)
					{
						printf("Mental command.\n");
					}
					else if (eventType == IEE_FacialExpressionEvent)
					{
						printf("Facial expression event.\n");
					}
					else
					{
						printf("Some other event.\n");
					}
				}
				else
				{
					printf("Unexpected EmoEngineBehavior!\n");
				}

				//state = IEE_EngineGetNextEvent(eEvent);
			}
			else
			{
				//printf("No event!\n");
			}
		}
			
		/*if (is_connected_to_emo)
		{
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
						printf("Wink!\n");
						if (cooldown == 0)
						{
							printf("Wink!\n");
							Creature::ptr_current_main_charater->CastSpell(spell_vortex);
							cooldown = 200;
						}
					}
					else if (IS_FacialExpressionIsLookingLeft(eState) == 1)
					{
						printf("Looking left.\n");
					}
					else if (IS_FacialExpressionIsEyesOpen(eState) == 0)
					{
						printf("Eyes closed.\n");
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
		}*/
		
		//EMOTIV_END

        //Handle events on queue
        while(SDL_PollEvent(&event_handler) != 0)
        {
            //User requests quit
            if(event_handler.type == SDL_QUIT)
            {
                quit = true;
            }
			ptr_game_interface->UseInterface(&event_handler);
        }

        //Clear screen
        SDL_RenderClear(Game::ptr_screen->renderer);

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
