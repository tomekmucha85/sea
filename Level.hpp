#ifndef LEVEL_HPP
#define LEVEL_HPP
#include <SDL.h>
#include <Creature.hpp>

class Level
{
    private:
		int map_block_width  = 40;
		int map_block_height = 40;
		int level_width = NULL;
		int level_height = NULL;
		float margin = 0.5;
		CreatureType** map = NULL;

    public:
		Level();
		~Level();
		void DrawMap();
		void PrintMap();
		static void RenderAllPresentCreatures();
		void CreateLevelGrid();
		CreatureType PickRandomObjectFromGiven(std::vector<CreatureType> my_creatures);
		SDL_Rect FindFreeTile();
		void InsertCreatureOntoMap(CreatureType my_type, SDL_Rect* ptr_my_position);
		void InsertStructureOntoMap(std::vector<std::vector<CreatureType>>, int my_struct_width, int my_struct_height, SDL_Rect* ptr_my_left_top_position);
		void GenerateRandomObjectOnMap();
		void SetLevelWidth(int width);
		void SetLevelHeight(int height);
		void CleanLevelGrid();
		bool CheckIfTileIsFree(int row, int column);
};

#endif //LEVEL_HPP
