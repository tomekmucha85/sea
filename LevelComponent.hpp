#ifndef LEVEL_COMPONENT_HPP
#define LEVEL_COMPONENT_HPP
#include <map>
#include <SDL.h>
#include <vector>
#include <iterator>
#include <algorithm>
#include <functional>
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
		SDL_Rect component_area = {0,0,0,0};
		std::vector<Creature*> creatures = {};
		std::map<LevelComponentType, std::vector<LevelComponent*>>* ptr_peer_level_components;

    public:
		//###################
        //Variables
        //###################
		//#TODO brakuje inicjalizacji zmiennej poni¿ej!
		LevelComponentType my_type;
		FactorySpawningCreatures* ptr_creatures_factory = nullptr;
		//Expressed in pixels
		static int map_block_width;
		static int map_block_height;

		//###################
		//Functions
		//###################
		LevelComponent(std::map<LevelComponentType, std::vector<LevelComponent*>>* my_ptr_peer_level_components);
		~LevelComponent();
		std::vector<Creature*>* TellPtrToCreaturesArray();
		void SetPointerToPeerComponentsIndex(std::map<LevelComponentType, std::vector<LevelComponent*>>* my_ptr_peer_level_components);
		std::map<Creature*, LevelComponent*> FindCreatureNeighborsInAllLevelComponents(Creature* ptr_my_creature);
		Creature* AddCreature(CreatureType my_type, SDL_Rect* ptr_my_position, InsertionMode my_mode, std::string my_trigger_signal="");
		void RemoveCreature(Creature* ptr_my_creature);
		void RemoveAllCreatures();
		SDL_Rect TellComponentArea();
		SDL_Rect TellComponentEdge(Directions my_direction);
		//###################
        //Virtual Functions
        //###################
		virtual void ClearMaze();
		virtual void GenerateMaze();
		virtual void SetBorderState(Directions border_side, bool value);
};

#endif LEVEL_COMPONENT_HPP