#include <LevelMenu.hpp>

const Coordinates LevelMenu::MENU_HERO_POSITION = {0,0};

LevelMenu::LevelMenu()
{
	SetMyType(level_menu);
	ptr_component_containing_menu_actions = ptr_components_factory->SpawnLevelComponent(levco_core);
	possible_actions = possible_actions_menu_top_level;
	DisplayMenuActions();
	//To perform initial highlight.
	BrowseActions(north);
	//Move hero to desired position
	Creature::ptr_current_main_charater->SetPosition(menu_hero_start_position);
	//Make hero face south
	Creature::ptr_current_main_charater->SetAngleDegree(-90);
	//Display game title
	Coordinates title_upper_left_corner = { 10,380 };
	Coordinates title_upper_left_corner_2nd_row = { 10,480 };
	std::string title_text = "Don't eat anyone";
	std::string title_text_2nd_row = "   for a minute";
	ptr_initial_core_component->AddCreature(cre_writing,
		&title_upper_left_corner,
		merge, title_text,
		{ 255,0,0,255 },
		FontBank::ptr_font_nightbird_huge,
		1);
	ptr_initial_core_component->AddCreature(cre_writing,
		&title_upper_left_corner_2nd_row,
		merge, title_text_2nd_row,
		{ 255,0,0,255 },
		FontBank::ptr_font_nightbird_huge,
		1);
	//###############
    //# GUI SETUP
    //###############

	ptr_gui->AddComponentToDisplay(gui_printer);
}

void LevelMenu::DisplayMenuActions()
{
	//Print menu items on screen
	int menu_counter = 0;
	for (MenuAction* ptr_menu_action : possible_actions)
	{
		SDL_Color menu_action_color = DetermineDefaultFontColorForMenuAction(ptr_menu_action);
		Coordinates text_upper_left_corner = { 10, 10 + (menu_counter*spacing_between_menu_actions) };
		Creature* ptr_my_writing = ptr_component_containing_menu_actions->AddCreature(cre_writing,
			&text_upper_left_corner,
			merge,
			ptr_menu_action->text,
			menu_action_color);

		menu_counter++;
	}
}

SDL_Color LevelMenu::DetermineDefaultFontColorForMenuAction(MenuAction* ptr_my_action)
{
	if (ptr_my_action->is_enabled)
	{
		return default_menu_font_color;
	}
	else
	{
		return default_menu_disabled_action_font_color;
	}
}

void LevelMenu::DestroyCurrentMenuActions()
{
	ptr_component_containing_menu_actions->RemoveAllCreatures();
}

void LevelMenu::LoadMenuActionsSet(std::vector<MenuAction*> actions_set)
{
	DestroyCurrentMenuActions();
	possible_actions = actions_set;
	DisplayMenuActions();
	current_menu_position = 0;
	//To perform initial highlight.
	BrowseActions(north);
}

MenuAction* LevelMenu::FindPossibleMenuActionWithSpecificText(std::string my_text)
{
	MenuAction* result = nullptr;
	for (MenuAction* ptr_menu_action : possible_actions)
	{
		if (ptr_menu_action->text == my_text)
		{
			result = ptr_menu_action;
			break;
		}   
	}
	return result;
}

void LevelMenu::ChangeColorOfMenuActionExclusively(MenuAction* ptr_my_action, SDL_Color my_color)
{
	bool was_color_change_successfull = ChangeColorOfMenuAction(ptr_my_action, my_color);
	if (was_color_change_successfull)
	{
		for (Creature* ptr_my_creature : *(ptr_component_containing_menu_actions->TellPtrToCreaturesArray()))
		{
			if (ptr_my_creature->my_type == cre_writing)
			{
				CreatureWriting* ptr_my_writing = static_cast<CreatureWriting*>(ptr_my_creature);
				SDL_Color current_color = ptr_my_writing->TellColor();
				std::string writing_text = ptr_my_writing->TellText();
				// If we found another CreatureWriting which has the same color as desired
				if (writing_text != ptr_my_action->text)
				{
					if (current_color.r == my_color.r
						&& current_color.g == my_color.g
						&& current_color.b == my_color.b
						&& current_color.a == my_color.a)
					{
						//Set color to default, according to entry in MenuAction structure
						MenuAction* ptr_corresponding_menu_action = FindPossibleMenuActionWithSpecificText(writing_text);
						if (ptr_corresponding_menu_action != nullptr)
						{
							ptr_my_writing->SetNewTextColor(DetermineDefaultFontColorForMenuAction(ptr_corresponding_menu_action));
						}
					}
				}
			}
		}
	}
}

