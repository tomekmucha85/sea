#ifndef LEVEL_NINE_MAZES_HPP
#define LEVEL_NINE_MAZES_HPP
#include <Level.hpp>
class LevelNineMazes : public Level
{
    private:
		int maze_rows_count = 0;
		int maze_columns_count = 0;

		SDL_Rect** mazes_areas[9];

		LevelComponent* ptr_current_central_maze = nullptr;

		/*
		MAZES DISTRIBUTION:
		_____________
		| 1 | 2 | 3 |
		-------------
		| 4 | 5 | 6 |
		-------------
		| 7 | 8 | 9 |
		-------------
		
		*/
		SDL_Rect maze_1_area = {};
		SDL_Rect maze_2_area = {};
		SDL_Rect maze_3_area = {};
		SDL_Rect maze_4_area = {};
		SDL_Rect maze_5_area = {};
		SDL_Rect maze_6_area = {};
		SDL_Rect maze_7_area = {};
		SDL_Rect maze_8_area = {};
		SDL_Rect maze_9_area = {};


		LevelComponent* ptr_maze1 = nullptr;
		LevelComponent* ptr_maze2 = nullptr;
		LevelComponent* ptr_maze3 = nullptr;
		LevelComponent* ptr_maze4 = nullptr;
		LevelComponent* ptr_maze5 = nullptr;
		LevelComponent* ptr_maze6 = nullptr;
		LevelComponent* ptr_maze7 = nullptr;
		LevelComponent* ptr_maze8 = nullptr;
		LevelComponent* ptr_maze9 = nullptr;

    public:
	    LevelNineMazes(int my_cols_count, int my_rows_count);
		void SetMazeRowsCount(int rows_num);
		void SetMazeColsCount(int cols_num);
		SDL_Rect CalculateInitialCentralMazeArea();
		LevelComponent* SpawnInitialCentralMaze();
		void DeleteMazeNumber(int my_number);
		void GenerateMazeNumber(int my_number);
		void SetCurrentCentralMaze( LevelComponent* ptr_my_maze);
		LevelComponent* TellCurrentCentralMaze();
};
#endif // LEVEL_NINE_MAZES_HPP
