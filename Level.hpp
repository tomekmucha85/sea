#ifndef LEVEL_HPP
#define LEVEL_HPP
#include <SDL.h>
#include <map>
#include <string>
#include <Creature.hpp>
#include <LevelComponent.hpp>
#include <Maze.hpp>

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
		std::map<LevelComponentType, std::vector<LevelComponent*>> level_components;
		//Expressed in pixels
		int map_block_width  = 40;
		int map_block_height = 40;
		//Expressed in map blocks
		int initial_level_width = NULL;
		int initial_level_height = NULL;
		int map_offset_x = NULL;
		int map_offset_y = NULL;
		//Main map array
		std::vector<std::vector<MapEntity>> map = {};

    public:
		Level();
		~Level();
		//void CreateMaze(SDL_Rect* ptr_maze_area, int maze_index);
		void DetermineMapDimensions(float margin=0.5);
		Creature* VivifyCreature(int map_row, int map_column, CreatureType my_type);
		void PrintMap();
		static void RenderAllPresentCreatures();
		void CreateLevelGrid();
		CreatureType PickRandomObjectFromGiven(std::vector<CreatureType> my_creatures);
		SDL_Rect FindFreeTile();
		Creature* InsertCreatureOntoMap(CreatureType my_type, SDL_Rect* ptr_my_position, bool force=false);
		bool RemoveCreatureFromMap(int map_column, int map_row);
		void InsertStructureOntoMap(std::vector<std::vector<CreatureType>>, SDL_Rect* ptr_my_left_top_position);
		void GenerateRandomObjectOnMap();
		int TellLevelWidth();
		int TellLevelHeight();
		void SetInitialLevelWidth(int width);
		void SetInitialLevelHeight(int height);
		void CleanLevelGrid();
		void InsertMapRowAtEnd();
		void InsertMapRowAtBeginning();
		void InsertMapColumnAtEnd();
		void InsertMapColumnAtBeginning();
		bool CheckIfTileIsFree(int row, int column);
		void SetMapOffsetX(int columns_count, float margin);
		void SetMapOffsetY(int rows_count, float margin);
		int TellMapOffsetX();
		int TellMapOffsetY();
		Creature* TellPointerToCreatureInSlot(int x, int y);
};

#endif //LEVEL_HPP
