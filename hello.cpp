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

    Game::InitializeGame();
	Interface* ptr_game_interface = new Interface();
	ptr_game_interface->SetInterfaceMode(interf_menu);
	BCI* ptr_bci_instance = new BCI(bci_none);

	Game::PrepareSingleLevel(level_menu);

	int looped_events = static_cast<int>(Game::ptr_current_level->cyclic_actions.size());
	printf("There are %d actions present in current event loop.\n", looped_events);
	printf("Main hero: %p.\n", Creature::ptr_current_main_charater);
	//TimerInterval* ptr_emotiv_bands_check = new TimerInterval(2000);

	int cooldown = 0;

	//TEXT TEST
	SDL_Texture* ptr_writing = Texture::LoadTextureFromRenderedText("dupa", Game::ptr_screen->renderer, FontBank::ptr_font_doom);


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

		//Handle BCI events on queque
		if (ptr_bci_instance->WhatBCIIsConnected() != bci_none)
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
        SDL_RenderClear(Game::ptr_screen->renderer);

		Game::ptr_current_level->RenderAllPresentCreatures();
		Game::ptr_current_level->RenderGui();

		//TEXT TEST
		int writing_w, writing_h;
		SDL_QueryTexture(ptr_writing, NULL, NULL, &writing_w, &writing_h);
		SDL_Rect result = { 0,0,writing_w, writing_h };
		SDL_RenderCopyEx(Game::ptr_screen->renderer, ptr_writing, nullptr, &result, 0, 0, SDL_FLIP_NONE);

        //Update screen
        SDL_RenderPresent(Game::ptr_screen->renderer);

		//Check if level should be finished
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
