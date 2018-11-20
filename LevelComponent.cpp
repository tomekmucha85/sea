#include <algorithm>
#include <LevelComponent.hpp>

void LevelComponent::SetPointerToPeerComponentsIndex(std::map<LevelComponentType, std::vector<LevelComponent*>>* my_ptr_peer_level_components)
{
	ptr_peer_level_components = my_ptr_peer_level_components;
}

std::vector<Creature*>* LevelComponent::TellPtrToCreaturesArray()
{
	std::vector<Creature*>* my_ptr_to_creatures_array = &creatures;
	return my_ptr_to_creatures_array;
}

Creature* LevelComponent::AddCreature( CreatureType my_type, SDL_Rect* ptr_my_position, InsertionMode my_mode)
{
	//Spawning creature and then checking if it can be left on map.
	Creature* ptr_my_creature = Creature::SpawnCreature(my_type, ptr_my_position);

	if (my_mode == merge)
	{
		return ptr_my_creature;
	}
	// Check for collision in every component
	//std::map<LevelComponentType, std::vector<LevelComponent*>>::iterator it = ptr_peer_level_components->begin();
	ptr_my_creature->RemoveNeighbors();
	//Iterating over map of Level Component types vs vectors containing pointers to them
	//#TODO rename "peer" to "all"
	for (std::pair<LevelComponentType, std::vector<LevelComponent*>> element : *ptr_peer_level_components)
	{
		std::vector<LevelComponent*> my_level_components = element.second;
		for (LevelComponent* ptr_my_level_component : my_level_components)
		{
			ptr_my_creature->FindNeighborsInSet(ptr_my_level_component->TellPtrToCreaturesArray());
		}
	}
	//Vector containing pointers to colliding neighbor creatures.
	std::vector<Creature*> collisions = ptr_my_creature->WhichNeighborsDoICollideWith();
	if (collisions.size() > 0)
	{
		if (my_mode == force)
		{
			printf("Inserting object forcefully.\n");
			for (Creature* ptr_colliding_creature : collisions)
			{
				// #TODO Do zawo³ania funkcja usuwaj¹ca!
				RemoveCreature(ptr_colliding_creature);
			}
			return ptr_my_creature;
		}
		else if (my_mode == safe)
		{
			printf("Not inserting object, because safe mode is on and collision(s) were detected.\n");
			return nullptr;
		}
		else
		{
			printf("Unexpected event while adding creature - insertion mode unknown.\n");
			return nullptr;
		}
	}
	else
	{
		return ptr_my_creature;
	}
}

//#TODO czy na pewno tak? Manipulujemy przy cudzym obiekcie!
void LevelComponent::RemoveCreature(Creature* ptr_my_creature)
{
	//Removing pointer from level components address book
	for (std::pair<LevelComponentType, std::vector<LevelComponent*>> element : *ptr_peer_level_components)
	{
		std::vector<LevelComponent*> my_level_components = element.second;
		for (LevelComponent* ptr_my_level_component : my_level_components)
		{
			std::vector<Creature*>* ptr_creatures_vector = ptr_my_level_component->TellPtrToCreaturesArray();
			ptr_creatures_vector->erase(std::remove(ptr_creatures_vector->begin(), 
				ptr_creatures_vector->end(), ptr_my_creature), ptr_creatures_vector->end());
		}
	}
	//Destroying creature.
	ptr_my_creature->~Creature();
}