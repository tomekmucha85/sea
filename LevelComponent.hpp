#ifndef LEVEL_COMPONENT_HPP
#define LEVEL_COMPONENT_HPP
#include <map>
#include <SDL.h>
#include <iterator>
#include <algorithm>
#include <Creature.hpp>

//Types of possible level components
enum LevelComponentType { levco_maze, levco_powerups };
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
		std::vector<Creature*> creatures = {};
		std::map<LevelComponentType, std::vector<LevelComponent*>>* ptr_peer_level_components;

    public:
		//###################
		//Functions
		//###################
		std::vector<Creature*>* TellPtrToCreaturesArray();
		void SetPointerToPeerComponentsIndex(std::map<LevelComponentType, std::vector<LevelComponent*>>* my_ptr_peer_level_components);
		Creature* AddCreature(CreatureType my_type, SDL_Rect* ptr_my_position, InsertionMode my_mode);
		void RemoveCreature(Creature* ptr_my_creature);
};

#endif LEVEL_COMPONENT_HPP