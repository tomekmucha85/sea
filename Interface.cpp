#include <Interface.hpp>


void Interface::UseInterface(SDL_Event* ptr_my_event_handler)
{
	//USER IN MENU
	if (current_mode == interf_menu)
	{
		;
	}
	//USER INSIDE GAME
	else if (current_mode == interf_guided || current_mode == interf_free)
	{
		if (ptr_my_event_handler->type == SDL_KEYDOWN &&
			ptr_my_event_handler->key.keysym.sym == SDLK_p &&
			ptr_my_event_handler->key.repeat == 0)
		{
			//#TODO implement pause
			Game::ptr_current_level->Pause();
		}
		else if (ptr_my_event_handler->type == SDL_KEYDOWN && 
			     ptr_my_event_handler->key.keysym.sym == SDLK_a && 
			     ptr_my_event_handler->key.repeat == 0)
		{
			Creature::ptr_current_main_charater->TurnLeft();
		}
		else if (ptr_my_event_handler->type == SDL_KEYDOWN && 
			     ptr_my_event_handler->key.keysym.sym == SDLK_d && 
			     ptr_my_event_handler->key.repeat == 0)
		{
			Creature::ptr_current_main_charater->TurnRight();
		}
		else if (ptr_my_event_handler->type == SDL_KEYUP && 
			     ptr_my_event_handler->key.keysym.sym == SDLK_a && 
			     ptr_my_event_handler->key.repeat == 0)
		{
			Creature::ptr_current_main_charater->TurnStop();
		}
		else if (ptr_my_event_handler->type == SDL_KEYUP && 
			     ptr_my_event_handler->key.keysym.sym == SDLK_d && 
			     ptr_my_event_handler->key.repeat == 0)
		{
			Creature::ptr_current_main_charater->TurnStop();
		}
		else if (ptr_my_event_handler->type == SDL_KEYDOWN && 
			     ptr_my_event_handler->key.keysym.sym == SDLK_w && 
			     ptr_my_event_handler->key.repeat == 0)
		{
			Creature::ptr_current_main_charater->ThrustForward(400);
		}
		else if (ptr_my_event_handler->type == SDL_KEYDOWN && 
			     ptr_my_event_handler->key.keysym.sym == SDLK_s && 
			     ptr_my_event_handler->key.repeat == 0)
		{
			Creature::ptr_current_main_charater->ThrustBackward(150);
		}
		else if (ptr_my_event_handler->type == SDL_KEYUP && 
			     ptr_my_event_handler->key.keysym.sym == SDLK_w && 
			     ptr_my_event_handler->key.repeat == 0)
		{
			Creature::ptr_current_main_charater->SetVelocity(0);
		}
		else if (ptr_my_event_handler->type == SDL_KEYUP && 
			     ptr_my_event_handler->key.keysym.sym == SDLK_s && 
			     ptr_my_event_handler->key.repeat == 0)
		{
			Creature::ptr_current_main_charater->SetVelocity(0);
		}
		else if (ptr_my_event_handler->type == SDL_KEYDOWN && 
			     ptr_my_event_handler->key.keysym.sym == SDLK_q && 
			     ptr_my_event_handler->key.repeat == 0)
		{
			Creature::ptr_current_main_charater->CastSpell(spell_vortex);
		}
		else if (ptr_my_event_handler->type == SDL_KEYDOWN && 
			     ptr_my_event_handler->key.keysym.sym == SDLK_r && 
			     ptr_my_event_handler->key.repeat == 0)
		{
			Game::PrepareSingleLevel(level_ninemazes);
		}
		else if (ptr_my_event_handler->type == SDL_KEYDOWN && 
			     ptr_my_event_handler->key.keysym.sym == SDLK_t && 
			     ptr_my_event_handler->key.repeat == 0)
		{
			Game::PrepareSingleLevel(level_test);
		}
		else if (ptr_my_event_handler->type == SDL_KEYDOWN && 
			     ptr_my_event_handler->key.keysym.sym == SDLK_v && 
			     ptr_my_event_handler->key.repeat == 0)
		{
			Game::ptr_current_level->ptr_initial_navgrid_component->SetVisibilityForAllCreatures(true);
		}
		else if (ptr_my_event_handler->type == SDL_KEYDOWN && 
			     ptr_my_event_handler->key.keysym.sym == SDLK_b && 
			     ptr_my_event_handler->key.repeat == 0)
		{
			Game::ptr_current_level->ptr_initial_navgrid_component->SetVisibilityForAllCreatures(false);
		}
		else if (ptr_my_event_handler->type == SDL_KEYDOWN && 
			     ptr_my_event_handler->key.keysym.sym == SDLK_n && 
			     ptr_my_event_handler->key.repeat == 0)
		{
			Game::ptr_current_level->should_nodes_be_reconnected = true;
		}
		else if (ptr_my_event_handler->type == SDL_KEYDOWN && 
			     ptr_my_event_handler->key.keysym.sym == SDLK_f &&
			     ptr_my_event_handler->key.repeat == 0)
        {
			printf("PRESSED F.\n");
	        //Creature::ptr_current_main_charater->SetBehaviorMode(beh_follow_closest_carrier);
			Creature::ptr_current_main_charater->RequestBehaviorMode(beh_follow_closest_carrier);
        }
        else if (ptr_my_event_handler->type == SDL_KEYDOWN && 
			     ptr_my_event_handler->key.keysym.sym == SDLK_x && 
			     ptr_my_event_handler->key.repeat == 0)
        {
		    //Creature::ptr_current_main_charater->SetBehaviorMode(beh_idle);
			Creature::ptr_current_main_charater->RequestBehaviorMode(beh_idle);
        }

		//FOR SPECIFIC MODES INSIDE GAME
		if (current_mode == interf_guided)
		{
			if (ptr_my_event_handler->type == SDL_KEYDOWN &&
				ptr_my_event_handler->key.keysym.sym == SDLK_g &&
				ptr_my_event_handler->key.repeat == 0)
			{
				printf("Key hit in guided mode.\n");
			}
			else if (ptr_my_event_handler->type == SDL_KEYDOWN &&
				ptr_my_event_handler->key.keysym.sym == SDLK_h &&
				ptr_my_event_handler->key.repeat == 0)
			{
				printf("Mode change.\n");
				SetInterfaceMode(interf_free);
			}
		}
		else if (current_mode == interf_free)
		{
			//printf("Free mode!\n");
			if (ptr_my_event_handler->type == SDL_KEYDOWN &&
				ptr_my_event_handler->key.keysym.sym == SDLK_g &&
				ptr_my_event_handler->key.repeat == 0)
			{
				printf("Key hit in free mode.\n");
			}
			else if (ptr_my_event_handler->type == SDL_KEYDOWN &&
				ptr_my_event_handler->key.keysym.sym == SDLK_h &&
				ptr_my_event_handler->key.repeat == 0)
			{
				printf("Mode change.\n");
				SetInterfaceMode(interf_guided);
			}
		}
	}

}

void Interface::UseInterface(std::string bci_detection)
{
	if (current_mode == interf_menu)
	{
		;
	}
	else if (current_mode == interf_guided)
	{
		;
	}
	else if (current_mode == interf_free)
	{
		if (bci_detection == "smile")
		{
			printf("Interface got smile!\n");
		}
		else if (bci_detection == "clench")
		{
			printf("Interface got clench!\n");
		}
	}
}

void Interface::UseInterface(EmoEngineEventHandle my_eEvent)
{
	if (current_mode == interf_menu)
	{
		;
	}
	else if (current_mode == interf_guided)
	{
		;
	}
	else if (current_mode == interf_free)
	{
		;
	}
}

void Interface::SetInterfaceMode(InterfaceMode my_mode)
{
	current_mode = my_mode;
}

InterfaceMode Interface::TellInterfaceMode()
{
	return current_mode;
}