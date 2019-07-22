#include <LevelMenu.hpp>

const Coordinates LevelMenu::MENU_HERO_POSITION = {0,0};

LevelMenu::LevelMenu()
{
	SetMyType(level_menu);
	Coordinates bground_position = { 100,100 };
	Creature* ptr_bground = ptr_initial_core_component->AddCreature(cre_blue_bground, &bground_position, merge);
	ptr_bground->MakeMeNotObstacle();
	possible_actions = possible_actions_menu_top_level;
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
		Logger::Log("Unsupported direction for browing menu actions.");
	}

	if (possible_actions[current_menu_position] == menu_new_game)
	{
		Logger::Log("NEW GAME");
	}
	else if (possible_actions[current_menu_position] == menu_calibration)
	{
		Logger::Log("CALIBRATION");
	}
	else if (possible_actions[current_menu_position] == menu_quit)
	{
		Logger::Log("QUIT");
	}
}

void LevelMenu::PerformSelectedAction()
{
	MenuActions selected_action = menu_none;
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

	if (selected_action == menu_new_game)
	{
		Win();
	}
	else if (selected_action == menu_calibration)
	{
		;
	}

}