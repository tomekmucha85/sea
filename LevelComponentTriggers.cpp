#include <LevelComponentTriggers.hpp>

LevelComponentTriggers::LevelComponentTriggers(std::map<LevelComponentType, std::vector<LevelComponent*>>* my_ptr_peer_level_components,
	SDL_Rect my_component_area) :
	LevelComponent(my_ptr_peer_level_components, my_component_area)
{
	;
}