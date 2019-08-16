#include <GUI.hpp>

GUI::GUI()
{
	//Hunger bar
	ptr_factory_spawning_sprites = new FactorySpawningSprites();
	ptr_hunger_bar = ptr_factory_spawning_sprites->SpawnSprite(spr_gui_mana_bar, &hunger_bar_center);
	//Winning timer
	ptr_writing_winning_timer = new TrueTypeWriting(
		std::to_string(0), 
		&winning_timer_upper_left_corner,
		nullptr,
		{ 255,0,0,0 });
}

GUI::~GUI()
{
	delete ptr_factory_spawning_sprites;
	delete ptr_writing_winning_timer;
	delete ptr_hunger_bar;
}

void GUI::AddComponentToDisplay(GuiElement my_element)
{
	if (my_element == gui_hunger_meter)
	{
		gui_components.push_back(ptr_hunger_bar);
	}
	else if (my_element == gui_winning_timer)
	{
		gui_components.push_back(ptr_writing_winning_timer);
	}
}

//#TODO - napisaæ funkcjê do usuwania komponentów

void GUI::ManageForCreature(Creature* ptr_creature)
{
	ManageHungerBar(ptr_creature);
}


void GUI::ManageWinningTimer(Uint32 new_value)
{
	Uint32 winning_timer_new_value_in_seconds = new_value / 1000;
	if (winning_timer_value_in_seconds != winning_timer_new_value_in_seconds)
	{
		ptr_writing_winning_timer->SetText(std::to_string(winning_timer_new_value_in_seconds));
		winning_timer_value_in_seconds = winning_timer_new_value_in_seconds;
	}
}

void GUI::ManageHungerBar(Creature* ptr_creature)
{
	int hunger_level = ptr_creature->TellHungerLevel();
	HungerBarSetChargeLevel(hunger_level);
}

//#TODO - utworzyæ osobny sprite dla hunger bar
void GUI::HungerBarSetChargeLevel(int new_level)
{
	SpriteGuiManaBar* ptr_specific_hunger_bar = static_cast<SpriteGuiManaBar*>(ptr_hunger_bar);
	ptr_specific_hunger_bar->SetTextureClipWithCharges(new_level);
}

void GUI::RenderComponents()
{
	for (VisualComponent* ptr_component : gui_components)
	{
		ptr_component->Render();
	}
}