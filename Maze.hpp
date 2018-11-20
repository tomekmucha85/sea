#ifndef MAZE_HPP
#define MAZE_HPP
#include <SDL.h>
#include <Creature.hpp>
#include <LevelComponent.hpp>
enum Directions { north, east, south, west };

class Maze : public LevelComponent
{
    private:
		struct Coordinates
		{
			int x = 0;
			int y = 0;
		};
		//Pointer to a level, where maze is constructed
		//Level* ptr_level = nullptr;
	    //Expressed in maze blocks
	    int maze_width = NULL;
		int maze_height = NULL;
		//Expressed in map blocks
		int maze_start_column = 0;
		int maze_finish_column = 0;
		int maze_start_row = 0;
		int maze_finish_row = 0;
		//Expressed in map blocks
		int maze_block_height = 8;
		int maze_block_width = 8;
		int** visited_cells = NULL;
		//Vector containing pointers to maze walls
		std::vector<Creature*> maze_walls;

    public:
		//Maze(Level* ptr_my_level);
		//Maze(Level* ptr_my_level, SDL_Rect* ptr_map_piece_to_populate);
		Maze(std::map<std::string, std::vector<LevelComponent*>>* my_peer_components,
			SDL_Rect* ptr_my_area, int my_index);
		~Maze();
		void SetMazeRegionToBeWholeMap();
		void CalculateMazeDimensions();
		void AddCreatureToMazeWalls(Creature* ptr_my_creature);
		void RemoveCreatureFromMazeWalls(Creature* ptr_my_creature);
		void PrepareMazeGrid();
		void GenerateMaze();
		void EnlargeMapDimensionsToFitMazeTiles();
		void SetMazeDimensions(int my_maze_width, int my_maze_height);
		void SetMazeBlockHeight(int height);
		void SetMazeBlockWidth(int width);
		bool CheckIfNeighbourIsAvailable(Coordinates my_current_cell, Directions my_direction);
		void CarveWall(SDL_Rect* ptr_position_of_block_to_carve);
		void RemoveCellWall(Coordinates my_current_cell, Directions my_direction);
		void CreateEmptyVisitedCellsGrid();
		void ClearMaze();
};


#endif //MAZE_HPP