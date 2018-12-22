#include <LevelComponent.hpp>
//***********************************
//DEFINITIONS OF STATIC CLASS MEMBERS
//***********************************
int LevelComponent::map_block_width = 32;
int LevelComponent::map_block_height = 32;

//***********************************
//FUNCTIONS
//***********************************
LevelComponent::LevelComponent(std::map<LevelComponentType, std::vector<LevelComponent*>>* my_ptr_peer_level_components, 
	SDL_Rect my_component_area)
{
	SetPointerToPeerComponentsIndex(my_ptr_peer_level_components);
	ptr_creatures_factory = new FactorySpawningCreatures();
	//If component has a defined area, spawn a Creature serving as "mask" covering this area.
	if (my_component_area.w != 0 && my_component_area.h != 0)
	{
		AddLevelComponentOutline(my_component_area);
	}
	cyclic_actions.push_back(func_reaper);
}

LevelComponent::~LevelComponent()
{
	printf("Removing level component with address %p.\n", this);
	if (ptr_component_outline != nullptr)
	{
		printf("Coordinates of deleted component: x:%d y:%d w:%d h:%d.\n", ptr_component_outline->TellHitbox().x,
			ptr_component_outline->TellHitbox().y,
			ptr_component_outline->TellHitbox().w,
			ptr_component_outline->TellHitbox().h);
	}
	RemoveAllCreatures();
}

void LevelComponent::AddLevelComponentOutline(SDL_Rect my_component_area)
{
	SDL_Rect* ptr_my_component_area = &my_component_area;
	ptr_component_outline =  AddCreature(cre_vector_mask, ptr_my_component_area, merge);
}

void LevelComponent::SetPointerToPeerComponentsIndex(std::map<LevelComponentType, std::vector<LevelComponent*>>* my_ptr_peer_level_components)
{
	ptr_peer_level_components = my_ptr_peer_level_components;
}

std::vector<Creature*>* LevelComponent::TellPtrToCreaturesArray()
{
	std::vector<Creature*>* my_ptr_to_creatures_array = &creatures;
	return my_ptr_to_creatures_array;
}

SDL_Rect LevelComponent::TellComponentArea()
{
	SDL_Rect current_component_area = ptr_component_outline->TellHitbox();
	return current_component_area;
}

SDL_Rect LevelComponent::TellComponentEdge(Directions my_direction)
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
	SDL_Rect result = {0,0,0,0};
	SDL_Rect component_area = ptr_component_outline->TellHitbox();
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
			ptr_my_creature->FindNeighborsInSet(ptr_my_level_component->TellPtrToCreaturesArray());
			for (Creature* ptr_colliding_creature : ptr_my_creature->WhichNeighborsDoICollideWith())
			{
				result.insert(std::pair<Creature*, LevelComponent*>(ptr_colliding_creature, ptr_my_level_component));
			}
		}
	}
	return result;
}


Creature* LevelComponent::AddCreature( CreatureType my_type, SDL_Rect* ptr_my_position, InsertionMode my_mode, std::string my_trigger_signal)
{
	//Spawning creature and then checking if it can be left on map.
	Creature* ptr_my_creature = ptr_creatures_factory->SpawnCreature(my_type, ptr_my_position, my_trigger_signal);
	//Binding creature with level component
	//ptr_my_creature->SetOwner(this);

	//Merge mode leaves the Creature unconditionally
	if (my_mode == merge)
	{
		//printf("Creature %p spawned in merge mode.\n", ptr_my_creature);
		creatures.push_back(ptr_my_creature);
		Creature::current_environment.push_back(ptr_my_creature);
		return ptr_my_creature;
	}
	//Vector containing pointers to colliding neighbor creatures.
	std::map<Creature*, LevelComponent*> collisions = FindCreatureNeighborsInAllLevelComponents(ptr_my_creature);
	if (collisions.size() > 0)
	{
		//Force mode removes all colliding Creatures.
		if (my_mode == force)
		{
			//printf("Inserting object forcefully, removing all colliding ones.\n");
			//Removing obstacles
			//#TODO zoptymalizowaæ - obecnie najpierw szukamy kolizji wœród komponentów, a potem poszukujemy komponentu-w³aœciciela
			for (std::pair<Creature*, LevelComponent*> collision: collisions)
			{
				Creature* ptr_my_creature = collision.first;
				LevelComponent* ptr_my_level_component = collision.second;
				//printf("Forceful creature removal!\n");
				ptr_my_level_component->RemoveCreature(ptr_my_creature);
			}
			creatures.push_back(ptr_my_creature);
			Creature::current_environment.push_back(ptr_my_creature);
			return ptr_my_creature;
		}
		//Safe mode does not insert Creature if collision occurs.
		else if (my_mode == safe)
		{
			//printf("Not inserting object, because safe mode is on and collision(s) were detected.\n");
			delete ptr_my_creature;
			return nullptr;
		}
		else
		{
			//printf("Unexpected event while adding creature - insertion mode unknown.\n");
			delete ptr_my_creature;
			return nullptr;
		}
	}
	else
	{
		//printf("No collisions detected during Creature %p spawn.\n", ptr_my_creature);
		creatures.push_back(ptr_my_creature);
		Creature::current_environment.push_back(ptr_my_creature);
		return ptr_my_creature;
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

std::vector<Creature*> LevelComponent::FindCollisionsWithMainCharacter(bool check_only_obstacles)
{
	Creature* ptr_main_character = Creature::ptr_current_main_charater;
	std::vector<Creature*> colliding_creatures = {};
	colliding_creatures = ptr_main_character->FindCollisionsInSet(&creatures, check_only_obstacles);
	return colliding_creatures;
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