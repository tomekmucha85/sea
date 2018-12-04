#ifndef LEVEL_NINE_MAZES_HPP
#define LEVEL_NINE_MAZES_HPP
#include <Level.hpp>
class LevelNineMazes : public Level
{
    private:
		int maze_rows_count = 0;
		int maze_columns_count = 0;

		SDL_Rect** mazes_areas[9];

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
		SDL_Rect Maze_1 = {};
		SDL_Rect Maze_2 = {};
		SDL_Rect Maze_3 = {};
		SDL_Rect Maze_4 = {};
		SDL_Rect Maze_5 = {};
		SDL_Rect Maze_6 = {};
		SDL_Rect Maze_7 = {};
		SDL_Rect Maze_8 = {};
		SDL_Rect Maze_9 = {};

    public:
	    LevelNineMazes(int my_cols_count, int my_rows_count);
		void SetMazeRowsCount(int rows_num);
		void SetMazeColsCount(int cols_num);
};
#endif // LEVEL_NINE_MAZES_HPP
