#include <CreatureWriting.hpp>

CreatureWriting::CreatureWriting(Coordinates* ptr_my_upper_left_corner, 
	std::string my_text, 
	SDL_Color my_color,
	int hitbox_margin) : Creature(my_text, ptr_my_upper_left_corner, my_color)
{
	Logger::Log("Will spawn a WritingCreature.");
	my_type = cre_writing;
	text = my_text;
	color = my_color;
}

std::string CreatureWriting::TellText()
{
	return text;
}

void CreatureWriting::SetNewTextColor(SDL_Color my_color)
{

	color = my_color;
	//#TODO - nie do koñca bezpieczne za³o¿enie, ¿e mo¿na zrobiæ cast
	TrueTypeWriting* ptr_current_writing_visual_part = static_cast<TrueTypeWriting*>(TellMainVisualComponent());
	TrueTypeWriting* ptr_new_writing_visual_part = new TrueTypeWriting(
		text,
		&(ptr_current_writing_visual_part->TellUpperLeftCorner()),
		*FontBank::ptr_ptr_default_font,
		my_color);

	ResetMainVisualComponent(ptr_new_writing_visual_part);
}

void CreatureWriting::SetNewText(std::string my_text)
{
	;
}


SDL_Color CreatureWriting::TellColor()
{
	return color;
}