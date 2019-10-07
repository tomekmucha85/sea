#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <string>
#include <stdexcept>
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

const std::string USAGE = "\nUSAGE:\nsea.exe [-b physical|virtual|none] [-f on|off]\nWhere:\n-b is BCI mode\n-f is fullscreen mode\n";

BCIMode ParseBCIArgument(std::string argument)
{
	if (argument == "physical")
	{
		return bci_mode_physical;
	}
	else if (argument == "virtual")
	{
		return bci_mode_virtual;
	}
	else if (argument == "none")
	{
		return bci_mode_none;
	}
	else
	{
		std::cout << "Invalid switch " + argument + "\n";
		std::cout << USAGE;
		throw std::invalid_argument("Invalid switch");
	}
}

bool ParseFullscreenArgument(std::string argument)
{
	if (argument == "on") 
	{
		return true;
	}
	else if (argument == "off")
	{
		return false;
	}
	else
	{
		std::cout << "Invalid switch " + argument + "\n";
		std::cout << USAGE;
		throw std::invalid_argument("Invalid switch");
	}
}

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
	int max_argument_count = 5;
	printf("Arguments count: %d\n", argc);
	bool is_fullscreen_evaluated = false;
	bool is_BCI_evaluated = false;
	for (int i = 0; i < argc; i++)
	{
		printf("Received argument: ");
		printf(args[i]);
		printf("\n");
	}
	if (argc > max_argument_count)
	{
		std::cout << USAGE;
		return 1;
	}
	printf("Checked arguments count.\n");
    // Beginning with number 1, 'cause 0 holds executable name.
	for (int i = 1; i < argc; i++)
	{
		printf("Will parse an argument.\n");
		std::string argument(args[i]);
		std::string message = "Parsing argument: " + argument + "\n";
		printf(message.c_str());
		if (argument == "-b" && !is_BCI_evaluated && !is_fullscreen_evaluated)
		{
			is_BCI_evaluated = true;
		}
		else if (argument == "-f" && !is_BCI_evaluated && !is_fullscreen_evaluated)
		{
			is_fullscreen_evaluated = true;
		}
		else if (is_BCI_evaluated)
		{
			chosen_bci_mode = ParseBCIArgument(argument);
			is_BCI_evaluated = false;
		}
		else if (is_fullscreen_evaluated)
		{
			is_fullscreen_evaluated = false;
			Screen::should_be_fullscreen = ParseFullscreenArgument(argument);
		}
		else
		{
			std::cout << USAGE;
			return 1;
		}
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
