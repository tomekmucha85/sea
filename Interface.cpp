#include <Interface.hpp>

void Interface::UseInterface(SDL_Event* ptr_my_event_handler)
{
	if (current_mode == interf_menu)
	{
		;
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