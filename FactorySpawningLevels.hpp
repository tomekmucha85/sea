#ifndef FACTORY_SPAWNING_LEVELS_HPP
#define FACTORY_SPAWNING_LEVELS_HPP
#include <SDL.h>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <Level.hpp>
#include <LevelNineMazes.hpp>

class FactorySpawningLevels
{
private:
	int maze_rows_count = 4;
	int maze_columns_count = 4;

public:
	FactorySpawningLevels();
	void SetMazeSize(int my_maze_columns_count, int my_maze_rows_count);
	Level* SpawnLevel(LevelType);
};

#endif //FACTORY_SPAWNING_LEVELS_HPP