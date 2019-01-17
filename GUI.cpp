#include <GUI.hpp>

GUI::GUI()
{
	ptr_factory_spawning_sprites = new FactorySpawningSprites();
	ptr_mana_bar = ptr_factory_spawning_sprites->SpawnSprite(spr_gui_mana_bar, &mana_bar_center);
	gui_components.push_back(ptr_mana_bar);
}

GUI::~GUI()
{
	delete ptr_factory_spawning_sprites;
	delete ptr_mana_bar;
}

void GUI::ManageForCreature(Creature* ptr_creature)
{
	ManageManaBar(ptr_creature);
}

void GUI::ManageManaBar(Creature* ptr_creature)
{
	int mana_level = ptr_creature->TellManaLevel();
	ManaBarSetChargeLevel(mana_level);
}

void GUI::ManaBarSetChargeLevel(int new_level)
{
	SpriteGuiManaBar* ptr_specific_mana_bar = static_cast<SpriteGuiManaBar*>(ptr_mana_bar);
    ptr_specific_mana_bar->SetTextureClipWithCharges(new_level);
}

void GUI::RenderComponents()
{
	for (Sprite* ptr_component : gui_components)
	{
		ptr_component->Render();
	}
}