#ifndef GUI_HPP
#define GUI_HPP
#include <vector>
#include <Sprite.hpp>
#include <Creature.hpp>
#include <TrueTypeWriting.hpp>
#include <FactorySpawningSprites.hpp>
#include <CustomDefinedTypes.hpp>

class GUI
{
    private:
		FactorySpawningSprites* ptr_factory_spawning_sprites = nullptr;
		std::vector<VisualComponent*> gui_components = {};
		std::vector<std::function<void(GUI*)>> cyclic_actions = {};
		//Hunger bar
		Coordinates hunger_bar_center = { static_cast<double>(Screen::TellScreenWidth()/2), 25 };
		Sprite* ptr_hunger_bar = nullptr;
		//Winning timer
		Coordinates winning_timer_upper_left_corner = { 400, 70 };
		TrueTypeWriting* ptr_writing_winning_timer = nullptr;
		Uint32 winning_timer_value_in_seconds = 0;
		//On-screen prints
		Coordinates onscreen_printer_upper_left_corner = { 10, static_cast<double>(Screen::TellScreenHeight() - 40) };
		Uint32 onscreen_printer_text_default_time_to_live_in_miliseconds = 2000;
		TimerCountdown* ptr_timer_for_onscreen_printer = nullptr;
		TrueTypeWriting* ptr_onscreen_printer = nullptr;
		SDL_Color onscreen_printer_default_color = {0,40,200,255}; //Blue

    public:
		GUI();
		~GUI();
		void AddComponentToDisplay(GuiElement my_element);
		void RenderComponents();
		void ManageForCreature(Creature* ptr_creature);
		void PerformCyclicActions();
		void ManageHungerBar(Creature* ptr_creature);
		void HungerBarSetChargeLevel(int new_level);
		void ManageWinningTimer(Uint32 my_time_passed);
		void PrintTextOnscreen(std::string my_text, Uint32 time_to_live=0);
		bool CheckIfOnscreenPrinterTextExpired();
		void ManageOnscreenPrinter();


		//################################
		//# COMMON LAMBDAS
		//################################

		//Cyclic action to fire all triggers hit by hero
		std::function<void(GUI*)> func_manage_onscreen_printer = [](GUI* ptr_gui)
		{
			ptr_gui->ManageOnscreenPrinter();
		};

};

#endif // GUI_HPP

