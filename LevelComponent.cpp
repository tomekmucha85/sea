#include <LevelComponent.hpp>
//***********************************
//DEFINITIONS OF STATIC CLASS MEMBERS
//***********************************
int LevelComponent::map_block_width = 48;
int LevelComponent::map_block_height = 48;

//***********************************
//FUNCTIONS
//***********************************
LevelComponent::LevelComponent(std::map<LevelComponentType, std::vector<LevelComponent*>>* my_ptr_peer_level_components, 
	PreciseRect my_component_area)
{
	SetPointerToPeerComponentsIndex(my_ptr_peer_level_components);
	ptr_creatures_factory = new FactorySpawningCreatures();
	//If component has a defined area, spawn a Creature serving as "mask" covering this area.
	if (my_component_area.w != 0 && my_component_area.h != 0)
	{
		AddLevelComponentOutline(my_component_area);
	}
	cyclic_actions.push_back(func_spawn_creatures_on_other_creature_demand);
	cyclic_actions.push_back(func_spawn_creatures_on_peer_component_demand);
	cyclic_actions.push_back(func_destroy_creatures_on_peer_component_demand);
	cyclic_actions.push_back(func_reaper);
	cyclic_actions.push_back(func_send_through_path_requests);
}

LevelComponent::~LevelComponent()
{
	delete ptr_creatures_factory;
	//printf("Removing level component with address %p.\n", this);
	if (ptr_component_outline != nullptr)
	{
		/*printf("Coordinates of deleted component: x:%f y:%f w:%f h:%f.\n", 
			ptr_component_outline->TellHitbox().x,
			ptr_component_outline->TellHitbox().y,
			ptr_component_outline->TellHitbox().w,
			ptr_component_outline->TellHitbox().h);*/
	}
	RemoveAllCreaturesExceptHero();
}

void LevelComponent::AddLevelComponentOutline(PreciseRect my_component_area)
{
	PreciseRect* ptr_my_component_area = &my_component_area;
	ptr_component_outline =  AddCreature(cre_vector_mask, ptr_my_component_area, merge);
}

void LevelComponent::SetPointerToPeerComponentsIndex(std::map<LevelComponentType, std::vector<LevelComponent*>>* my_ptr_peer_level_components)
{
	ptr_peer_level_components = my_ptr_peer_level_components;
}

std::map<LevelComponentType, std::vector<LevelComponent*>>* LevelComponent::TellPtrToPeerLevelComponentsArray()
{
	return ptr_peer_level_components;
}

std::vector<Creature*>* LevelComponent::TellPtrToCreaturesArray()
{
	std::vector<Creature*>* my_ptr_to_creatures_array = &creatures;
	return my_ptr_to_creatures_array;
}

PreciseRect LevelComponent::TellComponentArea()
{
	PreciseRect current_component_area = ptr_component_outline->TellHitbox();
	return current_component_area;
}

PreciseRect LevelComponent::TellComponentEdge(Directions my_direction)
//Returns rectangle 1 pixel high or wide covering entire border of component.
//Applied only to components with specified dimiensions!
/*

Example:

level component:

0,0
---------------
|             |
|             |
|             |
---------------
               800,600

SDL_Rect nothern_border = level_component.TellComponentEdge(north);		   
northern_border == {0,0,800,1}

*/
{
	PreciseRect result = {0,0,0,0};
	PreciseRect component_area = ptr_component_outline->TellHitbox();
	if (my_direction == north)
	{
		result = { component_area.x, component_area.y, component_area.w , 1 };
	}
	else if (my_direction == south)
	{
		result = { component_area.x, component_area.y + component_area.h - 2, component_area.w - 1, 1 };
	}
	else if (my_direction == west)
	{
		result = { component_area.x, component_area.y, 1, component_area.h };
	}
	else if (my_direction == east)
	{
		result = { component_area.x + component_area.w - 2, component_area.y, 1, component_area.h };
	}
	else
	{
		throw "Unknown direction!";
	}
	return result;
}

std::vector<Creature*> LevelComponent::FindCreaturesInRadius(Coordinates center_point, double radius)
{
	std::vector<Creature*> result = {};
	for (Creature* ptr_my_creature : creatures)
	{
		double distance_to_creature = Distance::CalculateDistanceBetweenPoints(
		ptr_my_creature->TellCenterPoint(),
			center_point);
		if (distance_to_creature <= radius)
		{
			result.push_back(ptr_my_creature);
		}
	}
	//printf("Found %d creatures in given radius.\n", result.size());
	return result;
}

