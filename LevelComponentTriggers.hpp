#ifndef LEVEL_COMPONENT_TRIGGERS
#define LEVEL_COMPONENT_TRIGGERS
#include <LevelComponent.hpp>
class LevelComponentTriggers : public LevelComponent
{
private:

public:
	LevelComponentTriggers(std::map<LevelComponentType, std::vector<LevelComponent*>>* my_ptr_peer_level_components);
};
#endif // LEVEL_COMPONENT_TRIGGERS
