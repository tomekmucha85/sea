#ifndef LEVEL_COMPONENT_HPP
#define LEVEL_COMPONENT_HPP
#include <map>
#include <SDL.h>
#include <vector>
#include <iterator>
#include <algorithm>
#include <functional>
#include <string>
#include <Creature.hpp>
#include <FactorySpawningCreatures.hpp>

//Types of possible level components
enum LevelComponentType { levco_maze, levco_powerups, levco_test, levco_core, levco_triggers };
//List of possible behaviours while inserting a Creature:
//    force - add creature and destroy colliding ones
//    merge - add creature and do not care if it collides with any other
//    safe - check if collision occurs. If yes - do not insert
enum InsertionMode {force,merge,safe};
enum Directions { north, east, south, west };

class LevelComponent
{
    private:
		//###################
		//Variables
		//###################
		std::vector<Creature*> creatures = {};
		std::map<LevelComponentType, std::vector<LevelComponent*>>* ptr_peer_level_components;
		//Invisible creature serving as "mask" of level component. Applied only to level components with defined area.
		Creature* ptr_component_outline = nullptr;
		//Contains actions associated to specific creature which will be performed during every game loop / every n game loops.
		std::vector<std::function<void(LevelComponent*)>> cyclic_actions = {};
		//###################
        //Functions
        //###################
		void AddLevelComponentOutline(SDL_Rect my_component_area);
    public:
		//###################
        //Variables
        //###################
		//#TODO brakuje inicjalizacji zmiennej poni�ej!
		LevelComponentType my_type;
		FactorySpawningCreatures* ptr_creatures_factory = nullptr;
		//Expressed in pixels
		static int map_block_width;
		static int map_block_height;

		//###################
		//Functions
		//###################
		LevelComponent(std::map<LevelComponentType, std::vector<LevelComponent*>>* my_ptr_peer_level_components, 
			SDL_Rect my_component_area = {0,0,0,0});
		~LevelComponent();
		std::vector<Creature*>* TellPtrToCreaturesArray();
		void SetPointerToPeerComponentsIndex(std::map<LevelComponentType, std::vector<LevelComponent*>>* my_ptr_peer_level_components);
		std::map<Creature*, LevelComponent*> FindCreatureNeighborsInAllLevelComponents(Creature* ptr_my_creature);
		Creature* AddCreature(CreatureType my_type, SDL_Rect* ptr_my_position, InsertionMode my_mode, std::string my_trigger_signal="");
		void RemoveCreature(Creature* ptr_my_creature);
		void RemoveAllCreatures();
		SDL_Rect TellComponentArea();
		SDL_Rect TellComponentEdge(Directions my_direction);
		std::vector<Creature*> FindCollisionsWithMainCharacter(bool check_only_obstacles = true);
		void MakeCreaturesPerformCyclicActions();
		void PerformCyclicActions();
		//###################
        //Virtual Functions
        //###################
		virtual void ClearMaze();
		virtual void GenerateMaze();
		virtual void SetBorderState(Directions border_side, bool value);
		virtual bool TellBorderState(Directions border_side);
		virtual std::vector<std::string> RunTriggersHitByHero();

		//###################
        // COMMON LAMBDAS
        //###################

        //Cyclic action to delete all dead creatures
		std::function<void(LevelComponent*)> func_reaper = [](LevelComponent* ptr_level_component)
		{
			for (Creature* ptr_my_creature : *(ptr_level_component->TellPtrToCreaturesArray()))
			{
				if (!ptr_my_creature->AmIAlive())
				{
					printf("Headshot!\n");
					ptr_level_component->RemoveCreature(ptr_my_creature);
				}
			}
		};

};

#endif LEVEL_COMPONENT_HPP