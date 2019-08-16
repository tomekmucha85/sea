#include <TrueTypeWriting.hpp>

TrueTypeWriting::TrueTypeWriting(std::string text,
	Coordinates* ptr_upper_left_corner,
	TTF_Font* ptr_my_font,
	SDL_Color color) : VisualComponent()
{
	printf("Will spawn a writing visual component.\n");
	//Spawning a texture
	SetColor(color);
	//Default font
	if (ptr_my_font == nullptr)
	{
		SetFont(FontBank::ptr_font_doom);
	}
	else
	{
		SetFont(ptr_my_font);
	}
	ptr_writing_texture = Texture::LoadTextureFromRenderedText(text, Screen::renderer, ptr_font, color);
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

void TrueTypeWriting::SetColor(SDL_Color my_color)
{
	color = my_color;
}

void TrueTypeWriting::SetFont(TTF_Font* ptr_my_font)
{
	ptr_font = ptr_my_font;
}

void TrueTypeWriting::SetText(std::string my_text)
{
	SDL_DestroyTexture(ptr_writing_texture);
	ptr_writing_texture = Texture::LoadTextureFromRenderedText(my_text, Screen::renderer, ptr_font, color);
	if (ptr_writing_texture != nullptr)
	{
		texture_clip = CheckTextureDimensions(ptr_writing_texture);
		//Altering dimensions, 'case texture clip may have changed.
		position.w = texture_clip.w;
		position.h = texture_clip.h;
	}
	else
	{
		printf("Unable to create writing texture!\n");
		throw("Unable to create writing texture!\n");
	}
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