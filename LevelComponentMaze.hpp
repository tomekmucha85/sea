#ifndef LEVEL_COMPONENT_MAZE_HPP
#define LEVEL_COMPONENT_MAZE_HPP
#include <stdlib.h>
#include <ctime>
#include <algorithm>
#include <string>
#include <LevelComponent.hpp>

class LevelComponentMaze : public LevelComponent
{
private:

	//Expressed in map blocks (1 map block == 1 entry in blueprint)
	int maze_block_width = 8;
	int maze_block_height = 8;
	int maze_rows_count = 0;
	int maze_columns_count = 0;
	//Expressed in maze blocks (1 maze block = (maze_block_width*maze_block_height) map blocks)
	int maze_blocks_count_vertical = 0;
	int maze_blocks_count_horizontal = 0;
	//Main array storing maze plan
	std::vector<std::vector<CreatureType>> blueprint;
	//Array storing maze block status (visited or not visited)
	int** visited_cells = nullptr;
	//Should maze have borders on specified sides?
	bool eastern_border = true;
	bool western_border = true;
	bool northern_border = true;
	bool southern_border = true;

public:
	LevelComponentMaze(std::map<LevelComponentType, std::vector<LevelComponent*>>* my_ptr_peer_level_components,
		SDL_Rect my_component_area = { 0,0,0,0 });
	int CalculateRowsNumber();
	int CalculateColumnsNumber();
	int CalculateBlocksCountVertically();
	int CalculateBlocksCountHorizontally();
	void ClearBlueprint();
	void PrepareMazeGrid();
	void PrintBlueprint();
	void GenerateMaze();
	void CreateEmptyVisitedCellsGrid();
	bool ValidateMazeArea(SDL_Rect maze_area);
	bool CheckIfNeighbourIsAvailable(Coordinates my_current_cell, Directions my_direction);
	void RemoveCellWall(Coordinates my_current_cell, Directions my_direction);
	void VivifyMaze();
	void CarveExitsFromMaze();
	void SetBorderState(Directions border_side, bool value);
	bool TellBorderState(Directions border_side);
	void ManageBorders();
	void ClearMaze();

	static SDL_Rect CalculateMazeDimensions(int w, int h, int my_maze_block_width = 8, int my_maze_block_height = 8);
};
#endif // LEVEL_COMPONENT_MAZE_HPP