#ifndef LEVEL_COMPONENT_CORE_HPP
#define LEVEL_COMPONENT_CORE_HPP
#include <LevelComponent.hpp>

class LevelComponentCore : public LevelComponent
{
private:

public:
	LevelComponentCore(std::map<LevelComponentType, std::vector<LevelComponent*>>* my_ptr_peer_level_components, 
		SDL_Rect my_component_area = { 0,0,0,0 });
};
#endif // LEVEL_COMPONENT_CORE_HPP