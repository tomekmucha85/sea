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
	const int maze_block_width = 8;
	const int maze_block_height = 8;
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

	//Patterns of maze elements
	/*
	    __
	   |
	
	*/
	std::vector<std::vector<CreatureType>> pattern_upper_corner_left =
	{
		{cre_none, cre_none,       cre_none},
	    {cre_none, cre_flying_box, cre_flying_box},
	    {cre_none, cre_flying_box, cre_none}
	};

	/*
	  __
        |

   */
	std::vector<std::vector<CreatureType>> pattern_upper_corner_right =
	{
		{cre_none,       cre_none,       cre_none},
		{cre_flying_box, cre_flying_box, cre_none},
		{cre_none,       cre_flying_box, cre_none}
	};

	/*

	|__

   */
	std::vector<std::vector<CreatureType>> pattern_bottom_corner_left =
	{
		{cre_none,       cre_flying_box,       cre_none},
		{cre_none,       cre_flying_box,       cre_flying_box},
		{cre_none,       cre_none,             cre_none}
	};


	/*

	__|

   */
	std::vector<std::vector<CreatureType>> pattern_bottom_corner_right =
	{
		{cre_none,       cre_flying_box,       cre_none},
		{cre_flying_box, cre_flying_box,       cre_none},
		{cre_none,       cre_none,             cre_none}
	};

	/*
	|
	|
	|
   */
	std::vector<std::vector<CreatureType>> pattern_bottom_vertical =
	{
		{cre_none,       cre_flying_box,       cre_none},
		{cre_none,       cre_flying_box,       cre_none},
		{cre_none,       cre_flying_box,       cre_none}
	};

public:
	LevelComponentMaze(std::map<LevelComponentType, std::vector<LevelComponent*>>* my_ptr_peer_level_components,
		PreciseRect my_component_area = { 0,0,0,0 });
	~LevelComponentMaze();
	int CalculateRowsNumber();
	int CalculateColumnsNumber();
	int CalculateBlocksCountVertically();
	int CalculateBlocksCountHorizontally();
	void ClearBlueprint();
	void PrepareMazeGrid();
	void PrepareNavigationGrid();
	void PrintBlueprint();
	void GenerateMaze();
	void CreateEmptyVisitedCellsGrid();
	bool ValidateMazeArea(PreciseRect maze_area);
	bool CheckIfNeighbourIsAvailable(Coordinates my_current_cell, Directions my_direction);
	void RemoveCellWall(Coordinates my_current_cell, Directions my_direction);
	void SetAppropriateSpriteClips(std::vector<std::vector<CreatureType>>* ptr_my_blueprint, int column, int row, Creature* ptr_creature);
	//#TODO - przerobiæ na tablicê
	std::vector<std::vector<CreatureType>> GetBlueptrintElementContextInGivenRadius(
		std::vector<std::vector<CreatureType>>* ptr_my_blueprint, 
		int checked_element_column, 
		int checked_element_row, 
		int radius);
	void VivifyMaze();
	void CarveExitsFromMaze();
	void SetBorderState(Directions border_side, bool value);
	bool TellBorderState(Directions border_side);
	void ManageBorders();
	void ClearMaze();

	static PreciseRect CalculateMazeDimensions(int w, int h, int my_maze_block_width = 8, int my_maze_block_height = 8);
};
#endif // LEVEL_COMPONENT_MAZE_HPP