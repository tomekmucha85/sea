#ifndef LEVEL_MENU_HPP
#define LEVEL_MENU_HPP
#include <Level.hpp>

class LevelMenu : public Level
{
private:
	std::string menu_action_new_game = "new game";
	std::string menu_action_calibration = "calibration";
	std::string menu_action_quit = "quit";
	std::string menu_action_calibrate_smile = "calibrate your smile";
	std::string menu_action_calibrate_smile_accept = "accept smile training";
	std::string menu_action_calibrate_smile_reject = "reject smile training";
	std::string menu_action_calibrate_smile_restart = "restart smile training";
	std::string menu_action_calibrate_clench = "calibrate your clench";
	std::string menu_action_calibrate_clench_accept = "accept clench training";
	std::string menu_action_calibrate_clench_reject = "reject clench training";
	std::string menu_action_calibrate_clench_restart = "restart clench training";
	std::string menu_action_go_to_main_menu = "return to main menu";
	std::string menu_action_go_to_calibration_menu = "return to calibration menu";
	double spacing_between_menu_positions = 40;
	SDL_Color default_menu_font_color = {255,0,0,255}; // red
	SDL_Color default_menu_highlight_font_color = { 255,255,0,255 }; // yellow
	static const Coordinates MENU_HERO_POSITION;
	std::vector<std::string> possible_actions = {};
	std::vector<std::string> possible_actions_menu_top_level = { menu_action_new_game, menu_action_calibration, menu_action_quit };
	std::vector<std::string> possible_actions_menu_calibration_level = {menu_action_calibrate_clench, menu_action_calibrate_smile, menu_action_go_to_main_menu };
	std::vector<std::string> possible_actions_menu_clench_calibration_level = { menu_action_calibrate_clench_accept, 
		menu_action_calibrate_clench_reject,
	    menu_action_calibrate_clench_restart,
	    menu_action_go_to_calibration_menu};
	std::vector<std::string> possible_actions_menu_smile_calibration_level = { menu_action_calibrate_smile_accept,
	menu_action_calibrate_smile_reject,
	menu_action_calibrate_smile_restart,
	menu_action_go_to_calibration_menu };
	unsigned int current_menu_position = 0;
	LevelComponent* ptr_component_containing_menu_positions = ptr_initial_core_component;
public:
	LevelMenu();
	void BrowseActions(Directions my_direction);
	void HighlightAMenuAction(std::string my_action);
	void PerformSelectedAction();
	void LoadMenuActionsSet(std::vector<std::string> actions_set);
	void DisplayMenuActions();
	void DestroyCurrentMenuActions();
};

#endif //LEVEL_MENU_HPP