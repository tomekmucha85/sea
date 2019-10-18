#include <Interface.hpp>

void Interface::SetModeAppropriateToLevelType()
/*
    Menu is just a kind of level, but with a whole different controls set.
*/
{
	LevelType current_level_type = Game::ptr_current_level->TellMyType();
	if (current_level_type == level_menu)
	{
		current_mode = interf_menu;
	}
	else
	{
		current_mode = interf_game;
	}
}

void Interface::UseInterface(SDL_Event* ptr_my_event_handler)
{
	SetModeAppropriateToLevelType();
	//USER IN MENU
	if (current_mode == interf_menu)
	{
	    if (ptr_my_event_handler->type == SDL_KEYDOWN &&
		    ptr_my_event_handler->key.keysym.sym == SDLK_ESCAPE &&
		    ptr_my_event_handler->key.repeat == 0)
	    {
		    Game::ptr_current_level->FinishLevel(ending_exiting_menu);
	    }
		else if (ptr_my_event_handler->type == SDL_KEYDOWN &&
			ptr_my_event_handler->key.keysym.sym == SDLK_w &&
			ptr_my_event_handler->key.repeat == 0)
		{
			Logger::Log("GOING UP ONE MENU POSITION.");
			Game::ptr_current_level->BrowseActions(north);
		}
		else if (ptr_my_event_handler->type == SDL_KEYDOWN &&
			ptr_my_event_handler->key.keysym.sym == SDLK_s &&
			ptr_my_event_handler->key.repeat == 0)
		{
			Logger::Log("GOING DOWN ONE MENU POSITION.");
			Game::ptr_current_level->BrowseActions(south);
		}
		else if (ptr_my_event_handler->type == SDL_KEYDOWN &&
			ptr_my_event_handler->key.keysym.sym == SDLK_RETURN &&
			ptr_my_event_handler->key.repeat == 0)
		{
			Logger::Log("PERFORMING ACTION.");
			Game::ptr_current_level->PerformSelectedAction();
		}
		else if (ptr_my_event_handler->type == SDL_KEYDOWN &&
			ptr_my_event_handler->key.keysym.sym == SDLK_t &&
			ptr_my_event_handler->key.repeat == 0)
		{
			Logger::Log("Trying to set trained signature.");
			BCI::TrySwitchingToTrainedSig();
		}
		else if (ptr_my_event_handler->type == SDL_KEYDOWN &&
			ptr_my_event_handler->key.keysym.sym == SDLK_SPACE &&
			ptr_my_event_handler->key.repeat == 0)
		{
			Logger::Log("Aborting finishing action.");
			Game::ptr_current_level->EndFinishingActionAbruptly();
		}
	}
	//USER INSIDE GAME
	else if (current_mode == interf_game)
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
			Creature::ptr_current_main_charater->ThrustForward();
		}
		else if (ptr_my_event_handler->type == SDL_KEYDOWN && 
			     ptr_my_event_handler->key.keysym.sym == SDLK_s && 
			     ptr_my_event_handler->key.repeat == 0)
		{
			Creature::ptr_current_main_charater->ThrustBackward();
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
			ptr_my_event_handler->key.keysym.sym == SDLK_DELETE &&
			ptr_my_event_handler->key.repeat == 0)
		{
			Creature::ptr_current_main_charater->Attack(attack_melee);
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
			ptr_my_event_handler->key.keysym.sym == SDLK_ESCAPE &&
			ptr_my_event_handler->key.repeat == 0)
		{
			Game::ptr_current_level->FinishLevel(ending_entering_menu);
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
			Game::ptr_current_level->ptr_gui->PrintBigMessaeOnScreenCenter("Hi there!");
		}
		else if (ptr_my_event_handler->type == SDL_KEYDOWN && 
			     ptr_my_event_handler->key.keysym.sym == SDLK_f &&
			     ptr_my_event_handler->key.repeat == 0)
        {
	        Creature::ptr_current_main_charater->SetBehaviorMode(beh_follow_closest_creature);
        }
		else if (ptr_my_event_handler->type == SDL_KEYDOWN &&
		         ptr_my_event_handler->key.keysym.sym == SDLK_y &&
		         ptr_my_event_handler->key.repeat == 0)
		{
		    Creature* ptr_addressee = Creature::ptr_current_main_charater->TellFollowedCreature();
			ConversationalMessage message = Creature::ptr_current_main_charater->ConstructConversationalMessage("hi!");
			Creature::ptr_current_main_charater->SendConversationalMessage(ptr_addressee, message);
		}
        else if (ptr_my_event_handler->type == SDL_KEYDOWN && 
			     ptr_my_event_handler->key.keysym.sym == SDLK_x && 
			     ptr_my_event_handler->key.repeat == 0)
        {
			Creature::ptr_current_main_charater->AlertLivingCreaturesInRadius();
			Creature::ptr_current_main_charater->RequestBehaviorMode(beh_sleep, 
				Behavior::BEH_PAT_STALKER_SLEEP_TIME);
        }
		else if (ptr_my_event_handler->type == SDL_KEYDOWN &&
		ptr_my_event_handler->key.keysym.sym == SDLK_0 &&
		ptr_my_event_handler->key.repeat == 0)
		{
		    Logger::SetDebugLevel(debug_none);
		}
		else if (ptr_my_event_handler->type == SDL_KEYDOWN &&
		ptr_my_event_handler->key.keysym.sym == SDLK_1 &&
		ptr_my_event_handler->key.repeat == 0)
		{
		    Logger::SetDebugLevel(debug_info);
		}
		else if (ptr_my_event_handler->type == SDL_KEYDOWN &&
		ptr_my_event_handler->key.keysym.sym == SDLK_2 &&
		ptr_my_event_handler->key.repeat == 0)
		{
		    Logger::SetDebugLevel(debug_full);
		}
		else if (ptr_my_event_handler->type == SDL_KEYDOWN &&
		ptr_my_event_handler->key.keysym.sym == SDLK_SPACE &&
		ptr_my_event_handler->key.repeat == 0)
		{
		Logger::Log("Aborting finishing action.");
		Game::ptr_current_level->EndFinishingActionAbruptly();
		}
	}
}

void Interface::UseInterface(BCIEvent my_event)
{
	Game::ptr_current_level->NotifyOfBciEvent(my_event);
}

void Interface::SetInterfaceMode(InterfaceMode my_mode)
{
	current_mode = my_mode;
}

InterfaceMode Interface::TellInterfaceMode()
{
	return current_mode;
}