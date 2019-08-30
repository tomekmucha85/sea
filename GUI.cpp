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
	//Onscreen printer
	ptr_onscreen_printer = new TrueTypeWriting(
		" ",
		&onscreen_printer_upper_left_corner,
		nullptr,
		onscreen_printer_default_color);
	//Cyclic tasks
	cyclic_actions.push_back(func_manage_onscreen_printer);
}

GUI::~GUI()
{
	delete ptr_factory_spawning_sprites;
	delete ptr_writing_winning_timer;
	delete ptr_hunger_bar;
	delete ptr_onscreen_printer;
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
	else if (my_element == gui_printer)
	{
		gui_components.push_back(ptr_onscreen_printer);
	}
}

//#TODO - napisaæ funkcjê do usuwania komponentów

void GUI::ManageForCreature(Creature* ptr_creature)
{
	ManageHungerBar(ptr_creature);
}

void GUI::PerformCyclicActions()
{
	for (std::function<void(GUI*)> action : cyclic_actions)
	{
		action(this);
	}
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

void GUI::PrintTextOnscreen(std::string my_text, Uint32 time_to_live_in_miliseconds)
{
	ptr_onscreen_printer->SetText(my_text);
	if (time_to_live_in_miliseconds == 0)
	{
		time_to_live_in_miliseconds = onscreen_printer_text_default_time_to_live_in_miliseconds;
	}
	ptr_timer_for_onscreen_printer = new TimerCountdown(time_to_live_in_miliseconds);
}

bool GUI::CheckIfOnscreenPrinterTextExpired()
{
	if (ptr_timer_for_onscreen_printer != nullptr)
	{
		return ptr_timer_for_onscreen_printer->CheckIfCountdownFinished();
	}
	return false;
}

//#TODO - mo¿e wiêcej akcji powinno trafiæ do cyclic actions?
void GUI::ManageOnscreenPrinter()
{
	if (CheckIfOnscreenPrinterTextExpired() == true)
	{
		//Clear text displayed onscreen if time to live expired.
		delete ptr_timer_for_onscreen_printer;
		ptr_timer_for_onscreen_printer = nullptr;
		ptr_onscreen_printer->SetText(" ");
	}
}

void GUI::RenderComponents()
{
	for (VisualComponent* ptr_component : gui_components)
	{
		//printf("Rendering GUI component.\n");
		ptr_component->Render();
	}
}