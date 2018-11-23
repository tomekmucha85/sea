#include <LevelComponent.hpp>

LevelComponent::LevelComponent(std::map<LevelComponentType, std::vector<LevelComponent*>>* my_ptr_peer_level_components)
{
	SetPointerToPeerComponentsIndex(my_ptr_peer_level_components);
	ptr_creatures_factory = new FactorySpawningCreatures();
}

LevelComponent::~LevelComponent()
{
	for (Creature* ptr_my_creature : creatures)
	{
		RemoveCreature(ptr_my_creature);
	}
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

std::vector<Creature*> LevelComponent::FindCreatureNeighborsInAllLevelComponents(Creature* ptr_my_creature)
{
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
	return ptr_my_creature->WhichNeighborsDoICollideWith();
}

Creature* LevelComponent::AddCreature( CreatureType my_type, SDL_Rect* ptr_my_position, InsertionMode my_mode)
{
	//Spawning creature and then checking if it can be left on map.
	Creature* ptr_my_creature = ptr_creatures_factory->SpawnCreature(my_type, ptr_my_position);
	//Binding creature with level component
	//ptr_my_creature->SetOwner(this);

	//Merge mode leaves the Creature unconditionally
	if (my_mode == merge)
	{
		printf("Creature %p spawned in merge mode.\n", ptr_my_creature);
		creatures.push_back(ptr_my_creature);
		Creature::current_environment.push_back(ptr_my_creature);
		return ptr_my_creature;
	}
	//Vector containing pointers to colliding neighbor creatures.
	std::vector<Creature*> collisions = FindCreatureNeighborsInAllLevelComponents(ptr_my_creature);
	if (collisions.size() > 0)
	{
		//Force mode removes all colliding Creatures.
		if (my_mode == force)
		{
			printf("Inserting object forcefully, removing all colliding ones.\n");
			//Removing obstacles
			//#TODO zoptymalizowaæ - obecnie najpierw szukamy kolizji wœród komponentów, a potem poszukujemy komponentu-w³aœciciela
			for (Creature* ptr_colliding_creature : collisions)
			{
				LevelComponent* ptr_colliding_creature_owner = nullptr/*ptr_colliding_creature->WhoIsMyOwner()*/;
				if (ptr_colliding_creature_owner != nullptr)
				{
					ptr_colliding_creature_owner->RemoveCreature(ptr_colliding_creature);
				}
				else
				{
					throw "No owner for creature found in current level!";
				}
			}
			creatures.push_back(ptr_my_creature);
			Creature::current_environment.push_back(ptr_my_creature);
			return ptr_my_creature;
		}
		//Safe mode does not insert Creature if collision occurs.
		else if (my_mode == safe)
		{
			printf("Not inserting object, because safe mode is on and collision(s) were detected.\n");
			delete ptr_my_creature;
			return nullptr;
		}
		else
		{
			printf("Unexpected event while adding creature - insertion mode unknown.\n");
			delete ptr_my_creature;
			return nullptr;
		}
	}
	else
	{
		printf("No collisions detected during Creature %p spawn.\n", ptr_my_creature);
		creatures.push_back(ptr_my_creature);
		Creature::current_environment.push_back(ptr_my_creature);
		return ptr_my_creature;
	}
}

void LevelComponent::RemoveCreature(Creature* ptr_my_creature)
{
	//Removing pointer from my creatures
	creatures.erase(std::remove(creatures.begin(), creatures.end(), ptr_my_creature), creatures.end());
	Creature::current_environment.erase(std::remove(Creature::current_environment.begin(),
		Creature::current_environment.end(), ptr_my_creature), Creature::current_environment.end());
	//Destroying creature.
	delete ptr_my_creature;
}