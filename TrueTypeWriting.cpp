#include <TrueTypeWriting.hpp>

TrueTypeWriting::TrueTypeWriting(std::string text,
	Coordinates* ptr_upper_left_corner,
	TTF_Font* ptr_font,
	Color color) : VisualComponent()
{
	//Spawning a texture
	ptr_writing_texture = Texture::LoadTextureFromRenderedText(text, Screen::renderer, FontBank::ptr_font_doom);
	//Positioning the texture on screen.
	//#TODO - sprawdziæ, czy w innych metodach query nie jest wykonywane za czêsto
	if (ptr_writing_texture != nullptr)
	{
		SDL_Rect texture_outline = CheckTextureDimensions(ptr_writing_texture);
		SetPosition({ ptr_upper_left_corner->x,
			ptr_upper_left_corner->y,
			static_cast<double>(texture_outline.w),
			static_cast<double>(texture_outline.h)});
		SetCenter(CalculateCenterOfGivenPosition(position));
	}
	else
	{
		printf("Unable to create writing texture!\n");
		throw("Unable to create writing texture!\n");
	}

	Logger::Log("Spawned a true type writing!: " + text);
}

void TrueTypeWriting::SetColor(Color my_color)
{
	;
}

void TrueTypeWriting::Render()
{
	SDL_Rect position_int = ConvertPreciseRectToSdlRect(position);
	SDL_RenderCopyEx(Screen::renderer, ptr_writing_texture, 
		nullptr, 
		&position_int,
		0, 
		0, 
		SDL_FLIP_NONE);
}

Coordinates TrueTypeWriting::TellCenter()
{
	return center;
}

void TrueTypeWriting::SetCenter(Coordinates my_center)
{
	center = my_center;
}

void TrueTypeWriting::SetPosition(PreciseRect my_position)
{
	position = my_position;
}

PreciseRect TrueTypeWriting::TellPosition()
{
	return position;
}

void TrueTypeWriting::Move(double step_x, double step_y)
{
	;
}