#ifndef LEVEL_HPP
#define LEVEL_HPP
#include <SDL.h>
#include <vector>
#include <map>
#include <functional>
#include <string>
#include <stdlib.h>
#include <ctime>
#include <Creature.hpp>
#include <LevelComponent.hpp>
#include <FactorySpawningLevelComponents.hpp>
#include <CustomDefinedTypes.hpp>

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
		//std::vector<std::vector<MapEntity>> map = {};

    public:
		//Contains actions associated to specific level which will be performed during every game loop.
		//std::vector<TriggeredEvent> cyclic_actions = {};
		std::vector<std::function<void(Level*)>> cyclic_actions = {};

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
		void PerformCyclicActions();
		std::vector<Creature*> FindHeroColissionsInGivenComponent(LevelComponent* ptr_my_component, bool check_only_obstacles=true);
		void RunTriggersHitByHero(LevelComponent* ptr_component_with_triggers);
};

#endif //LEVEL_HPP