std::map<Creature*, LevelComponent*> LevelComponent::FindCreatureNeighborsInAllLevelComponents(Creature* ptr_my_creature)
{
	std::map<Creature*, LevelComponent*> result = {};
	//Iterating over map of Level Component types vs vectors containing pointers to them
	for (std::pair<LevelComponentType, std::vector<LevelComponent*>> element : *ptr_peer_level_components)
	{
		std::vector<LevelComponent*> my_level_components = element.second;
		for (LevelComponent* ptr_my_level_component : my_level_components)
		{
			ptr_my_creature->RemoveNeighbors();
			//#TODO - uproœciæ szukanie s¹siadów
			std::vector<Creature*> found_neighbors = ptr_my_creature->FindNeighborsInSet(ptr_my_level_component->TellPtrToCreaturesArray());
			ptr_my_creature->AddToNeighbors(found_neighbors);
			for (Creature* ptr_colliding_creature : ptr_my_creature->WhichNeighborsDoICollideWith())
			{
				result.insert(std::pair<Creature*, LevelComponent*>(ptr_colliding_creature, ptr_my_level_component));
			}
		}
	}
	return result;
}


Creature* LevelComponent::AddCreature(CreatureType my_type, PreciseRect* ptr_my_position, InsertionMode my_mode, std::string my_trigger_signal)
{
	//Spawning creature and then checking if it can be left on map.
	Creature* ptr_my_creature = ptr_creatures_factory->SpawnCreature(my_type, ptr_my_position, my_trigger_signal);
	creatures.push_back(ptr_my_creature);
	Creature::current_environment.push_back(ptr_my_creature);
	if (DetermineIfCreatureCanBeLeftOnMap(ptr_my_creature, my_mode))
	{
		return ptr_my_creature;
	}
	else
	{
		return nullptr;
	}
}

Creature* LevelComponent::AddCreature(CreatureType my_type, Coordinates* ptr_my_center, InsertionMode my_mode, int my_render_layer)
{
	//Spawning creature and then checking if it can be left on map.
	Creature* ptr_my_creature = ptr_creatures_factory->SpawnCreature(my_type, ptr_my_center, my_render_layer);
	creatures.push_back(ptr_my_creature);
	Creature::current_environment.push_back(ptr_my_creature);
	if (DetermineIfCreatureCanBeLeftOnMap(ptr_my_creature, my_mode))
	{
		return ptr_my_creature;
	}
	else
	{
		return nullptr;
	}
}

void LevelComponent::AddExistingCreature(Creature* ptr_my_creature)
{
	//#TODO - zabezpieczyæ siê przed dowi¹zaniem Creature jednoczeœnie do dwóch komponentów
	creatures.push_back(ptr_my_creature);
}

bool LevelComponent::DetermineIfCreatureCanBeLeftOnMap(Creature* ptr_my_creature, InsertionMode my_mode)
{
	//Merge mode leaves the Creature unconditionally
	if (my_mode == merge)
	{
		return true;
	}
	//Vector containing pointers to colliding neighbor creatures.
	std::map<Creature*, LevelComponent*> neighbors = FindCreatureNeighborsInAllLevelComponents(ptr_my_creature);
	if (neighbors.size() > 0)
	{
		//Force mode removes all colliding Creatures.
		if (my_mode == force)
		{
			//printf("Inserting object forcefully, removing all colliding ones.\n");
			//Removing obstacles
			for (std::pair<Creature*, LevelComponent*> neighbor : neighbors)
			{
				Creature* ptr_my_neighbor_creature = neighbor.first;
				LevelComponent* ptr_my_neighbor_level_component = neighbor.second;
				if (ptr_my_creature->DoICollideWithThisCreature(ptr_my_neighbor_creature))
				{
					//printf("Forceful creature removal!\n");
					ptr_my_neighbor_level_component->RemoveCreature(ptr_my_neighbor_creature);
				}
			}
			return true;
		}
		//Safe mode does not insert Creature if collision occurs.
		else if (my_mode == safe)
		{
			for (std::pair<Creature*, LevelComponent*> neighbor : neighbors)
			{
				Creature* ptr_my_neighbor_creature = neighbor.first;
				LevelComponent* ptr_my_neighbor_level_component = neighbor.second;
				if (ptr_my_creature->DoICollideWithThisCreature(ptr_my_neighbor_creature))
				{
					printf("Not inserting object, because safe mode is on and collision(s) were detected.\n");
					RemoveCreature(ptr_my_creature);
					return false;
				}
			}
			return true;
		}
		else
		{
			printf("Unexpected event while adding creature - insertion mode unknown.\n");
			throw std::invalid_argument("Unexpected event while adding creature - insertion mode unknown.\n");
		}
	}
	else
	{
		//printf("No neighbors detected during Creature %p spawn.\n", ptr_my_creature);
		return true;
	}
}

