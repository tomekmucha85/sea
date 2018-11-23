#ifndef LEVEL_COMPONENT_CORE_HPP
#define LEVEL_COMPONENT_CORE_HPP
#include <LevelComponent.hpp>

class LevelComponentCore : public LevelComponent
{
private:

public:
	LevelComponentCore(std::map<LevelComponentType, std::vector<LevelComponent*>>* my_ptr_peer_level_components);
};
#endif // LEVEL_COMPONENT_CORE_HPP