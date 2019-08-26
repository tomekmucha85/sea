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
		//Variables & const
		//###################
		LevelType type = level_base;
		bool has_player_lost = false;
		bool has_player_won = false;
		LevelEnding ending = ending_none;
		bool is_level_finished = false;

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
		//Pointer to GUI object
		GUI* ptr_gui = nullptr;
		//Contains actions associated to specific level which will be performed during every game loop.
		std::vector<std::function<void(Level*)>> cyclic_actions = {};
		//Contains assignment of triggers to certain events
		std::map<std::string, std::function<void()>> signals_vs_events = {};

		//#TODO - zabezpieczyæ domyœlne komponenty przed zniszczeniem

		//Core level component - always created
		LevelComponent* ptr_initial_core_component = nullptr;
		//Initial level component for triggers - always created
		LevelComponent* ptr_initial_triggers_component = nullptr;
		//Initial level component for navigation grid - always created
		LevelComponent* ptr_initial_navgrid_component = nullptr;

		//###########################
        //#VARIABLES FOR OPTIONAL USE 
		//###########################

		//Timer deciding if level is won
		TimerStartStop* ptr_winning_timer = nullptr;
		//How much time has to pass before a level is won via a winning timer
		Uint32 time_to_endure_before_win = 60000;

		//#TODO - czy nie usun¹æ?
		bool should_nodes_be_reconnected = false;

		Level();
		virtual ~Level();
		//Level components factory
		virtual Creature* SpawnHero(CreatureType hero_type = cre_clawy, Coordinates* ptr_hero_position = nullptr,
			bool force_respawn = true);
		LevelType TellMyType();
		void SetMyType(LevelType my_type);
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
		void Win();
		void Loose();
		void SetLevelEnding(LevelEnding my_ending);
		LevelEnding TellLevelEnding();
		bool TellIfPlayerWon();
		bool TellIfPlayerLost();
		virtual void FinishLevel(LevelEnding my_ending);
		bool TellIfLevelIsFinished();
		void Pause();
		void UnPause();
		bool TellIfPaused();

		void ProcessAllPathRequests();

		//VIRTUAL METHODS FOR SPECIFIC LEVELS

		//Methods for LevelMenu
		virtual void BrowseActions(Directions my_direction);
		virtual bool PerformSelectedAction();
		virtual bool NotifyOfBciEvent(BCIEvent my_event);

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
				ptr_level->FinishLevel(ending_victory);
			}
		};

		//Cyclic action to check if losing conditions were met
		std::function<void(Level*)> func_check_and_react_if_player_lost = [](Level* ptr_level)
		{
			if (ptr_level->TellIfPlayerLost())
			{
				ptr_level->FinishLevel(ending_defeat);
			}
		};

		//Cyclic action to manage GUI for main character and current level
		std::function<void(Level*)> func_manage_gui = [](Level* ptr_level)
		{
			ptr_level->TellMyGui()->ManageForCreature(Creature::ptr_current_main_charater);
			// If a winning timer exists for this level
			if (ptr_level->ptr_winning_timer != nullptr)
			{
				Uint32 time_passed_on_winning_timer = ptr_level->ptr_winning_timer->HowManyMilisecondsPassedFromStart();
				ptr_level->TellMyGui()->ManageWinningTimer(time_passed_on_winning_timer);
			}
		};

		//Lambda to win level. (Useful for triggers)
		std::function<void()> ptr_func_win = [this]()
		{
			printf("TRIGGER!\n");
			this->Win();
		};

		//Lambda to lose level. (Useful for triggers)
		std::function<void()> ptr_func_lose = [this]()
		{
			printf("TRIGGER!\n");
			this->Loose();
		};

		//Lambda to connect navigation nodes.
		/*std::function<void(Level*)> func_connect_nodes = [](Level* ptr_level)
		{
			if (ptr_level->should_nodes_be_reconnected == true)
			{
				printf("WILL CONNECT NODES!\n");
				dynamic_cast<LevelComponentNavGrid*>(ptr_level->ptr_initial_navgrid_component)->ConnectNodes(800);
				ptr_level->should_nodes_be_reconnected = false;
			}

		};*/

		//Lambda to process path requests.
		std::function<void(Level*)> func_serve_path_requests = [](Level* ptr_level)
		{
			ptr_level->ProcessAllPathRequests();
		};

		//Lambda to check timer allowing to win a level
		std::function<void(Level*)> func_check_winning_timer = [](Level* ptr_level)
		{
			if (ptr_level->ptr_winning_timer != nullptr)
			{
				Uint32 time_passed = ptr_level->ptr_winning_timer->Read();
				if (time_passed >= ptr_level->time_to_endure_before_win)
				{
					printf("Level won, cause timer was hit!\n");
					ptr_level->Win();
				}
			}
			else
			{
				Logger::Log("Cannot check winning timer, because it was not initialized.\n");
			}

		};

		//Lambda to check if main character attacked someone since last check.
		//In such case winning timer is restarted.
		std::function<void(Level*)> func_restart_winning_timer_if_main_character_attacks_anyone = [](Level* ptr_level)
		{
			if (Creature::ptr_current_main_charater->TellIfCreaturePerformedAttack())
			{
				if (ptr_level->ptr_winning_timer != nullptr)
				{
					ptr_level->ptr_winning_timer->RestartFromZero();
					Creature::ptr_current_main_charater->ResetAttackFlag();
				}
				else
				{
					Logger::Log("Cannot reset winning timer, because it was not initialized.\n");
				}
			}
		};

};

#endif //LEVEL_HPP
