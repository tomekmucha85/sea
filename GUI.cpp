#include <GUI.hpp>

const std::string GUI::EMPTY_TEXT_STRING = " ";

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
	//Onscreen printer - initialize 2 rows
	ptr_onscreen_printer_1st_row = new TrueTypeWriting(
		" ",
		&onscreen_printer_1st_row_upper_left_corner,
		nullptr,
		onscreen_printer_default_color);
	ptr_onscreen_printer_2nd_row = new TrueTypeWriting(
		" ",
		&onscreen_printer_2nd_row_upper_left_corner,
		nullptr,
		onscreen_printer_default_color);
	ptr_timer_for_onscreen_printer_1st_row = new TimerCountdown(0);
	ptr_timer_for_onscreen_printer_2nd_row = new TimerCountdown(0);
	ptr_timer_for_onscreen_printer_3rd_row = new TimerCountdown(0);
	//Onscreen printer - instant messages in the middle of the screen
	ptr_centered_onscreen_printer = new TrueTypeWriting(
		" ",
		&centered_onscreen_printer_upper_left_corner,
		FontBank::ptr_font_mania_huge,
		onscreen_centered_printer_default_color);
	ptr_timer_for_centered_onscreen_printer = new TimerCountdown(0);
	//Cyclic tasks
	cyclic_actions.push_back(func_manage_onscreen_printer);
	cyclic_actions.push_back(func_manage_centered_onscreen_printer);
}

GUI::~GUI()
{
	delete ptr_factory_spawning_sprites;
	delete ptr_writing_winning_timer;
	delete ptr_hunger_bar;
	delete ptr_onscreen_printer_1st_row;
	delete ptr_onscreen_printer_2nd_row;
	delete ptr_onscreen_printer_3rd_row;
	delete ptr_timer_for_onscreen_printer_1st_row;
	delete ptr_timer_for_onscreen_printer_2nd_row;
	delete ptr_timer_for_onscreen_printer_3rd_row;
	delete ptr_centered_onscreen_printer;
	delete ptr_timer_for_centered_onscreen_printer;
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
		gui_components.push_back(ptr_onscreen_printer_1st_row);
		gui_components.push_back(ptr_onscreen_printer_2nd_row);
		//gui_components.push_back(ptr_onscreen_printer_3rd_row);
	}
	else if (my_element == gui_centered_printer)
	{
		gui_components.push_back(ptr_centered_onscreen_printer);
	}
}

void GUI::RemoveComponentFromDisplay(GuiElement my_element)
{
	if (my_element == gui_hunger_meter)
	{
		gui_components.erase(std::remove(gui_components.begin(), gui_components.end(), ptr_hunger_bar), gui_components.end());
	}
	else if (my_element == gui_winning_timer)
	{
		gui_components.erase(std::remove(gui_components.begin(), gui_components.end(), ptr_writing_winning_timer), gui_components.end());
	}
	else if (my_element == gui_printer)
	{
		gui_components.erase(std::remove(gui_components.begin(), gui_components.end(), ptr_onscreen_printer_1st_row), gui_components.end());
		gui_components.erase(std::remove(gui_components.begin(), gui_components.end(), ptr_onscreen_printer_2nd_row), gui_components.end());
		//gui_components.erase(std::remove(gui_components.begin(), gui_components.end(), ptr_onscreen_printer_3rd_row), gui_components.end());
	}
}

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

//#TODO - utworzy� osobny sprite dla hunger bar
void GUI::HungerBarSetChargeLevel(int new_level)
{
	SpriteGuiManaBar* ptr_specific_hunger_bar = static_cast<SpriteGuiManaBar*>(ptr_hunger_bar);
	ptr_specific_hunger_bar->SetTextureClipWithCharges(new_level);
}

void GUI::PrintBigMessaeOnScreenCenter(std::string my_text, Uint32 time_to_live)
{
	Logger::Log("Will print big message: " + my_text, debug_info);
	ptr_centered_onscreen_printer->SetText(my_text);
	if (time_to_live == 0)
	{
		time_to_live = centered_oncreen_printer_text_default_time_to_live_in_miliseconds;
	}
	printf("Message will last %d miliseconds.\n", time_to_live);
	ptr_timer_for_centered_onscreen_printer->ResetWithNewTimeToLive(time_to_live);
}

