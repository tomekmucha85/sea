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