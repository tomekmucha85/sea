#ifndef CREATURE_WRITING_HPP
#define CREATURE_WRITING_HPP
#include <Creature.hpp>
class CreatureWriting : public Creature
{
private:
	std::string text = "";
	SDL_Color color = {0,0,0,0};
	TTF_Font* ptr_font = nullptr;
public:
	CreatureWriting(Coordinates* ptr_my_upper_left_corner, 
		std::string my_text, 
		SDL_Color my_color = { 255,0,0,255 },
		TTF_Font* ptr_my_font = *(FontBank::ptr_ptr_default_font));
	//bool Move(double x, double y);
	std::string TellText();
	void SetNewTextColor(SDL_Color my_color);
	void SetNewText(std::string my_text);
	SDL_Color TellColor();
};
#endif // CREATURE_WRITING_HPP