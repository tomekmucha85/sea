#ifndef FONTBANK_HPP
#define FONTBANK_HPP
#include <stdio.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <texture.hpp>
#include <Logger.hpp>
#include <FontBank.hpp>

class FontBank
{
private:
	//###################
	//Variables
	//###################
	std::string path_font_doom = "images/doom.ttf";
	std::string path_font_mania = "images/HoganMania.ttf";
	static const int FONT_SIZE_MED = 28;
	static const int FONT_SIZE_BIG = 48;
	static const int FONT_SIZE_HUGE = 100;

public:
	//###################
	//Variables
	//###################
	static TTF_Font* ptr_font_doom;
	static TTF_Font* ptr_font_doom_big;
	static TTF_Font* ptr_font_mania_huge;
	static TTF_Font** ptr_ptr_default_font;

	//###################
	//Functions
	//###################
	FontBank();
	TTF_Font* LoadFont(std::string path, int size = FONT_SIZE_MED);
	~FontBank();

};

#endif //FONTBANK_HPP