bool LevelMenu::ChangeColorOfMenuAction(MenuAction* ptr_my_action, SDL_Color my_color)
{
	bool was_color_change_successfull = false;
	for (Creature* ptr_my_creature : *(ptr_component_containing_menu_actions->TellPtrToCreaturesArray()))
	{
		if (ptr_my_creature->my_type == cre_writing)
		{
			CreatureWriting* ptr_my_writing = static_cast<CreatureWriting*>(ptr_my_creature);
			SDL_Color current_color = ptr_my_writing->TellColor();
			// If we found desired menu action
			if (ptr_my_writing->TellText() == ptr_my_action->text)
			{
				//Change color
				ptr_my_writing->SetNewTextColor(my_color);
				was_color_change_successfull = true;
			}
			else
			{
				Logger::Log("Action " + ptr_my_action->text + "was not displayed on screen currently.", debug_info);
			}
		}
	}
	return was_color_change_successfull;
}

void LevelMenu::HighlightMenuAction(MenuAction* ptr_my_action)
{
	ChangeColorOfMenuActionExclusively(ptr_my_action, default_menu_highlight_font_color);
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

	ChangeColorOfMenuActionExclusively(possible_actions[current_menu_position], default_menu_highlight_font_color);
}

bool LevelMenu::PerformSelectedAction()
{
	MenuAction* ptr_selected_action = nullptr;
	if (current_menu_position < possible_actions.size() &&
		possible_actions.size() > 0)
	{
		ptr_selected_action = possible_actions[current_menu_position];
	}
	else if (current_menu_position >= possible_actions.size() &&
		possible_actions.size() > 0)
	{
		ptr_selected_action = possible_actions[0];
	}
	else
	{
		Logger::Log("WARNING! No possible actions!");
	}

	if (ptr_selected_action != nullptr)
	{
		if (ptr_selected_action->is_enabled == true)
		{
			ExecuteTaskBoundToAction(ptr_selected_action);
			return true;
		}
		else
		{
			ptr_gui->PrintTextOnscreen("this action is disabled", 750);
			return false;
		}
	}
	else
	{
		return false;
	}

}

void LevelMenu::ExecuteTaskBoundToAction(MenuAction* ptr_my_action)
{
	if (ptr_my_action->text == menu_action_new_game.text)
	{
		FinishLevel(ending_exiting_menu);
	}
	else if (ptr_my_action->text == menu_action_calibration.text)
	{
		LoadMenuActionsSet(possible_actions_menu_calibration_level);
	}
	else if (ptr_my_action->text == menu_action_save_profile.text)
	{
		BCI::SaveUserProfile();
	}
	else if (ptr_my_action->text == menu_action_quit.text)
	{
		SDL_Event quit_event;
		quit_event.type = SDL_QUIT;
		SDL_PushEvent(&quit_event);
	}
	else if (ptr_my_action->text == menu_action_go_to_main_menu.text)
	{
		LoadMenuActionsSet(possible_actions_menu_top_level);
	}
	else if (ptr_my_action->text == menu_action_go_to_calibration_menu.text)
	{
		LoadMenuActionsSet(possible_actions_menu_calibration_level);
	}
	else if (ptr_my_action->text == menu_action_calibration_reset.text)
	{
		BCI::EraseAllTrainingData();
	}
	//###############################
	//# CALIBRATION WIZARD - 1st STEP
	//###############################
	else if (ptr_my_action->text == menu_action_calibration_wizard.text)
	{
		LoadMenuActionsSet(possible_actions_menu_calibration_wizard_1st_step);
		//All actions disabled except calibration start and return to menu
		ManageMenuActionsForNotCalibratedNeutralBCIExpression();
	}
	else if (ptr_my_action->text == menu_action_calibration_wizard_calibrate_neutral.text)
	{
		BCI::TrainNeutral();
		//Preparing set of actions to enable when calibration succeeds
		//Disabling proceeding to next calibration phase.
		ManageMenuActionsForNeutralBCICalibration();
		ptr_gui->PrintTextOnscreen("training will start soon");
	}
	else if (ptr_my_action->text == menu_action_calibration_wizard_calibrate_neutral_accept.text)
	{
		BCI::AcceptTraining();
		ManageMenuActionsForAcceptedBCICalibration();
	}
	else if (ptr_my_action->text == menu_action_calibration_wizard_calibrate_neutral_reject.text)
	{
		BCI::RejectTraining();
		ManageMenuActionsForRejectedBCICalibration();
	}
	//###############################
    //# CALIBRATION WIZARD - 2nd STEP
    //###############################
	else if (ptr_my_action->text == menu_action_calibration_wizard_calibrate_neutral_proceed.text)
	{
		LoadMenuActionsSet(possible_actions_menu_calibration_wizard_2nd_step);
		//All actions disabled except calibration start and return to menu
		ManageMenuActionsForNotCalibratedRaiseBrowBCIExpression();
	}
	else if (ptr_my_action->text == menu_action_calibration_wizard_calibrate_raise_brow.text)
	{
		BCI::TrainRaiseBrow();
		//Preparing set of actions to enable when calibration succeeds
		//Disabling proceeding to next calibration phase.
		ManageMenuActionsForRaiseBrowBCICalibration();
		ptr_gui->PrintTextOnscreen("training will start soon");
	}
	else if (ptr_my_action->text == menu_action_calibration_wizard_calibrate_raise_brow_accept.text)
	{
		BCI::AcceptTraining();
		ManageMenuActionsForAcceptedBCICalibration();
	}
	else if (ptr_my_action->text == menu_action_calibration_wizard_calibrate_raise_brow_reject.text)
	{
		BCI::RejectTraining();
		ManageMenuActionsForRejectedBCICalibration();
	}
	//###############################
    //# CALIBRATION WIZARD - 3rd STEP
    //###############################
	else if (ptr_my_action->text == menu_action_calibration_wizard_calibrate_raise_brow_proceed.text)
	{
		LoadMenuActionsSet(possible_actions_menu_calibration_wizard_3rd_step);
		//All actions disabled except calibration start and return to menu
		ManageMenuActionsForNotCalibratedClenchBCIExpression();
	}
	else if (ptr_my_action->text == menu_action_calibration_wizard_calibrate_clench.text)
	{
		BCI::TrainClench();
		//Preparing set of actions to enable when calibration succeeds
        //Disabling proceeding to next calibration phase.
		ManageMenuActionsForClenchBCICalibration();
		ptr_gui->PrintTextOnscreen("training will start soon");
	}
	else if (ptr_my_action->text == menu_action_calibration_wizard_calibrate_clench_accept.text)
	{
		BCI::AcceptTraining();
		ManageMenuActionsForAcceptedBCICalibration();
	}
	else if (ptr_my_action->text == menu_action_calibration_wizard_calibrate_clench_reject.text)
	{
		BCI::RejectTraining();
		ManageMenuActionsForRejectedBCICalibration();
	}
	//#################################
    //# CALIBRATION WIZARD - FINAL STEP
    //#################################
	else if (ptr_my_action->text == menu_action_calibration_wizard_finish.text)
	{
	    if (BCI::TrySwitchingToTrainedSig())
		{
			ptr_gui->PrintTextOnscreen("Calibration finished successfully.");
		}
		else
		{
			ptr_gui->PrintTextOnscreen("Unable to use calibration data. Please recalibrate.");
		}
		LoadMenuActionsSet(possible_actions_menu_calibration_level);
	}
}