void LevelComponent::RemoveCreature(Creature* ptr_my_creature)
{
	//Removing pointer from my creatures
	//printf("Attempting to remove: %p.\n", ptr_my_creature);
	creatures.erase(std::remove(creatures.begin(), creatures.end(), ptr_my_creature), creatures.end());
	//Removing pointer from Creature class current environment static vector
	Creature::current_environment.erase(std::remove(Creature::current_environment.begin(),
	    Creature::current_environment.end(), ptr_my_creature), Creature::current_environment.end());
	//Destroying creature.
	delete ptr_my_creature;
}

void LevelComponent::RemoveAllCreatures()
{
	int i = 1;
	//Temporary vector holding copy of "creatures"
	//Necessary, 'cause we don't want to iterate over vector and modify it simultaneously!
	std::vector<Creature*> creatures_to_remove = creatures;
	for (Creature* ptr_my_creature : creatures_to_remove)
	{
		//printf("Creature number %d, address: %p.\n", i, ptr_my_creature);
		i++;
		//printf("Remaining creatures: %d.\n", creatures.size());
		//printf("Removing creature of type %d.\n", ptr_my_creature->my_type);
		RemoveCreature(ptr_my_creature);
	}
}

void LevelComponent::RemoveAllCreaturesExceptHero()
{
	int i = 1;
	std::vector<Creature*> creatures_to_remove = creatures;
	for (Creature* ptr_my_creature : creatures_to_remove)
	{
		i++;
		if (ptr_my_creature != Creature::ptr_current_main_charater)
		{
			RemoveCreature(ptr_my_creature);
		}
	}
}


std::vector<Creature*> LevelComponent::FindCollisionsWithMainCharacter(bool check_only_obstacles)
{
	Creature* ptr_main_character = Creature::ptr_current_main_charater;
	std::vector<Creature*> colliding_creatures = {};
	colliding_creatures = ptr_main_character->FindCollisionsInSet(&creatures, check_only_obstacles);
	return colliding_creatures;
}

void LevelComponent::SetVisibilityForAllCreatures(bool should_be_visible)
{
	for (Creature* ptr_my_creature : creatures)
	{
		ptr_my_creature->SetVisibility(should_be_visible);
	}
}

//##############################
//Spawn and destruction requests
//##############################

void LevelComponent::ServeSpawnRequest(CreatureSpawnRequest my_request)
{
	Creature* ptr_spawned_creature = nullptr;

	if (my_request.mode == area_coordinates)
	{
		ptr_spawned_creature = AddCreature(my_request.type, &my_request.initial_area, my_request.insertion_mode);
	}
	else if (my_request.mode == center_coordinates)
	{
		ptr_spawned_creature = AddCreature(my_request.type, &my_request.initial_center_cooridnates, my_request.insertion_mode);
	}
	else
	{
		printf("Trying to serve a spawn request, but positioning mode %d is not set correctly!\n", my_request.mode);
		throw std::invalid_argument("Trying to serve a spawn request, but positioning mode is not set correctly!\n");
	}

	if (ptr_spawned_creature != nullptr)
	{
		ptr_spawned_creature->SetAngleDegree(my_request.initial_angle_degree);
		ptr_spawned_creature->SetBehaviorMode(my_request.initial_behavior_mode);
		if (ptr_spawned_creature->visual_components[0]->TellMyType() == visco_vector)
		{
			ptr_spawned_creature->visual_components[0]->SetColor(my_request.color);
		}
	}
	else
	{
		"Did not spawn requested creature!\n";
	}
}

void LevelComponent::ServeInternalSpawnRequest(CreatureSpawnRequest my_request)
/*
Serving internal spawn requests for creatures. Internal requests are placed by creatures belonging to 
this level component.
*/
{
	ServeSpawnRequest(my_request);
}

void LevelComponent::ServeExternalSpawnRequest(CreatureSpawnRequest my_request)
/*
Serving external spawn requests for creatures. Internal requests are placed by peer level components.
*/
{
	ServeSpawnRequest(my_request);
}

void LevelComponent::ServeAllExternalSpawnRequests()
{
	for (CreatureSpawnRequest request_to_serve : external_spawn_requests)
	{
		ServeExternalSpawnRequest(request_to_serve);
	}
	external_spawn_requests.clear();
}

