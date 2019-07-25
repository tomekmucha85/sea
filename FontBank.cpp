#include <FontBank.hpp>


TTF_Font* FontBank::ptr_font_doom = nullptr;
TTF_Font** FontBank::ptr_ptr_default_font = &ptr_font_doom;

FontBank::FontBank()
{
	Logger::Log("Loading fonts");
	ptr_font_doom = LoadFont(path_font_doom);
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
