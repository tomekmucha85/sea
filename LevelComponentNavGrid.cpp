#include <LevelComponentNavGrid.hpp>

const double LevelComponentNavGrid::MAX_RADIUS_FOR_SEARCHING_CLOSEST_NODE = 600;
const double LevelComponentNavGrid::MAX_RADIUS_FOR_NODE_CONNECTION = 600;

LevelComponentNavGrid::LevelComponentNavGrid(std::map<LevelComponentType, std::vector<LevelComponent*>>* my_ptr_peer_level_components) :
	LevelComponent(my_ptr_peer_level_components)
{
	;
}

void LevelComponentNavGrid::ServeAllExternalSpawnRequests()
{
	LevelComponent::ServeAllExternalSpawnRequests();
}

void LevelComponentNavGrid::ConnectNodes(double search_radius)
{
	printf("Will connect nodes: %d nodes present.\n", TellPtrToCreaturesArray()->size());
	printf("Search radius is: %f.\n", search_radius);
	for (Creature* ptr_first_creature : *(TellPtrToCreaturesArray()))
	{
		if (ptr_first_creature->my_type == cre_navgrid_node)
		{
			dynamic_cast<CreatureNavGridNode*>(ptr_first_creature)->RemoveAllConnections();
			for (Creature* ptr_second_creature : *(TellPtrToCreaturesArray()))
			{
				if (ptr_second_creature->my_type == cre_navgrid_node && ptr_first_creature != ptr_second_creature)
				{
					if (Distance::CalculateDistanceBetweenPoints(ptr_first_creature->TellCenterPoint(), ptr_second_creature->TellCenterPoint()) <= search_radius)
					{
						//printf("Distance between nodes is: %f.\n", Distance::CalculateDistanceBetweenPoints(ptr_first_creature->TellCenterPoint(), ptr_second_creature->TellCenterPoint()));
						if (ptr_first_creature->IsThisCreatureWithinSightInCurrentEnvironment(ptr_second_creature))
						{
							dynamic_cast<CreatureNavGridNode*>(ptr_first_creature)->AddTwoSidedConnection(dynamic_cast<CreatureNavGridNode*>(ptr_second_creature));
						}
					}

				}
			}
		}
	};
}

RandomPathResponseEncapsulated LevelComponentNavGrid::GiveResponseForRandomPathRequest(RandomPathRequestEncalpsulated my_request)
{
	RandomPathResponseEncapsulated response;
	response.source_component = my_request.source_component;
	response.requestor_id = my_request.requestor_id;

	CreatureNavGridNode* ptr_initial_node = FindAGridNodeInSight(my_request.requestor_id);
	if (ptr_initial_node != nullptr)
	{
		response.navigation_path = GenerateRandomPathFromNode(ptr_initial_node, my_request.requested_hops_length);
	}
	return response;
}

PointToPointPathResponseEncapsulated LevelComponentNavGrid::GiveResponseForPointToPointPathRequest(PointToPointPathRequestEncalpsulated my_request)
{
	PointToPointPathResponseEncapsulated response;
	response.source_component = my_request.source_component;
	response.requestor_id = my_request.requestor_id;
	ConnectNodes(MAX_RADIUS_FOR_NODE_CONNECTION);
	response.navigation_path = GeneratePathToChosenPoint(my_request.my_position, my_request.destination);
	return response;
}

Creature* LevelComponentNavGrid::SelectNodeClosestToPointFromNodesSet(Coordinates point, std::vector<Creature*> nodes_set)
{
	std::map<int, Creature*> distance_vs_node = {};
	for (Creature* ptr_candidate : nodes_set)
	{
		ptr_candidate->TellCenterPoint().x,
		ptr_candidate->TellCenterPoint().y;
		int distance = static_cast<int>(Distance::CalculateDistanceBetweenPoints(ptr_candidate->TellCenterPoint(), point));
		distance_vs_node[distance] = ptr_candidate;
	}
	//std::map should be ordered by ascending int key value
	return distance_vs_node.begin()->second;
}

CreatureNavGridNode* LevelComponentNavGrid::FindAGridNodeAccessibleFromPoint(Coordinates point, double search_radius)
{
	std::vector<Creature*> accessible_nodes = {};
	CreatureNavGridNode* result = nullptr;

	std::vector<Creature*> neighboring_nodes = FindCreaturesInRadius(point, search_radius);
	if (neighboring_nodes.size() == 0)
	{
		printf("No navgrid nodes found in given range!\n");
	}
	else
	{
		for (Creature* ptr_candidate : neighboring_nodes)
		{
			printf("Found candidate x: %f y: %f.\n",
				ptr_candidate->TellCenterPoint().x,
				ptr_candidate->TellCenterPoint().y);
			if (Creature::IsThereCorridorBetweenThesePointsInCurrentEnvironment(point, ptr_candidate->TellCenterPoint(),60, MAX_RADIUS_FOR_SEARCHING_CLOSEST_NODE))
			{
				accessible_nodes.push_back(ptr_candidate);
				printf("Candidate %p suitable as declared by FindAGridNodeAccessibleFromPoint.\n", ptr_candidate);
			}
			else
			{
				printf("Candidate behind wall!\n");
			}
		}
		if (accessible_nodes.size() > 0)
		{
			result = dynamic_cast<CreatureNavGridNode*>(SelectNodeClosestToPointFromNodesSet(point, accessible_nodes));
		}
		else
		{
			printf("No accessible node found.\n");
		}
	}
	return result;
}

