#ifndef LEVEL_COMPONENT_NAVGRID_HPP
#define LEVEL_COMPONENT_NAVGRID_HPP
#include <LevelComponent.hpp>
class LevelComponentNavGrid : public LevelComponent
{
private:
	const double MAX_RADIUS_FOR_NODE_CONNECTION = 400;
	const double MAX_RADIUS_FOR_SEARCHING_CLOSEST_NODE = 500;
public:
	LevelComponentNavGrid(std::map<LevelComponentType, std::vector<LevelComponent*>>* my_ptr_peer_level_components);
	void ConnectNodes(double search_radius);
	void ServeAllExternalSpawnRequests();
	RandomPathResponseEncapsulated GiveResponseForRandomPathRequest(RandomPathRequestEncalpsulated my_request);
	CreatureNavGridNode* FindAGridNodeInSight(Creature* ptr_my_creature);
	CreatureNavGridNode* FindAGridNodeAccessibleFromPoint(Coordinates point);
	std::vector<Coordinates> GenerateRandomPathFromNode(CreatureNavGridNode* ptr_starting_node, unsigned int number_of_hops);
	std::vector<Coordinates> GeneratePathToChosenPoint(Creature* ptr_wanderer, Coordinates end_point);
	std::vector<Coordinates> GeneratePathBetweenNodes(CreatureNavGridNode* ptr_my_start_node, CreatureNavGridNode* ptr_my_end_node);
};

#endif // LEVEL_COMPONENT_NAVGRID_HPP

