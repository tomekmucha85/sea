#ifndef LEVEL_COMPONENT_NAVGRID_HPP
#define LEVEL_COMPONENT_NAVGRID_HPP
#include <LevelComponent.hpp>
class LevelComponentNavGrid : public LevelComponent
{
private:
	const double MAX_RADIUS_FOR_NODE_CONNECTION = 400;
public:
	LevelComponentNavGrid(std::map<LevelComponentType, std::vector<LevelComponent*>>* my_ptr_peer_level_components);
	void ConnectNodes(double search_radius);
	void ServeAllExternalSpawnRequests();
};
#endif // LEVEL_COMPONENT_NAVGRID_HPP

