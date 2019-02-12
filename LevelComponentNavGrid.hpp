#ifndef LEVEL_COMPONENT_NAVGRID_HPP
#define LEVEL_COMPONENT_NAVGRID_HPP
#include <LevelComponent.hpp>
class LevelComponentNavGrid : public LevelComponent
{
private:
	
public:
	LevelComponentNavGrid(std::map<LevelComponentType, std::vector<LevelComponent*>>* my_ptr_peer_level_components);
};
#endif // LEVEL_COMPONENT_NAVGRID_HPP

