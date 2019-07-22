#ifndef LEVEL_MENU_HPP
#define LEVEL_MENU_HPP
#include <Level.hpp>

class LevelMenu : public Level
{
private:
	static const Coordinates MENU_HERO_POSITION;
	std::vector<MenuActions> possible_actions = {};
	std::vector<MenuActions> possible_actions_menu_top_level = { menu_new_game, menu_calibration, menu_quit };
	//MenuActions currently_selected_action = menu_none;
	unsigned int current_menu_position = 0;
public:
	LevelMenu();
	void BrowseActions(Directions my_direction);
	void PerformSelectedAction();
};

#endif //LEVEL_MENU_HPP