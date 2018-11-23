#ifndef LEVEL_COMPONENT_HPP
#define LEVEL_COMPONENT_HPP
#include <map>
#include <SDL.h>
#include <vector>
#include <iterator>
#include <algorithm>
#include <Creature.hpp>
#include <FactorySpawningCreatures.hpp>

//Types of possible level components
enum LevelComponentType { levco_maze, levco_powerups, levco_test, levco_core };
//List of possible behaviours while inserting a Creature:
//    force - add creature and destroy colliding ones
//    merge - add creature and do not care if it collides with any other
//    safe - check if collision occurs. If yes - do not insert
enum InsertionMode {force,merge,safe};

class LevelComponent
{
    private:
		//###################
		//Variables
		//###################
		SDL_Rect component_area = {0,0,0,0};
		Creature* test = nullptr;
		std::vector<Creature*> creatures = {};
		//Level* ptr_owner = nullptr;
		std::map<LevelComponentType, std::vector<LevelComponent*>>* ptr_peer_level_components;

    public:
		//###################
        //Variables
        //###################
		FactorySpawningCreatures* ptr_creatures_factory = nullptr;


		//###################
		//Functions
		//###################
		LevelComponent(std::map<LevelComponentType, std::vector<LevelComponent*>>* my_ptr_peer_level_components);
		~LevelComponent();
		std::vector<Creature*>* TellPtrToCreaturesArray();
		void SetPointerToPeerComponentsIndex(std::map<LevelComponentType, std::vector<LevelComponent*>>* my_ptr_peer_level_components);
		std::vector<Creature*> FindCreatureNeighborsInAllLevelComponents(Creature* ptr_my_creature);
		Creature* AddCreature(CreatureType my_type, SDL_Rect* ptr_my_position, InsertionMode my_mode);
		void RemoveCreature(Creature* ptr_my_creature);
};

#endif LEVEL_COMPONENT_HPP