void LevelMenu::DisableMenuAction(MenuAction* ptr_my_action)
{
	ptr_my_action->is_enabled = false;
	ChangeColorOfMenuAction(ptr_my_action, default_menu_disabled_action_font_color);
}

void LevelMenu::EnableMenuAction(MenuAction* ptr_my_action)
{
	ptr_my_action->is_enabled = true;
	ChangeColorOfMenuAction(ptr_my_action, default_menu_font_color);
}

void LevelMenu::ManageMenuActionsForNotCalibratedNeutralBCIExpression()
{
	for (MenuAction* ptr_menu_action : possible_actions_menu_calibration_wizard_1st_step)
	{
		//Disable all menu actions except calibration neutral and return option
		if (ptr_menu_action->text != menu_action_calibration_wizard_calibrate_neutral.text &&
			ptr_menu_action->text != menu_action_go_to_calibration_menu.text)
		{
			DisableMenuAction(ptr_menu_action);
		}
	}
}

void LevelMenu::ManageMenuActionsForNeutralBCICalibration()
{
	menu_actions_related_to_current_bci_calibration_outcome.clear();
	//Define which actions should be affected when training succeeds/is rejected/is accepted
	menu_actions_related_to_current_bci_calibration_outcome =
	{
		&menu_action_calibration_wizard_calibrate_neutral_accept,
		&menu_action_calibration_wizard_calibrate_neutral_reject,
	};
	ptr_menu_action_leading_to_next_calibration_stage = &menu_action_calibration_wizard_calibrate_neutral_proceed;
	ptr_menu_action_calibrate_chosen_expression = &menu_action_calibration_wizard_calibrate_neutral;
	DisableMenuAction(ptr_menu_action_calibrate_chosen_expression);
	DisableMenuAction(ptr_menu_action_leading_to_next_calibration_stage);
}

