#include <TrueTypeWriting.hpp>

TrueTypeWriting::TrueTypeWriting(std::string text,
	Coordinates* ptr_upper_left_corner,
	TTF_Font* ptr_font,
	SDL_Color color) : VisualComponent()
{
	printf("Will spawn a writing visual component.\n");
	//Spawning a texture
	SetColor(color);
	ptr_writing_texture = Texture::LoadTextureFromRenderedText(text, Screen::renderer, FontBank::ptr_font_doom, color);
	//Positioning the texture on screen.
	//#TODO - sprawdziæ, czy w innych metodach query nie jest wykonywane za czêsto
	if (ptr_writing_texture != nullptr)
	{
		texture_clip = CheckTextureDimensions(ptr_writing_texture);
		SetPosition({ ptr_upper_left_corner->x,
			ptr_upper_left_corner->y,
			static_cast<double>(texture_clip.w),
			static_cast<double>(texture_clip.h)});
		SetCenter(CalculateCenterOfGivenPosition(position));
	}
	else
	{
		printf("Unable to create writing texture!\n");
		throw("Unable to create writing texture!\n");
	}

	Logger::Log("Spawned a true type writing!: " + text);
}

TrueTypeWriting::~TrueTypeWriting()
{
	SDL_DestroyTexture(ptr_writing_texture);
}

void TrueTypeWriting::Render()
{
	//printf("Will render a true type writing.\n");
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
	position = CalculatePositionAroundCenter(center, texture_clip.w, texture_clip.h);
}

void TrueTypeWriting::SetPosition(PreciseRect my_position)
{
	position = my_position;
	center = CalculateCenterOfGivenPosition(my_position);
}

PreciseRect TrueTypeWriting::TellPosition()
{
	return position;
}

Coordinates TrueTypeWriting::TellUpperLeftCorner()
{
	return { position.x, position.y };
}

void TrueTypeWriting::Move(double step_x, double step_y)
{
	center.x += step_x;
	center.y += step_y;
	position = CalculatePositionAroundCenter(center, texture_clip.w, texture_clip.h);
}