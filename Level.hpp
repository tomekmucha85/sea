#ifndef LEVEL_HPP
#define LEVEL_HPP
#include <SDL.h>
#include <Creature.hpp>

class Level
{
    private:
		int map_block_width  = 40;
		int map_block_height = 40;
		//Expressed in map blocks
		int maze_block_height = 8;
		int maze_block_width = 8;
		int level_width = NULL;
		int level_height = NULL;
		int map_offset_x = NULL;
		int map_offset_y = NULL;
		//Main map array
		CreatureType** map = NULL;

    public:
		Level();
		~Level();
		void DetermineMapDimensions(float margin=0.5);
		void TestArray();
		void DrawMap();
		void PrintMap();
		static void RenderAllPresentCreatures();
		void CreateLevelGrid();
		void PrepareMazeGrid();
		CreatureType PickRandomObjectFromGiven(std::vector<CreatureType> my_creatures);
		SDL_Rect FindFreeTile();
		void InsertCreatureOntoMap(CreatureType my_type, SDL_Rect* ptr_my_position);
		void InsertStructureOntoMap(std::vector<std::vector<CreatureType>>, SDL_Rect* ptr_my_left_top_position);
		void GenerateRandomObjectOnMap();
		void SetLevelWidth(int width);
		void SetLevelHeight(int height);
		void CleanLevelGrid();
		bool CheckIfTileIsFree(int row, int column);
		void SetMapOffsetX(int columns_count, float margin);
		void SetMapOffsetY(int rows_count, float margin);
		int TellMapOffsetX();
		int TellMapOffsetY();
		void SetMazeBlockHeight(int height);
		void SetMazeBlockWidth(int width);
};

#endif //LEVEL_HPP
