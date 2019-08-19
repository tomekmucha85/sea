#include <LevelMenu.hpp>

const Coordinates LevelMenu::MENU_HERO_POSITION = {0,0};

LevelMenu::LevelMenu()
{
	SetMyType(level_menu);
	/*Coordinates bground_position = { 100,100 };
	Creature* ptr_bground = ptr_initial_core_component->AddCreature(cre_blue_bground, 
		&bground_position, 
		merge);
	ptr_bground->MakeMeNotObstacle();*/
	ptr_component_containing_menu_positions = ptr_components_factory->SpawnLevelComponent(levco_core);
	possible_actions = possible_actions_menu_top_level;
	DisplayMenuActions();
	//To perform initial highlight.
	BrowseActions(north);
}

void LevelMenu::DisplayMenuActions()
{
	//Print menu items on screen
	int menu_counter = 0;
	for (std::string menu_choice : possible_actions)
	{
		Coordinates text_upper_left_corner = { 10, 10 + (menu_counter*spacing_between_menu_positions) };

		Creature* ptr_my_writing = ptr_component_containing_menu_positions->AddCreature(cre_writing,
			&text_upper_left_corner,
			merge,
			menu_choice,
			default_menu_font_color);

		menu_counter++;
	}
}

void LevelMenu::DestroyCurrentMenuActions()
{
	ptr_component_containing_menu_positions->RemoveAllCreatures();
}

void LevelMenu::LoadMenuActionsSet(std::vector<std::string> actions_set)
{
	DestroyCurrentMenuActions();
	possible_actions = actions_set;
	DisplayMenuActions();
	current_menu_position = 0;
	//To perform initial highlight.
	BrowseActions(north);
}

void LevelMenu::HighlightAMenuAction(std::string my_action)
{
	for (Creature* ptr_my_creature: *(ptr_component_containing_menu_positions->TellPtrToCreaturesArray()))
	{
		if (ptr_my_creature->my_type == cre_writing)
		{
			CreatureWriting* ptr_my_writing = static_cast<CreatureWriting*>(ptr_my_creature);
			SDL_Color current_color = ptr_my_writing->TellColor();
			// If we found desired menu action
			if (ptr_my_writing->TellText() == my_action)
			{
				//Highlight
				ptr_my_writing->SetNewTextColor(default_menu_highlight_font_color);
			}
			// If we found another menu action which is currently highlighted
			else if (current_color.r == default_menu_highlight_font_color.r
				&& current_color.g == default_menu_highlight_font_color.g
				&& current_color.b == default_menu_highlight_font_color.b
				&& current_color.a == default_menu_highlight_font_color.a)
			{
				//Disable highlight
				ptr_my_writing->SetNewTextColor(default_menu_font_color);
			}
		}
	}
}

void LevelMenu::BrowseActions(Directions my_direction)
{
	if (my_direction == north)
	{
		if (current_menu_position > 0)
		{
			current_menu_position--;
		}
	}
	else if (my_direction == south)
	{
		if (current_menu_position < possible_actions.size() - 1)
		{
			current_menu_position++;
		}
	}
	else
	{
		Logger::Log("Unsupported direction for browsing menu actions.");
	}

	HighlightAMenuAction(possible_actions[current_menu_position]);
}

void LevelMenu::PerformSelectedAction()
{
	std::string selected_action = "";
	if (current_menu_position < possible_actions.size() &&
		possible_actions.size() > 0)
	{
		selected_action = possible_actions[current_menu_position];
	}
	else if (current_menu_position >= possible_actions.size() &&
		possible_actions.size() > 0)
	{
		selected_action = possible_actions[0];
	}
	else
	{
		Logger::Log("WARNING! No possible actions!");
	}

	if (selected_action == menu_action_new_game)
	{
		FinishLevel(ending_exiting_menu);
	}
	else if (selected_action == menu_action_calibration)
	{
		LoadMenuActionsSet(possible_actions_menu_calibration_level);
	}
	else if (selected_action == menu_action_save_profile)
	{
		BCI::SaveUserProfile();
	}
	else if (selected_action == menu_action_quit)
	{
		SDL_Event quit_event;
		quit_event.type = SDL_QUIT;
		SDL_PushEvent(&quit_event);
	}
	else if (selected_action == menu_action_go_to_main_menu)
	{
		LoadMenuActionsSet(possible_actions_menu_top_level);
	}
	else if (selected_action == menu_action_calibrate_clench)
	{
		LoadMenuActionsSet(possible_actions_menu_clench_calibration_level);
	}
	else if (selected_action == menu_action_calibrate_smile)
	{
		LoadMenuActionsSet(possible_actions_menu_smile_calibration_level);
	}
	else if (selected_action == menu_action_calibrate_smile_start)
	{
		printf("Selected smile training.\n");
		BCI::TrainSmile();
	}
	else if (selected_action == menu_action_calibrate_smile_accept)
	{
		BCI::AcceptTraining();
	}
	else if (selected_action == menu_action_calibrate_smile_reject)
	{
		BCI::RejectTraining();
	}
	else if (selected_action == menu_action_go_to_calibration_menu)
	{
		LoadMenuActionsSet(possible_actions_menu_calibration_level);
	}
}