void LevelComponent::ServeExternalDestructionRequest(CreatureDestructionInGivenAreaRequest my_request)
{
	//printf("Destruction request to be served. Area: x: %f y: %f w: %f h: %f.\n",
	//	my_request.nukage_area.x, my_request.nukage_area.y, my_request.nukage_area.w, my_request.nukage_area.h);
	std::vector<Creature*> creatures_copy = creatures;
	for (Creature* ptr_my_creature : creatures_copy)
	{

		if (Collisions::DoTheseRectanglesOverlap(ptr_my_creature->TellHitbox(), my_request.nukage_area) &&
			(ptr_my_creature->my_type == my_request.kind_to_be_destroyed || 
				my_request.kind_to_be_destroyed == cre_none))
		{
			//printf("Will destruct!\n");
			//printf("Creature to be destructed (center): x: %f y: %f .\n", ptr_my_creature->TellCenterPoint().x, ptr_my_creature->TellCenterPoint().y);
			RemoveCreature(ptr_my_creature);
		}
		else
		{
			//printf("Will NOT destruct!\n");
			//printf("Creature to be spared (center): x: %f y: %f.\n", ptr_my_creature->TellCenterPoint().x, ptr_my_creature->TellCenterPoint().y);
		}
	}
}

void LevelComponent::PushIntoExternalSpawnRequests(CreatureSpawnRequest my_request)
{
	external_spawn_requests.push_back(my_request);
}

void LevelComponent::SendSpawnRequestToPeerComponent(CreatureSpawnRequest my_request, LevelComponent* ptr_peer_component)
{
	ptr_peer_component->PushIntoExternalSpawnRequests(my_request);
}

void LevelComponent::SendDestructionRequestToPeerComponent(CreatureDestructionInGivenAreaRequest my_request, LevelComponent* ptr_peer_component)
{
	//printf("Destruction request pushed into external vector.\n");
	ptr_peer_component->PushIntoExternalDestructionRequests(my_request);
}

void LevelComponent::PushIntoExternalDestructionRequests(CreatureDestructionInGivenAreaRequest my_request)
{
	external_destruction_requests.push_back(my_request);
}

//##############################
//Path requests
//##############################
void LevelComponent::SendPathRequest(RandomPathRequest my_request)
{
	RandomPathRequestEncalpsulated encapsulated_request = EncapsulatePathRequest(my_request);
	path_requests_encapsulated.push_back(encapsulated_request);
	printf("Level component will send a path request.\n");
}


RandomPathRequestEncalpsulated LevelComponent::EncapsulatePathRequest(RandomPathRequest my_request)
{
	RandomPathRequestEncalpsulated result;
	result.source_component = this;
	result.requestor_id = my_request.requestor_id;
	result.requested_hops_length = my_request.requested_hops_length;
	return result;
}

RandomPathResponse LevelComponent::DecapsulatePathResponse(RandomPathResponseEncapsulated my_response)
{
	RandomPathResponse result;
	result.navigation_path = my_response.navigation_path;
	result.requestor_id = my_response.requestor_id;
	return result;
}

void LevelComponent::SendAllPathRequests()
{
	//printf("Level component will send all path requests.\n");
	for (Creature* ptr_my_creature : creatures)
	{
		if (ptr_my_creature->path_requests.size() > 0)
		{
			for (RandomPathRequest my_request : ptr_my_creature->path_requests)
			{
				SendPathRequest(my_request);
			}
			ptr_my_creature->path_requests.clear();
		}
	}
}


void LevelComponent::DeliverPathResponse(RandomPathResponseEncapsulated my_response)
{
	printf("Level component will deliver path response.\n");
	RandomPathResponse response_decapsulated = DecapsulatePathResponse(my_response);
	Creature* receiving_creature = response_decapsulated.requestor_id;
	if (std::find(creatures.begin(), creatures.end(), receiving_creature) != creatures.end())
	{
		receiving_creature->MakeUseOfPathResponse(response_decapsulated);
	}
}



//##############################
//Cyclic actions
//##############################

void LevelComponent::PerformCyclicActions()
{
	//Perform cyclic actions related to this level component
	for (std::function<void(LevelComponent*)> action : cyclic_actions)
	{
		action(this);
	}
	//Perform cyclic actions related to owned creatures.
	MakeCreaturesPerformCyclicActions();
}

void LevelComponent::MakeCreaturesPerformCyclicActions()
{
	for (Creature* ptr_creature : creatures)
	{
		ptr_creature->PerformCyclicActions();
	}
}

//##############################
//Virtual functions
//##############################

void LevelComponent::ClearMaze()
{
	printf("ClearMaze has no implementation here!\n");
}

void LevelComponent::GenerateMaze()
{
	printf("GenerateMaze has no implementation here!\n");
}

void LevelComponent::SetBorderState(Directions border_side, bool value)
{
	printf("SetBordersState has no implementation here!\n");
}

bool LevelComponent::TellBorderState(Directions border_side)
{
	printf("TallBorderState has no implementation here!\n");
	//#TODO czy wszêdzie daæ wyj¹tki?
	throw "No implementation!\n";
	return true;
}

 std::vector<std::string> LevelComponent::RunTriggersHitByHero()
 {
	 std::vector<std::string> result = {};
	 printf("RunTriggersHitByHero has no implementation here!\n");
	 throw "No implementation!\n";
	 return result;
 }