void GUI::PrintTextOnscreen(std::string my_text, Uint32 time_to_live_in_miliseconds)
{
	if (CheckIfOnscreenPrinter1stRowIsAvailable())
	{
		printf("1st row was available.\n");
		Populate1stPrinterRow(my_text, time_to_live_in_miliseconds);
	}
	else if (CheckIfOnscreenPrinter2ndRowIsAvailable())
	{
		printf("2nd row was available.\n");
		PushFirstPrinterRowUpToSecond();
		Populate1stPrinterRow(my_text, time_to_live_in_miliseconds);
	}
	else
	//In case both 1st and second row are occupied
	{
		printf("No rows were available.\n");
		PushFirstPrinterRowUpToSecond();
		Populate1stPrinterRow(my_text, time_to_live_in_miliseconds);
	}
}

void GUI::Populate1stPrinterRow(std::string my_text, Uint32 time_to_live_in_miliseconds)
{
	ptr_onscreen_printer_1st_row->SetText(my_text);
	if (time_to_live_in_miliseconds == 0)
	{
		time_to_live_in_miliseconds = onscreen_printer_text_default_time_to_live_in_miliseconds;
	}
	ptr_timer_for_onscreen_printer_1st_row->ResetWithNewTimeToLive(time_to_live_in_miliseconds);
}

void GUI::PushFirstPrinterRowUpToSecond()
{
	//Populate 2nd row
	ptr_onscreen_printer_2nd_row->SetText(ptr_onscreen_printer_1st_row->TellText()); // Copy text from 1st row
	printf("Text should be transfered to second row: ");
	Logger::Log(ptr_onscreen_printer_1st_row->TellText());
	//Timer for the second row "inherits" value from timer for the first row
	//If inherited value is smaller than minimal value, it is normalized to min. value.
	const Uint32 MINIMAL_TIME_TO_LIVE = 800;
	Uint32 time_left_for_inherited_entry = ptr_timer_for_onscreen_printer_1st_row->HowManyMilisecondsLeftTillEnd();
	if (time_left_for_inherited_entry < MINIMAL_TIME_TO_LIVE)
	{
		time_left_for_inherited_entry = MINIMAL_TIME_TO_LIVE;
	}
	ptr_timer_for_onscreen_printer_2nd_row->ResetWithNewTimeToLive(time_left_for_inherited_entry);
	printf("Time to live of %d was set for the second row.\n", time_left_for_inherited_entry);
}

bool GUI::CheckIfCenteredOnscreenPrinterIsAvailable()
{
	return ptr_timer_for_centered_onscreen_printer->CheckIfCountdownFinished();
}

bool GUI::CheckIfOnscreenPrinter1stRowIsAvailable()
{
	return ptr_timer_for_onscreen_printer_1st_row->CheckIfCountdownFinished();
}

bool GUI::CheckIfOnscreenPrinter2ndRowIsAvailable()
{
	return ptr_timer_for_onscreen_printer_2nd_row->CheckIfCountdownFinished();
}

bool GUI::CheckIfOnscreenPrinter3rdRowIsAvailable()
{
	return ptr_timer_for_onscreen_printer_3rd_row->CheckIfCountdownFinished();
}

//#TODO - mo�e wi�cej akcji powinno trafi� do cyclic actions?
void GUI::ManageOnscreenPrinter()
// Wiping out printed text if it's outdated
{
	if (CheckIfOnscreenPrinter1stRowIsAvailable() == true)
	{
		if (ptr_onscreen_printer_1st_row->TellText() != EMPTY_TEXT_STRING)
		{
			ptr_onscreen_printer_1st_row->SetText(EMPTY_TEXT_STRING);
		}
	}
	if (CheckIfOnscreenPrinter2ndRowIsAvailable() == true)
	{
		if (ptr_onscreen_printer_2nd_row->TellText() != EMPTY_TEXT_STRING)
		{
			ptr_onscreen_printer_2nd_row->SetText(EMPTY_TEXT_STRING);
		}
	}
}

void GUI::ManageCenteredOnscreenPrinter()
{
	// Wiping out printed text if it's outdated
	{
		if (CheckIfCenteredOnscreenPrinterIsAvailable() == true)
		{
			if (ptr_centered_onscreen_printer->TellText() != EMPTY_TEXT_STRING)
			{
				ptr_centered_onscreen_printer->SetText(EMPTY_TEXT_STRING);
			}
		}
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