CreatureNavGridNode* LevelComponentNavGrid::FindAGridNodeInSight(Creature* ptr_my_creature, double search_radius)
{
	CreatureNavGridNode* result = nullptr;
	if (Creature::IsThisCreaturePresentInEnvironment(ptr_my_creature)) //Checking if pointer shows a valid creature present in current game environment
	{
		Coordinates center_point = ptr_my_creature->TellCenterPoint();
		result = FindAGridNodeAccessibleFromPoint(center_point);
	}
	return result;
}

CreatureNavGridNode* LevelComponentNavGrid::FindAGridNodeNearestToPoint(Coordinates my_point, double search_radius)
{
	CreatureNavGridNode* result = nullptr;

	std::vector<Creature*> neighboring_nodes = FindCreaturesInRadius(my_point, search_radius, cre_navgrid_node);
	if (neighboring_nodes.size() == 0)
	{
		printf("No navgrid nodes found in given range!\n");
	}
	else
	{
		result = dynamic_cast<CreatureNavGridNode*>(SelectNodeClosestToPointFromNodesSet(my_point, neighboring_nodes));
	}
	return result;
}

std::vector<Coordinates> LevelComponentNavGrid::GenerateRandomPathFromNode(CreatureNavGridNode* ptr_starting_node, unsigned int number_of_hops)
{
	// #TODO - rozbi� na mniejsze funkcje
	CreatureNavGridNode* ptr_previous_node = nullptr;
	CreatureNavGridNode* ptr_current_node = ptr_starting_node;
	std::vector<Coordinates> result = {};
	std::vector<CreatureNavGridNode*> visited_nodes = {};
	//Reconnecting nodes every time someone needs mapping
	ConnectNodes(MAX_RADIUS_FOR_NODE_CONNECTION);
	for (unsigned int i = 1; i <= number_of_hops; i++)
	{
		//printf("Generating path. Run number %d.\n", i);
		result.push_back(ptr_current_node->TellCenterPoint());
		visited_nodes.push_back(ptr_current_node);
		std::vector<CreatureNavGridNode*> my_connected_nodes = ptr_current_node->TellConnectedNodes();
		std::vector<CreatureNavGridNode*> not_visited_connected_nodes = my_connected_nodes;
		for (CreatureNavGridNode* ptr_visited_node : visited_nodes)
		{
			not_visited_connected_nodes.erase(std::remove(not_visited_connected_nodes.begin(),
				not_visited_connected_nodes.end(),
				ptr_visited_node),
				not_visited_connected_nodes.end());
		}
		unsigned int not_visited_connected_nodes_count = not_visited_connected_nodes.size();
		printf("There are %d connected nodes and %d not visited connected nodes.\n", my_connected_nodes.size(),
			not_visited_connected_nodes_count);
		if (not_visited_connected_nodes_count > 1)
		{
			unsigned int chosen_node_number = rand() % not_visited_connected_nodes_count;
			ptr_previous_node = ptr_current_node;
			ptr_current_node = not_visited_connected_nodes[chosen_node_number];
		}
		else if (not_visited_connected_nodes_count == 1)
		{
			ptr_previous_node = ptr_current_node;
			ptr_current_node = not_visited_connected_nodes[0];
		}
		else
		{
			if (my_connected_nodes.size() > 1)
			{
				std::vector<CreatureNavGridNode*>connected_nodes_without_previuos = my_connected_nodes;
				connected_nodes_without_previuos.erase(
					std::remove(connected_nodes_without_previuos.begin(),
					connected_nodes_without_previuos.end(),
					ptr_previous_node),
					connected_nodes_without_previuos.end());
				ptr_previous_node = ptr_current_node;
				if (connected_nodes_without_previuos.size() > 0)
				{
					unsigned int chosen_node_number = rand() % connected_nodes_without_previuos.size();
					ptr_current_node = connected_nodes_without_previuos[chosen_node_number];
					printf("Had to choose a visited node: x: %f, y: %f.\n", ptr_current_node->TellCenterPoint().x,
						ptr_current_node->TellCenterPoint().y);
				}
				else
				{
					printf("Connection chain broken - cut out previous node and no more nodes left!\n");
					break;
				}
			}
			else if (my_connected_nodes.size() == 1)
			{
				ptr_previous_node = ptr_current_node;
				ptr_current_node = my_connected_nodes[0];
				printf("Had to choose a visited node as only choice: x: %f, y: %f.\n", ptr_current_node->TellCenterPoint().x,
					ptr_current_node->TellCenterPoint().y);
			}
			else
			{
				printf("Connection chain broken!\n");
				break;
			}
		}
	}
	printf("Generated random path:\n");
	for (Coordinates my_coordinate : result)
	{
		printf("x: %f, y: %f\n", my_coordinate.x, my_coordinate.y);
	}
	return result;
}

