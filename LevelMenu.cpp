#include <LevelMenu.hpp>

const Coordinates LevelMenu::MENU_HERO_POSITION = {0,0};

LevelMenu::LevelMenu()
{
	SetMyType(level_menu);
	Coordinates bground_position = { 100,100 };
	Creature* ptr_bground = ptr_initial_core_component->AddCreature(cre_blue_bground, 
		&bground_position, 
		merge);
	ptr_bground->MakeMeNotObstacle();

	possible_actions = possible_actions_menu_top_level;

	int menu_counter = 0;
	for (std::string menu_choice : possible_actions)
	{
		Coordinates text_upper_left_corner = { 10, 10 + (menu_counter*spacing_between_menu_positions) };

		Creature* ptr_my_writing = ptr_initial_core_component->AddCreature(cre_writing,
			&text_upper_left_corner,
			merge,
			menu_choice);

		menu_counter++;
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
		Logger::Log("Unsupported direction for browing menu actions.");
	}

	if (possible_actions[current_menu_position] == menu_action_new_game)
	{
		Logger::Log("NEW GAME");
	}
	else if (possible_actions[current_menu_position] == menu_action_calibration)
	{
		Logger::Log("CALIBRATION");
	}
	else if (possible_actions[current_menu_position] == menu_action_quit)
	{
		Logger::Log("QUIT");
	}
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
		Win();
	}
	else if (selected_action == menu_action_calibration)
	{
		;
	}
	else if (selected_action == menu_action_quit)
	{
		;
	}
}