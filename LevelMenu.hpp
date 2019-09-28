#ifndef LEVEL_MENU_HPP
#define LEVEL_MENU_HPP
#include <Level.hpp>
#include <Brain.hpp>

class LevelMenu : public Level
{
private:

	MenuAction menu_action_new_game = { "new game", true };
	MenuAction menu_action_calibration = { "calibration", true };
	MenuAction menu_action_save_profile = { "save user profile", false };
	MenuAction menu_action_quit = { "quit", true };
	MenuAction menu_action_calibration_wizard = {"calibration wizard", true};
	MenuAction menu_action_calibration_wizard_calibrate_neutral = {"calibrate your neutral expression", true};
	MenuAction menu_action_calibration_wizard_calibrate_neutral_accept = { "accept neutral expression training", true };
	MenuAction menu_action_calibration_wizard_calibrate_neutral_reject = { "reject neutral expression training", true };
	MenuAction menu_action_calibration_wizard_calibrate_neutral_proceed = { "proceed to smile calibration", true };
	MenuAction menu_action_calibration_wizard_calibrate_smile = { "calibrate your smile", true };
	MenuAction menu_action_calibration_wizard_calibrate_smile_accept = { "accept smile training", true};
	MenuAction menu_action_calibration_wizard_calibrate_smile_reject = { "reject smile training", true };
	MenuAction menu_action_calibration_wizard_calibrate_smile_proceed = { "proceed to clench calibration", true };
	MenuAction menu_action_calibration_wizard_calibrate_clench = { "calibrate your clench", true };
	MenuAction menu_action_calibration_wizard_calibrate_clench_accept = { "accept clench training", true };
	MenuAction menu_action_calibration_wizard_calibrate_clench_reject = { "reject clench training", true };
	MenuAction menu_action_calibration_wizard_finish = { "finish calibration", true };
	MenuAction menu_action_go_to_main_menu = { "return to main menu", true };
	MenuAction menu_action_go_to_calibration_menu = { "return to calibration menu", true };
	MenuAction menu_action_calibration_reset = { "reset calibration data", true };

	double spacing_between_menu_actions = 40;
	SDL_Color default_menu_font_color = {255,0,0,255}; // red
	SDL_Color default_menu_highlight_font_color = { 255,255,0,255 }; // yellow
	SDL_Color default_menu_disabled_action_font_color = { 100,100,100,255 }; //grey
	static const Coordinates MENU_HERO_POSITION;
	std::vector<MenuAction*> possible_actions = {};
	std::vector<MenuAction*> possible_actions_menu_top_level = { 
		&menu_action_new_game, 
		&menu_action_calibration, 
		&menu_action_save_profile, 
		&menu_action_quit };
	std::vector<MenuAction*> possible_actions_menu_calibration_level = {
		&menu_action_calibration_wizard,
		&menu_action_calibration_reset,
		&menu_action_go_to_main_menu };
	std::vector<MenuAction*> possible_actions_menu_calibration_wizard_1st_step = {
		&menu_action_calibration_wizard_calibrate_neutral,
		&menu_action_calibration_wizard_calibrate_neutral_accept, 
		&menu_action_calibration_wizard_calibrate_neutral_reject,
	    &menu_action_calibration_wizard_calibrate_neutral_proceed,
	    &menu_action_go_to_calibration_menu};
	std::vector<MenuAction*> possible_actions_menu_calibration_wizard_2nd_step = {
		&menu_action_calibration_wizard_calibrate_smile,
		&menu_action_calibration_wizard_calibrate_smile_accept,
		&menu_action_calibration_wizard_calibrate_smile_reject,
		&menu_action_calibration_wizard_calibrate_smile_proceed,
		&menu_action_go_to_calibration_menu };
	std::vector<MenuAction*> possible_actions_menu_calibration_wizard_3rd_step = {
	    &menu_action_calibration_wizard_calibrate_clench,
	    &menu_action_calibration_wizard_calibrate_clench_accept,
	    &menu_action_calibration_wizard_calibrate_clench_reject,
	    &menu_action_calibration_wizard_finish,
	    &menu_action_go_to_calibration_menu };
	unsigned int current_menu_position = 0;
	Coordinates menu_hero_start_position = {static_cast<double>(Screen::TellScreenWidth()) - 40, 
		static_cast<double>(Screen::TellScreenHeight() - 60)};

	//BCI CALIBRATION
	bool is_bci_calibration_in_progress = false;
	std::vector<MenuAction*> menu_actions_related_to_current_bci_calibration_outcome = {};
	MenuAction* ptr_menu_action_leading_to_next_calibration_stage = nullptr;
	MenuAction* ptr_menu_action_calibrate_chosen_expression = nullptr;
	LevelComponent* ptr_component_containing_menu_actions = ptr_initial_core_component;
public:
	LevelMenu();
	void BrowseActions(Directions my_direction);
	void ChangeColorOfMenuActionExclusively(MenuAction* ptr_my_action, SDL_Color my_color);
	bool ChangeColorOfMenuAction(MenuAction* ptr_my_action, SDL_Color my_color);
	void HighlightMenuAction(MenuAction* ptr_my_action);
	SDL_Color DetermineDefaultFontColorForMenuAction(MenuAction* ptr_my_action);
	bool PerformSelectedAction();
	void ExecuteTaskBoundToAction(MenuAction* ptr_my_action);
	void LoadMenuActionsSet(std::vector<MenuAction*> actions_set);
	void DisplayMenuActions();
	void DestroyCurrentMenuActions();
	void DisableMenuAction(MenuAction* ptr_my_action);
	void EnableMenuAction(MenuAction* ptr_my_action);
	void ManageMenuActionsForAcceptedBCICalibration();
	void ManageMenuActionsForRejectedBCICalibration();
	void ManageMenuActionsForNotCalibratedNeutralBCIExpression();
	void ManageMenuActionsForNotCalibratedSmileBCIExpression();
	void ManageMenuActionsForNotCalibratedClenchBCIExpression();
	void ManageMenuActionsForNeutralBCICalibration();
	void ManageMenuActionsForSmileBCICalibration();
	void ManageMenuActionsForClenchBCICalibration();
	MenuAction* FindPossibleMenuActionWithSpecificText(std::string my_text);
	//BCI usage
	void NotifyOfBciEvent(BCIEvent my_event);
};

#endif //LEVEL_MENU_HPP