#include <LevelComponentNavGrid.hpp>

LevelComponentNavGrid::LevelComponentNavGrid(std::map<LevelComponentType, std::vector<LevelComponent*>>* my_ptr_peer_level_components) :
	LevelComponent(my_ptr_peer_level_components)
{
	;
}

void LevelComponentNavGrid::ServeAllExternalSpawnRequests()
{
	unsigned int creatures_count_before = TellPtrToCreaturesArray()->size();
	LevelComponent::ServeAllExternalSpawnRequests();
	unsigned int creatures_count_after = TellPtrToCreaturesArray()->size();
	if (creatures_count_before != creatures_count_after)
	{
		ConnectNodes(MAX_RADIUS_FOR_NODE_CONNECTION);
	}
}

void LevelComponentNavGrid::ConnectNodes(double search_radius)
{
	printf("Will connect nodes: %d nodes present.\n", TellPtrToCreaturesArray()->size());
	printf("Search radius is: %f.\n", search_radius);
	for (Creature* ptr_first_creature : *(TellPtrToCreaturesArray()))
	{
		if (ptr_first_creature->my_type == cre_navgrid_node)
		{
			for (Creature* ptr_second_creature : *(TellPtrToCreaturesArray()))
			{
				if (ptr_second_creature->my_type == cre_navgrid_node && ptr_first_creature != ptr_second_creature)
				{
					if (Distance::CalculateDistanceBetweenPoints(ptr_first_creature->TellCenterPoint(), ptr_second_creature->TellCenterPoint()) <= search_radius)
					{
						//printf("Distance between nodes is: %f.\n", Distance::CalculateDistanceBetweenPoints(ptr_first_creature->TellCenterPoint(), ptr_second_creature->TellCenterPoint()));
						if (ptr_first_creature->IsThisCreatureWithinSight(ptr_second_creature))
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

CreatureNavGridNode* LevelComponentNavGrid::FindAGridNodeAccessibleFromPoint(Coordinates point)
{
	//#TODO - duplikacja kodu z FindAGridNodeInSight. Do ogarniêcia.
	const double seek_range = MAX_RADIUS_FOR_SEARCHING_CLOSEST_NODE;
	CreatureNavGridNode* result = nullptr;

	std::vector<Creature*> neighboring_nodes = FindCreaturesInRadius(point, seek_range);
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

			if (Creature::IsThereLineOfSightBetweenThesePoints(point, ptr_candidate->TellCenterPoint()))
			{
				result = dynamic_cast<CreatureNavGridNode*>(ptr_candidate);
				printf("Candidate %p suitable.\n", result);
				return result;
			}
			printf("Candidate behind wall!\n");
		}
	}

	printf("No node found in given range!.\n");
	return result;
}

CreatureNavGridNode* LevelComponentNavGrid::FindAGridNodeInSight(Creature* ptr_my_creature)
{
	const double seek_range = MAX_RADIUS_FOR_SEARCHING_CLOSEST_NODE;
	CreatureNavGridNode* result = nullptr;
	if (Creature::IsThisCreaturePresentInEnvironment(ptr_my_creature)) //Checking if pointer shows a valid creature present in current game environment
	{
		std::vector<Creature*> neighboring_nodes = FindCreaturesInRadius(ptr_my_creature->TellCenterPoint(), seek_range);
		if (neighboring_nodes.size() == 0)
		{
			printf("No navgrid nodes found in given range!\n");
			//throw("Gotcha!\n");
		}
		else
		{
			for (Creature* ptr_candidate : neighboring_nodes)
			{
				printf("Found candidate x: %f y: %f.\n", 
					ptr_candidate->TellCenterPoint().x,
					ptr_candidate->TellCenterPoint().y);
				if (ptr_my_creature->IsThisCreatureWithinSight(ptr_candidate, seek_range))
				{
					result = dynamic_cast<CreatureNavGridNode*>(ptr_candidate);
					printf("Candidate %p suitable.\n", result);
					printf("Number of connected nodes: %d.\n", result->TellConnectedNodes().size());
					for (Creature* ptr_connected_node : result->TellConnectedNodes())
					{
						printf("%p\n", ptr_connected_node);
					}
					return result;
				}
				printf("Candidate to far or behind wall!\n");
			}
		}
	}
	printf("No node found in given range!.\n");
	return result;
}

std::vector<Coordinates> LevelComponentNavGrid::GenerateRandomPathFromNode(CreatureNavGridNode* ptr_starting_node, unsigned int number_of_hops)
{
	CreatureNavGridNode* ptr_previous_node = nullptr;
	CreatureNavGridNode* ptr_current_node = ptr_starting_node;
	std::vector<Coordinates> result = {};
	for (unsigned int i = 1; i <= number_of_hops; i++)
	{
		//printf("Generating path. Run number %d.\n", i);
		result.push_back(ptr_current_node->TellCenterPoint());
		std::vector<CreatureNavGridNode*> my_connected_nodes = ptr_current_node->TellConnectedNodes();
		unsigned int neighbor_nodes_count = my_connected_nodes.size();
		if (neighbor_nodes_count > 1)
		{
			unsigned int chosen_node_number = rand() % neighbor_nodes_count;
			//#TODO - niezbyt bezpieczne
			while (my_connected_nodes[chosen_node_number] == ptr_previous_node)
			{
				chosen_node_number = rand() % neighbor_nodes_count;
			}
			ptr_previous_node = ptr_current_node;
			ptr_current_node = my_connected_nodes[chosen_node_number];
		}
		else if (neighbor_nodes_count == 1)
		{
			ptr_previous_node = ptr_current_node;
			ptr_current_node = my_connected_nodes[0];
		}
		else
		{
			printf("Connection chain broken!\n");
			break;
		}

	}
	printf("Generated random path:\n");
	for (Coordinates my_coordinate : result)
	{
		printf("x: %f, y: %f\n", my_coordinate.x, my_coordinate.y);
	}
	return result;
}

std::vector<Coordinates> LevelComponentNavGrid::GeneratePathToChosenPoint(Creature* ptr_wanderer, Coordinates end_point)
{
	CreatureNavGridNode* ptr_start_node =  FindAGridNodeInSight(ptr_wanderer);
	CreatureNavGridNode* ptr_end_node = FindAGridNodeAccessibleFromPoint(end_point);
	std::vector<Coordinates> result = {};
	if (ptr_start_node == nullptr || ptr_end_node == nullptr)
	{
		printf("Could not find suitable start/end node for path between points.\n");
		return result;
	}
	else
	{
		result = GeneratePathBetweenNodes(ptr_start_node, ptr_end_node);
		return result;
	}
}

std::vector<Coordinates> LevelComponentNavGrid::GeneratePathBetweenNodes(CreatureNavGridNode* ptr_my_start_node, CreatureNavGridNode* ptr_my_end_node)
{
	std::vector<Coordinates> result = {};
	return result;
}