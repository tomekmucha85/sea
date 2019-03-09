#include <CreatureNavGridNode.hpp>

CreatureNavGridNode::CreatureNavGridNode(Coordinates* ptr_my_coordinates) :
	Creature(ptr_my_coordinates)
{
	//printf("Spawned a navigation grid node.\n");
	my_type = cre_navgrid_node;
	//printf("Assigned type.\n");
	MakeMeNotObstacle();
	//printf("No obstacle set.\n");
	//By default this creature should not be rendered on screen.
	SetVisibility(true);
	visual_components[0]->SetColor({ 255,0,0,255 });
}

bool CreatureNavGridNode::CheckIfConnectionAlrreadyExists(CreatureNavGridNode* ptr_neighbor_node)
{
	if (find(connected_nodes.begin(), connected_nodes.end(), ptr_neighbor_node) != connected_nodes.end())
	{
		//printf("Connection exists!\n");
		return true;
	}
	else
	{
		return false;
	}
}

void CreatureNavGridNode::CreateLineTowardsNode(Creature* ptr_neighbor_node)
{
	Coordinates line_start = TellCenterPoint();
	Coordinates line_end = ptr_neighbor_node->TellCenterPoint();
	VisualComponent* ptr_line_connector = new Line(&line_start, &line_end);
	ptr_line_connector->SetColor({0,0,255,255});
	visual_components.push_back(ptr_line_connector);
}

void CreatureNavGridNode::RemoveLineTowardsNode(Creature* ptr_neighbor_node)
{
	/*printf("Will attempt to remove lines towards node.\n");
	Coordinates line_end = ptr_neighbor_node->TellCenterPoint();
	std::vector<VisualComponent*> lines_to_remove = {};
	for (VisualComponent* ptr_visual_component : visual_components)
	{
		if (ptr_visual_component->end.x == line_end.x && ptr_visual_component->end.y == line_end.y)
		{
			lines_to_remove.push_back(ptr_visual_component);
		}
	}
	for (VisualComponent* ptr_line_to_remove : lines_to_remove)
	{
		visual_components.erase(std::remove(visual_components.begin(), visual_components.end(), ptr_line_to_remove), visual_components.end());
		delete ptr_line_to_remove;
		printf("Removed a line towards node.\n");
	}*/
	throw "Not implemented yet - removing lines.\n";
}

void CreatureNavGridNode::AddOneSidedConnection(CreatureNavGridNode* ptr_neighbor_node)
{
	//Insert key if not existing yet.
	//printf("Will attempt to add one-sided node connection.\n");
	//Connection should not exist yet and not point to myself
	if (CheckIfConnectionAlrreadyExists(ptr_neighbor_node) == false && ptr_neighbor_node != this)
	{
		connected_nodes.push_back(ptr_neighbor_node);
		CreateLineTowardsNode(ptr_neighbor_node);
		//printf("Added connection between %p and %p.\n", this, ptr_neighbor_node);
	}
	else
	{
		//printf("Rejected connection between %p and %p.\n", this, ptr_neighbor_node);
	}
}

void CreatureNavGridNode::AddTwoSidedConnection(CreatureNavGridNode* ptr_neighbor_node)
{
	AddOneSidedConnection(ptr_neighbor_node);
	ptr_neighbor_node->AddOneSidedConnection(this);
}

void CreatureNavGridNode::RemoveOneSidedConnection(CreatureNavGridNode* ptr_neighbor_node)
{
	//RemoveLineTowardsNode(ptr_neighbor_node);
	connected_nodes.erase(std::remove(connected_nodes.begin(), connected_nodes.end(), ptr_neighbor_node), connected_nodes.end());
}

void CreatureNavGridNode::RemoveTwoSidedConnection(CreatureNavGridNode* ptr_neighbor_node)
{
	RemoveOneSidedConnection(ptr_neighbor_node);
	ptr_neighbor_node->RemoveOneSidedConnection(this);
}

std::vector<CreatureNavGridNode*> CreatureNavGridNode::TellConnectedNodes()
{
	return connected_nodes;
}