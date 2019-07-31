#ifndef LEVEL_MENU_HPP
#define LEVEL_MENU_HPP
#include <Level.hpp>

class LevelMenu : public Level
{
private:
	std::string menu_action_new_game = "new game";
	std::string menu_action_calibration = "calibration";
	std::string menu_action_quit = "quit";
	double spacing_between_menu_positions = 40;
	static const Coordinates MENU_HERO_POSITION;
	std::vector<std::string> possible_actions = {};
	std::vector<std::string> possible_actions_menu_top_level = { menu_action_new_game, menu_action_calibration, menu_action_quit };
	unsigned int current_menu_position = 0;
public:
	LevelMenu();
	void BrowseActions(Directions my_direction);
	void PerformSelectedAction();
};

#endif //LEVEL_MENU_HPP