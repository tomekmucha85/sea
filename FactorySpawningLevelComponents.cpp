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

void FactorySpawningLevelComponents::InsertComponentIntoComponentsArray(LevelComponentType my_type, LevelComponent* ptr_my_level_component)
{
	//Access specific map element via pointer to map
	std::vector<LevelComponent*>* ptr_to_proper_array = &ptr_level_components_array->operator[](my_type);
	ptr_to_proper_array->push_back(ptr_my_level_component);
}

LevelComponent* FactorySpawningLevelComponents::SpawnLevelComponent(LevelComponentType my_type)
{
	if (my_type == levco_core)
	{
		LevelComponent* ptr_my_component_core = new LevelComponentCore(ptr_level_components_array);
		EnsureKeyExistsInComponentsArray(my_type);
		InsertComponentIntoComponentsArray(my_type, ptr_my_component_core);
		ptr_my_component_core->my_type = levco_core;
		return ptr_my_component_core;
	}
	else if (my_type == levco_powerups)
	{
		;
	}
	else if (my_type == levco_test)
	{
		LevelComponent* ptr_my_component_test = new LevelComponentTest(ptr_level_components_array);
		EnsureKeyExistsInComponentsArray(my_type);
		InsertComponentIntoComponentsArray(my_type, ptr_my_component_test);
		ptr_my_component_test->my_type = levco_test;
		return ptr_my_component_test;
	}
	else if (my_type == levco_triggers)
	{
		LevelComponent* ptr_my_component_triggers = new LevelComponentTriggers(ptr_level_components_array);
		EnsureKeyExistsInComponentsArray(my_type);
		InsertComponentIntoComponentsArray(my_type, ptr_my_component_triggers);
		ptr_my_component_triggers->my_type = levco_triggers;
		return ptr_my_component_triggers;
	}
	else if (my_type == levco_navgrid)
	{
		LevelComponent* ptr_my_component_navgrid = new LevelComponentNavGrid(ptr_level_components_array);
		EnsureKeyExistsInComponentsArray(my_type);
		InsertComponentIntoComponentsArray(my_type, ptr_my_component_navgrid);
		ptr_my_component_navgrid->my_type = levco_navgrid;
		return ptr_my_component_navgrid;
	}
	else
	{
		throw "Unknown level component or not suitable argument list!\n";
	}
}

LevelComponent* FactorySpawningLevelComponents::SpawnLevelComponent(LevelComponentType my_type, PreciseRect my_component_area)
{
	if (my_type == levco_maze)
	{
		if (my_component_area.w == 0 && my_component_area.h == 0)
		{
			std::string exception_text = "No valid area specified for maze level component!\n";
			throw std::invalid_argument(exception_text);
		}
		else
		{
			LevelComponent* ptr_my_component_maze = new LevelComponentMaze(ptr_level_components_array, my_component_area);
			EnsureKeyExistsInComponentsArray(my_type);
			InsertComponentIntoComponentsArray(my_type, ptr_my_component_maze);
			ptr_my_component_maze->my_type = levco_maze;
			printf("Spawned maze with address %p.\n", ptr_my_component_maze);
			return ptr_my_component_maze;
		}
	}
	else
	{
		throw "Unknown level component or not suitable argument list!\n";
	}
}