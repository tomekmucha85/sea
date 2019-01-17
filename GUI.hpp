#ifndef GUI_HPP
#define GUI_HPP
#include <vector>
#include <Sprite.hpp>
#include <Creature.hpp>

#include <FactorySpawningSprites.hpp>
#include <CustomDefinedTypes.hpp>

class GUI
{
    private:
		FactorySpawningSprites* ptr_factory_spawning_sprites = nullptr;
		std::vector<Sprite*> gui_components = {};
		Coordinates mana_bar_center = {320, 25};
		Sprite* ptr_mana_bar = nullptr;
    public:
		GUI();
		~GUI();
		void RenderComponents();
		void ManageForCreature(Creature* ptr_creature);
		void ManageManaBar(Creature* ptr_creature);
		void ManaBarSetChargeLevel(int new_level);

};

#endif // GUI_HPP

