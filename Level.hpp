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
#include <GUI.hpp>
#include <CustomDefinedTypes.hpp>

class Level
{
    private:
		//###################
        //Types
        //###################
		
		bool has_player_lost = false;
		bool has_player_won = false;

		//###################
		//Variables & const
		//###################
		GUI* ptr_gui = nullptr;
        //Map of level components
		std::map<LevelComponentType, std::vector<LevelComponent*>> level_component_types_vs_level_components = {};
		//Expressed in map blocks
		int initial_level_width = NULL;
		int initial_level_height = NULL;
		int map_offset_x = NULL;
		int map_offset_y = NULL;
		bool is_paused = false;

    public:
		//Default position for hero spawned on this level
		Coordinates default_hero_start_position = {400,380};
		//Contains actions associated to specific level which will be performed during every game loop.
		std::vector<std::function<void(Level*)>> cyclic_actions = {};
		//Contains assignment of trigers to certain creatures
		std::map<std::string, std::function<void()>> signals_vs_events = {};

		//#TODO - zabezpieczyæ domyœlne komponenty przed zniszczeniem

		//Core level component - always created
		LevelComponent* ptr_initial_core_component = nullptr;
		//Initial level component for triggers - always created
		LevelComponent* ptr_initial_triggers_component = nullptr;
		//Initial level component for navigation grid - always created
		LevelComponent* ptr_initial_navgrid_component = nullptr;
		Level();
		virtual ~Level();
		//Level components factory
		Creature* SpawnHero(CreatureType hero_type = cre_clawy, Coordinates* ptr_hero_position = nullptr);
		void LoadLevel();
		void LoadLevelCreaturesIntoEnvironment();
		FactorySpawningLevelComponents* ptr_components_factory = nullptr;
		void RenderCreatureVisualComponent(Creature* ptr_my_creature);
		void RenderAllPresentCreatures();
		void RenderGui();
		GUI* TellMyGui();
		CreatureType PickRandomObjectFromGiven(std::vector<CreatureType> my_creatures);
		void SetMapOffsetX(int columns_count, float margin);
		void SetMapOffsetY(int rows_count, float margin);
		int TellMapOffsetX();
		int TellMapOffsetY();
		std::map<LevelComponentType, std::vector<LevelComponent*>>* TellPointerToComponentsArray();
		std::vector<LevelComponent*>*TellPointerToSpecificComponentTypeArray(LevelComponentType my_type);
		FactorySpawningLevelComponents* CreateComponentsFactory();
		void RemoveLevelComponent(LevelComponent* ptr_my_component);
		void RemoveAllLevelComponents();
		//#TODO - dopisaæ metodê dodaj¹c¹ level component zamiast obecnych dzia³añ na wskaŸniku do fabryki

		//Shortcut methods for adding Creatures using level components genereated by default.
		Creature* AddTriggerUsingDefaultComponent(PreciseRect my_trigger_area, std::string my_trigger_signal);
		Creature* AddNavigationNodeUsingDefaultComponent(Coordinates my_center);

		void PerformCyclicActions();
		void MakeLevelComponentsPerformCyclicActions();
		std::vector<Creature*> FindHeroColissionsInGivenComponent(LevelComponent* ptr_my_component, bool check_only_obstacles=true);
		void PerformActionsForTriggersHitByHero(LevelComponent* ptr_component_with_triggers);
		bool CheckIfPlayerIsAlive();
		bool TellIfPlayerWon();
		bool TellIfPlayerLost();
		void Win();
		virtual void FinishLevel(LevelEnding my_ending);
		void Loose();
		void Pause();
		void UnPause();
		bool TellIfPaused();

		//###################
		// COMMON LAMBDAS
		//###################

		//Cyclic action to fire all triggers hit by hero
		std::function<void(Level*)> func_fire_triggers = [](Level* ptr_level)
		{
			std::vector<LevelComponent*>* ptr_trigger_components_vector = ptr_level->TellPointerToSpecificComponentTypeArray(levco_triggers);
			for (LevelComponent* ptr_component_with_triggers : *ptr_trigger_components_vector)
			{
				ptr_level->PerformActionsForTriggersHitByHero(ptr_component_with_triggers);
			}
		};

		//Cyclic action to check if winning conditions were met
		std::function<void(Level*)> func_check_and_react_if_player_won = [](Level* ptr_level)
		{
			if (ptr_level->TellIfPlayerWon())
			{
				ptr_level->FinishLevel(victory);
			}
		};

		//Cyclic action to check if losing conditions were met
		std::function<void(Level*)> func_check_and_react_if_player_lost = [](Level* ptr_level)
		{
			if (ptr_level->TellIfPlayerLost())
			{
				ptr_level->FinishLevel(defeat);
			}
		};

		//Cyclic action to manage GUI for main character
		std::function<void(Level*)> func_manage_gui_for_main_character = [](Level* ptr_level)
		{
			ptr_level->TellMyGui()->ManageForCreature(Creature::ptr_current_main_charater);
		};

		//Lambda to win level. (Useful for triggers)
		std::function<void()> ptr_func_win = [this]()
		{
			printf("TRIGGER!\n");
			this->Win();
		};
};

#endif //LEVEL_HPP
