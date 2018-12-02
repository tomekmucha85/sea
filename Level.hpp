#ifndef LEVEL_HPP
#define LEVEL_HPP
#include <SDL.h>
#include <vector>
#include <map>
#include <string>
#include <Creature.hpp>
#include <LevelComponent.hpp>
#include <FactorySpawningLevelComponents.hpp>

enum LevelType {level_ninemazes, level_base};

class Level
{
    private:
		//###################
        //Types
        //###################

		struct MapEntity
		{
			CreatureType type = cre_none;
			Creature* ptr_creature = NULL;
		};

		//###################
		//Variables & const
		//###################
        //Map of level components
		std::map<LevelComponentType, std::vector<LevelComponent*>> level_components = {};
		//Expressed in map blocks
		int initial_level_width = NULL;
		int initial_level_height = NULL;
		int map_offset_x = NULL;
		int map_offset_y = NULL;
		//Main map array
		std::vector<std::vector<MapEntity>> map = {};

    public:
		//Pointer to creature serving currently as hero
		Creature* ptr_hero = nullptr;
		
		
		Level();
		~Level();
		//Level components factory
		FactorySpawningLevelComponents* ptr_components_factory = nullptr;
		void RenderAllPresentCreatures();
		CreatureType PickRandomObjectFromGiven(std::vector<CreatureType> my_creatures);
		void SetMapOffsetX(int columns_count, float margin);
		void SetMapOffsetY(int rows_count, float margin);
		int TellMapOffsetX();
		int TellMapOffsetY();
		void SetCurrentHero(Creature* ptr_my_hero);
		std::map<LevelComponentType, std::vector<LevelComponent*>>* TellPointerToComponentsArray();
		FactorySpawningLevelComponents* CreateComponentsFactory();
};

#endif //LEVEL_HPP
