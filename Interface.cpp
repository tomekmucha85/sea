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
			Game::ptr_current_level->Pause();
		}
		else if (current_mode == interf_guided)
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
			else if (ptr_my_event_handler->type == SDL_KEYDOWN &&
				ptr_my_event_handler->key.keysym.sym == SDLK_z &&
				ptr_my_event_handler->key.repeat == 0)
			{
				printf("Wink - going to free mode.\n");
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
			else if (ptr_my_event_handler->key.keysym.sym == SDLK_z &&
				ptr_my_event_handler->key.repeat == 0)
			{
			    printf("Wink.\n");
				if (ptr_my_event_handler->type == SDL_KEYDOWN)
				{
					if (ptr_double_wink_timer != nullptr)
					{
						if (ptr_double_wink_timer->CheckIfIntervalPassed())
						{
							printf("Too late for double wink!\n");
							if (Creature::ptr_current_main_charater->TellVelocity() != 0)
							{
								Creature::ptr_current_main_charater->SetVelocity(0);
							}
							else
							{
								Creature::ptr_current_main_charater->ThrustForward();
							}
						}
						else
						{
							printf("Caught double wink!\n");
							Creature::ptr_current_main_charater->TurnLeft();
						}
						delete ptr_double_wink_timer;
						ptr_double_wink_timer = nullptr;
					}
					else
					{
						printf("Double wink timer ptr is nullptr.\n");
						ptr_double_wink_timer = new TimerInterval(double_wink_timeout);
					}
				}
				else if (ptr_my_event_handler->type == SDL_KEYUP)
				{
					if (Creature::ptr_current_main_charater->TellTurnVelocity() != 0)
					{
						Creature::ptr_current_main_charater->TurnStop();
					}
				}
				else
				{
					printf("What the hell has just happened?\n");
				}
			}
			else if (ptr_my_event_handler->type == SDL_KEYDOWN &&
				ptr_my_event_handler->key.keysym.sym == SDLK_x &&
				ptr_my_event_handler->key.repeat == 0)
			{
				printf("Eyes closed - going to guided mode.\n");
				SetInterfaceMode(interf_guided);
			}
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