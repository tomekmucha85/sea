#include <LevelComponentTriggers.hpp>

LevelComponentTriggers::LevelComponentTriggers(std::map<LevelComponentType, std::vector<LevelComponent*>>* my_ptr_peer_level_components) :
	LevelComponent(my_ptr_peer_level_components)
{
	;
}