void LevelMenu::ManageMenuActionsForNotCalibratedRaiseBrowBCIExpression() 
{
	for (MenuAction* ptr_menu_action : possible_actions_menu_calibration_wizard_2nd_step)
	{
		//Disable all menu actions except calibration raise brow and return option
		if (ptr_menu_action->text != menu_action_calibration_wizard_calibrate_raise_brow.text &&
			ptr_menu_action->text != menu_action_go_to_calibration_menu.text)
		{
			DisableMenuAction(ptr_menu_action);
		}
	}
}

void LevelMenu::ManageMenuActionsForRaiseBrowBCICalibration()
{
	menu_actions_related_to_current_bci_calibration_outcome.clear();
	//Define which actions should be affected when training succeeds/is rejected/is accepted
	menu_actions_related_to_current_bci_calibration_outcome =
	{
		&menu_action_calibration_wizard_calibrate_raise_brow_accept,
		&menu_action_calibration_wizard_calibrate_raise_brow_reject,
	};
	ptr_menu_action_leading_to_next_calibration_stage = &menu_action_calibration_wizard_calibrate_raise_brow_proceed;
	ptr_menu_action_calibrate_chosen_expression = &menu_action_calibration_wizard_calibrate_raise_brow;
	DisableMenuAction(ptr_menu_action_calibrate_chosen_expression);
	DisableMenuAction(ptr_menu_action_leading_to_next_calibration_stage);
}

void LevelMenu::ManageMenuActionsForNotCalibratedClenchBCIExpression()
{
	for (MenuAction* ptr_menu_action : possible_actions_menu_calibration_wizard_3rd_step)
	{
		//Disable all menu actions except calibration clench and return option
		if (ptr_menu_action->text != menu_action_calibration_wizard_calibrate_clench.text &&
			ptr_menu_action->text != menu_action_go_to_calibration_menu.text)
		{
			DisableMenuAction(ptr_menu_action);
		}
	}
}

void LevelMenu::ManageMenuActionsForClenchBCICalibration()
{
	menu_actions_related_to_current_bci_calibration_outcome.clear();
	//Define which actions should be affected when training succeeds/is rejected/is accepted
	menu_actions_related_to_current_bci_calibration_outcome =
	{
		&menu_action_calibration_wizard_calibrate_clench_accept,
		&menu_action_calibration_wizard_calibrate_clench_reject,
	};
	ptr_menu_action_leading_to_next_calibration_stage = &menu_action_calibration_wizard_finish;
	ptr_menu_action_calibrate_chosen_expression = &menu_action_calibration_wizard_calibrate_clench;
	DisableMenuAction(ptr_menu_action_calibrate_chosen_expression);
	DisableMenuAction(ptr_menu_action_leading_to_next_calibration_stage);
}

void LevelMenu::ManageMenuActionsForAcceptedBCICalibration()
{
	//Disable accepting/rejecting training results
	for (MenuAction* ptr_my_action : menu_actions_related_to_current_bci_calibration_outcome)
	{
		DisableMenuAction(ptr_my_action);
	}
	EnableMenuAction(ptr_menu_action_calibrate_chosen_expression);
}

void LevelMenu::ManageMenuActionsForRejectedBCICalibration()
{
	//Disable accepting/rejecting training results
	for (MenuAction* ptr_my_action : menu_actions_related_to_current_bci_calibration_outcome)
	{
		DisableMenuAction(ptr_my_action);
	}
	EnableMenuAction(ptr_menu_action_calibrate_chosen_expression);
	DisableMenuAction(ptr_menu_action_leading_to_next_calibration_stage);
}

void LevelMenu::NotifyOfBciEvent(BCIEvent my_event)
{
	if (my_event == bci_event_training_failed)
	{
		ptr_gui->PrintTextOnscreen("calibration failed!");;
	}
	else if (my_event == bci_event_training_success)
	{
		for (MenuAction* ptr_my_action : menu_actions_related_to_current_bci_calibration_outcome)
		{
			EnableMenuAction(ptr_my_action);
		}
		ptr_gui->PrintTextOnscreen("calibration succeeded!");
	}
	else if (my_event == bci_event_training_start)
	{
		ptr_gui->PrintTextOnscreen("training started!");
	}
	else if (my_event == bci_event_training_completed)
	{
		ptr_gui->PrintTextOnscreen("signatures updated!");
		//Enable proceeding to next calibration stage
		EnableMenuAction(ptr_menu_action_leading_to_next_calibration_stage);
	}
	else if (my_event == bci_event_training_erased)
	{
		ptr_gui->PrintTextOnscreen("data erased!");
	}
	else if (my_event == bci_event_smile)
	{
		Logger::Log("Smile caught!", debug_info);
	}
	else if (my_event == bci_event_clench)
	{
		Logger::Log("Clench caught!", debug_info);
	}
	else if (my_event == bci_event_raise_brow)
	{
		Logger::Log("Raise brow caught!", debug_info);
	}
}