std::vector<Coordinates> LevelComponentNavGrid::GeneratePathToChosenPoint(Coordinates start_point, Coordinates end_point)
{
	printf("Start: x: %f, y: %f.\n", start_point.x, start_point.y);
	printf("Destination: x: %f, y: %f.\n", end_point.x, end_point.y);
	CreatureNavGridNode* ptr_start_node = FindAGridNodeAccessibleFromPoint(start_point);
	CreatureNavGridNode* ptr_end_node = FindAGridNodeNearestToPoint(end_point);
	std::vector<Coordinates> result = {};
	//Reconnecting nodes every time someone needs mapping
	ConnectNodes(MAX_RADIUS_FOR_NODE_CONNECTION);
	if (ptr_start_node == nullptr || ptr_end_node == nullptr)
	{
		printf("Could not find suitable start/end node for path between points.\n");
		return result;
	}
	else
	{

		printf("Start node at: x: %f, y: %f.\n", ptr_start_node->TellCenterPoint().x,
			ptr_start_node->TellCenterPoint().y);
		printf("End node at: x: %f, y: %f.\n", ptr_end_node->TellCenterPoint().x,
			ptr_end_node->TellCenterPoint().y);
		result = GeneratePathBetweenNodes(ptr_start_node, ptr_end_node);
		result.push_back(end_point);
		return result;
	}
}

std::vector<Coordinates> LevelComponentNavGrid::GeneratePathBetweenNodes(CreatureNavGridNode* ptr_my_start_node, CreatureNavGridNode* ptr_my_end_node)
{
	std::vector<Coordinates> result = {};
	std::map<CreatureNavGridNode*, CreatureNavGridNode*> reached_node_vs_where_did_I_come_from = {};
	std::vector<CreatureNavGridNode*>  frontier = {};

	frontier.push_back(ptr_my_start_node);

	//Construct mapping

	printf("Will construct mapping.\n");
	while (frontier.size() > 0)
	{
		std::vector<CreatureNavGridNode*>  future_frontier = {};
		for (CreatureNavGridNode* ptr_frontier_node : frontier)
		{
			for (CreatureNavGridNode* ptr_connected_node : ptr_frontier_node->TellConnectedNodes())
			{
				if (reached_node_vs_where_did_I_come_from.find(ptr_connected_node) == reached_node_vs_where_did_I_come_from.end())
					//If key was not found in map
				{
					reached_node_vs_where_did_I_come_from[ptr_connected_node] = ptr_frontier_node;
					future_frontier.push_back(ptr_connected_node);
				}
			}
		}
		frontier = future_frontier;
	}
	Logger::Log("Mapping size: " + std::to_string(reached_node_vs_where_did_I_come_from.size()), 
		debug_full);
	//Find way back
	Logger::Log("Will find way back to starting node.", debug_full);
	result.insert(result.begin(), ptr_my_end_node->TellCenterPoint());
	CreatureNavGridNode* ptr_currently_evaluated_node = ptr_my_end_node;
	while (ptr_currently_evaluated_node != ptr_my_start_node)
	{
		/*printf("Inserting: x: %f y: %f.\n", 
			reached_node_vs_where_did_I_come_from[ptr_currently_evaluated_node]->TellCenterPoint().x,
			reached_node_vs_where_did_I_come_from[ptr_currently_evaluated_node]->TellCenterPoint().y);*/
		result.insert(result.begin(), reached_node_vs_where_did_I_come_from[ptr_currently_evaluated_node]->TellCenterPoint());
		ptr_currently_evaluated_node = reached_node_vs_where_did_I_come_from[ptr_currently_evaluated_node];
	}
	result.insert(result.begin(), ptr_my_start_node->TellCenterPoint());
	Logger::Log("Following way was proposed from: x: " +
		std::to_string(ptr_my_start_node->TellCenterPoint().x) +
		" y: " +
		std::to_string(ptr_my_start_node->TellCenterPoint().y) +
		" to x: " +
		std::to_string(ptr_my_end_node->TellCenterPoint().x) +
		" to y: " +
		std::to_string(ptr_my_end_node->TellCenterPoint().y) +
		" :", debug_full);

	for (Coordinates step : result)
	{
		Logger::Log("x: " + std::to_string(step.x) +" y: " + std::to_string(step.y),
			debug_full);
	}
	return result;
}