#ifndef LEVEL_COMPONENT_NAVGRID_HPP
#define LEVEL_COMPONENT_NAVGRID_HPP
#include <LevelComponent.hpp>
class LevelComponentNavGrid : public LevelComponent
{
private:
	static const double MAX_RADIUS_FOR_NODE_CONNECTION;
	static const double MAX_RADIUS_FOR_SEARCHING_CLOSEST_NODE;
public:
	LevelComponentNavGrid(std::map<LevelComponentType, std::vector<LevelComponent*>>* my_ptr_peer_level_components);
	void ConnectNodes(double search_radius);
	void ServeAllExternalSpawnRequests();
	RandomPathResponseEncapsulated GiveResponseForRandomPathRequest(RandomPathRequestEncalpsulated my_request);
	PointToPointPathResponseEncapsulated GiveResponseForPointToPointPathRequest(PointToPointPathRequestEncalpsulated my_request);
	CreatureNavGridNode* FindAGridNodeInSight(Creature* ptr_my_creature, double search_radius = MAX_RADIUS_FOR_SEARCHING_CLOSEST_NODE);
	Creature* SelectNodeClosestToPointFromNodesSet(Coordinates point, std::vector<Creature*> nodes_set);
	CreatureNavGridNode* FindAGridNodeNearestToPoint(Coordinates my_point, double search_radius = MAX_RADIUS_FOR_SEARCHING_CLOSEST_NODE);
	CreatureNavGridNode* FindAGridNodeAccessibleFromPoint(Coordinates point, double search_radius = MAX_RADIUS_FOR_SEARCHING_CLOSEST_NODE);
	std::vector<Coordinates> GenerateRandomPathFromNode(CreatureNavGridNode* ptr_starting_node, unsigned int number_of_hops);
	std::vector<Coordinates> GeneratePathToChosenPoint(Coordinates start_point, Coordinates end_point);
	std::vector<Coordinates> GeneratePathBetweenNodes(CreatureNavGridNode* ptr_my_start_node, CreatureNavGridNode* ptr_my_end_node);
};

#endif // LEVEL_COMPONENT_NAVGRID_HPP

