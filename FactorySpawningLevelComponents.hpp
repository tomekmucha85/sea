#ifndef FACTORY_SPAWNING_LEVEL_COMPONENTS_HPP
#define FACTORY_SPAWNING_LEVEL_COMPONENTS_HPP
#include <SDL.h>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <LevelComponent.hpp>
#include <LevelComponentTest.hpp>
#include <LevelComponentCore.hpp>
#include <LevelComponentMaze.hpp>

class FactorySpawningLevelComponents
{
    private:
		std::map<LevelComponentType, std::vector<LevelComponent*>>* ptr_level_components_array;

    public:
		FactorySpawningLevelComponents(std::map<LevelComponentType, std::vector<LevelComponent*>>* ptr_my_level_components_array);
		void SetPointerToComponentsArray(std::map<LevelComponentType, std::vector<LevelComponent*>>* ptr_my_level_components_array);
	    LevelComponent* SpawnLevelComponent(LevelComponentType my_type, int my_component_index, SDL_Rect* ptr_my_component_area=nullptr);
		void EnsureKeyExistsInComponentsArray(LevelComponentType my_type);
		void InsertComponentIntoDesiredPlaceInComponentsArray(LevelComponentType my_type, LevelComponent* ptr_my_level_component, int my_index);
};

#endif //FACTORY_SPAWNING_LEVEL_COMPONENTS_HPP