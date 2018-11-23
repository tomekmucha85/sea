#include<FactorySpawningLevelComponents.hpp>

FactorySpawningLevelComponents::FactorySpawningLevelComponents(std::map<LevelComponentType, std::vector<LevelComponent*>>* ptr_my_level_components_array)
{
	SetPointerToComponentsArray(ptr_my_level_components_array);
}

void FactorySpawningLevelComponents::SetPointerToComponentsArray(std::map<LevelComponentType, std::vector<LevelComponent*>>* ptr_my_level_components_array)
{
	ptr_level_components_array = ptr_my_level_components_array;
}

void FactorySpawningLevelComponents::EnsureKeyExistsInComponentsArray(LevelComponentType my_type)
{
	//Insert key if not existing yet.
	if (ptr_level_components_array->find(my_type) == ptr_level_components_array->end())
	{
		ptr_level_components_array->insert({ my_type,{} });
	}
}

void FactorySpawningLevelComponents::InsertComponentIntoDesiredPlaceInComponentsArray(LevelComponentType my_type, LevelComponent* ptr_my_level_component, int my_index)
{
	//Access specific map element via pointer to map
	std::vector<LevelComponent*>* ptr_to_proper_array = &ptr_level_components_array->operator[](my_type);
	ptr_to_proper_array->insert(ptr_to_proper_array->begin() + my_index, ptr_my_level_component);
}

LevelComponent* FactorySpawningLevelComponents::SpawnLevelComponent(LevelComponentType my_type, int my_component_index, SDL_Rect* ptr_my_component_area)
{
	LevelComponent* result = nullptr;

	if (my_type == levco_core)
	{
		LevelComponent* ptr_my_component_core = new LevelComponentCore(ptr_level_components_array);
		EnsureKeyExistsInComponentsArray(my_type);
		InsertComponentIntoDesiredPlaceInComponentsArray(my_type, ptr_my_component_core, my_component_index);
		return ptr_my_component_core;
	}
	else if (my_type == levco_maze)
	{
		;
	}
	else if (my_type == levco_powerups)
	{
		;
	}
	else if (my_type == levco_test)
	{
		LevelComponent* ptr_my_component_test = new LevelComponentTest(ptr_level_components_array);
		EnsureKeyExistsInComponentsArray(my_type);
		InsertComponentIntoDesiredPlaceInComponentsArray(my_type, ptr_my_component_test, my_component_index);
		return ptr_my_component_test;
	}
	else
	{
		throw "Unknown level component!\n";
	}

	return result;
}