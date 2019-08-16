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
		//Hunger bar
		Coordinates hunger_bar_center = { 400, 25 };
		Sprite* ptr_hunger_bar = nullptr;
		//Winning timer
		Coordinates winning_timer_upper_left_corner = { 400, 70 };
		TrueTypeWriting* ptr_writing_winning_timer = nullptr;
		Uint32 winning_timer_value_in_seconds = 0;


    public:
		GUI();
		~GUI();
		void AddComponentToDisplay(GuiElement my_element);
		void RenderComponents();
		void ManageForCreature(Creature* ptr_creature);
		//void ManageManaBar(Creature* ptr_creature);
		//void ManaBarSetChargeLevel(int new_level);
		void ManageHungerBar(Creature* ptr_creature);
		void HungerBarSetChargeLevel(int new_level);
		void ManageWinningTimer(Uint32 my_time_passed);

};

#endif // GUI_HPP

