#ifndef LEVEL_COMPONENT_TRIGGERS
#define LEVEL_COMPONENT_TRIGGERS
#include <LevelComponent.hpp>
class LevelComponentTriggers : public LevelComponent
{
private:

public:
	LevelComponentTriggers(std::map<LevelComponentType, std::vector<LevelComponent*>>* my_ptr_peer_level_components, 
		PreciseRect my_component_area = { 0,0,0,0 });
	std::vector<std::string> RunTriggersHitByHero();
};
#endif // LEVEL_COMPONENT_TRIGGERS
