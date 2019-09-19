#ifndef CREATURE_NAVGRID_NODE_HPP
#define CREATURE_NAVGRID_NODE_HPP
#include <vector>
#include <algorithm>
#include <Creature.hpp>

class CreatureNavGridNode : public Creature
{
private:
	std::vector<CreatureNavGridNode*> connected_nodes = {};
public:
	CreatureNavGridNode(Coordinates* ptr_my_coordinates);
	bool CheckIfConnectionAlrreadyExists(CreatureNavGridNode* ptr_neighbor_node);
	void AddOneSidedConnection(CreatureNavGridNode* ptr_neighbor_node);
	void AddTwoSidedConnection(CreatureNavGridNode* ptr_neighbor_node);
	void RemoveOneSidedConnection(CreatureNavGridNode* ptr_neighbor_node);
	void RemoveTwoSidedConnection(CreatureNavGridNode* ptr_neighbor_node);
	void CreateLineTowardsNode(Creature* ptr_neighbor_node);
	void RemoveLineTowardsNode(Creature* ptr_neighbor_node);
	void RemoveAllConnections();
	std::vector<CreatureNavGridNode*> TellConnectedNodes();
};
#endif // CREATURE_NAVGRID_NODE_HPP