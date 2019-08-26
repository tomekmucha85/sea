#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
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
#include <Brain.hpp>

//###################
//Variables
//###################

int main(int argc, char* args[])
{

    //Main loop flag
    bool quit = false;

    //Event handler
    SDL_Event event_handler;

	//BCI mode requested when no mode is explicitly chosen
	BCIMode default_bci_mode = bci_mode_virtual;
	BCIMode chosen_bci_mode = default_bci_mode;

	//Arguments handling
	std::string usage = "\nUSAGE:\nsea.exe [-b physical|virtual|none]\n";
	int max_argument_count = 3;
	printf("Arguments count: %d\n", argc);
	for (int i = 0; i < argc; i++)
	{
		printf("Received argument: ");
		printf(args[i]);
		printf("\n");
	}
	if (argc > max_argument_count)
	{
		std::cout << usage;
		return 1;
	}
	else if (argc == 3)
	{
		std::string argument_0(args[0]);
		std::string argument_1(args[1]);
		std::string argument_2(args[2]);
		if (argument_1 == "-b")
		{
			if (argument_2 == "physical")
			{
				chosen_bci_mode = bci_mode_physical;
			}
			else if (argument_2 == "virtual")
			{
				chosen_bci_mode = bci_mode_virtual;
			}
			else if (argument_2 == "none")
			{
				chosen_bci_mode = bci_mode_none;
			}
		}
		else
		{
			std::cout << "Invalid switch: \n";
			std::cout << usage ;
			return 1;
		}
	}
	else if (argc == 1)
	{
		chosen_bci_mode = default_bci_mode;
	}
	else
	{
		std::cout << usage;
		return 1;
	}


    Game::InitializeGame();
	Interface* ptr_game_interface = new Interface();
	ptr_game_interface->SetInterfaceMode(interf_menu);
	BCI* ptr_bci_instance = new BCI(chosen_bci_mode);

	Game::PrepareSingleLevel(level_menu);

	int looped_events = static_cast<int>(Game::ptr_current_level->cyclic_actions.size());
	printf("There are %d actions present in current event loop.\n", looped_events);
	printf("Main hero: %p.\n", Creature::ptr_current_main_charater);
	//TimerInterval* ptr_emotiv_bands_check = new TimerInterval(2000);

	int cooldown = 0;

    while (!quit)
    {
		//Timer
		Timer::CalculateLoopDuration();
		//printf("Loop duration: %f.\n", Timer::loop_duration);

		//Perform cyclic actions from current level
		Game::ptr_current_level->PerformCyclicActions();

		//Handle BCI events on queque
		if (ptr_bci_instance->WhatBCIIsConnected() != bci_mode_none)
		{
			ptr_game_interface->UseInterface(ptr_bci_instance->GetNextBCIEvent());
		}

        //Handle non-BCI events on queue
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
        SDL_RenderClear(Screen::renderer);

		Game::ptr_current_level->RenderAllPresentCreatures();
		Game::ptr_current_level->RenderGui();

        //Update screen
        SDL_RenderPresent(Screen::renderer);

		//Check if level should be loaded
		Game::SwitchBetweenLevelsIfNeeded();

    }
    //Do cleanup
	//delete ptr_emotiv_bands_check;
	delete ptr_bci_instance;
    Game::DestroyGame();
    //Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
    SDL_Quit();
    return 0;
}
