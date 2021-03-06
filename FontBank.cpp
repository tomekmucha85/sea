#include <FontBank.hpp>


TTF_Font* FontBank::ptr_font_doom = nullptr;
TTF_Font* FontBank::ptr_font_doom_big = nullptr;
TTF_Font* FontBank::ptr_font_mania_huge = nullptr;
TTF_Font* FontBank::ptr_font_nightbird_huge = nullptr;
TTF_Font* FontBank::ptr_font_morris = nullptr;
TTF_Font* FontBank::ptr_font_morris_big = nullptr;
TTF_Font** FontBank::ptr_ptr_default_font = &ptr_font_morris_big;

FontBank::FontBank()
{
	Logger::Log("Loading fonts");
	ptr_font_doom = LoadFont(path_font_doom);
	ptr_font_doom_big = LoadFont(path_font_doom, FONT_SIZE_BIG);
	ptr_font_mania_huge = LoadFont(path_font_mania, FONT_SIZE_HUGE);
	ptr_font_nightbird_huge = LoadFont(path_font_nightbird, FONT_SIZE_HUGE);
	ptr_font_morris = LoadFont(path_font_morris, FONT_SIZE_MED);
	ptr_font_morris_big = LoadFont(path_font_morris, FONT_SIZE_BIG);
	Logger::Log("Finished loading fonts");
}

TTF_Font* FontBank::LoadFont(std::string path, int size)
{
	TTF_Font* result = TTF_OpenFont(path.c_str(), size);
	if (result == NULL)
	{
		Logger::Log("Unable to load font: " + path);
		throw("Unable to load font!");
	}
	else
	{
		Logger::Log("Font " + path + " loaded.");
		return result;
	}
}

FontBank::~FontBank()
{
	;
}
