#include<FactorySpawningLevels.hpp>

FactorySpawningLevels::FactorySpawningLevels()
{
	// #TODO - czy potrzebny konstruktor?
	printf("Levels factory created.\n");
}

Level* FactorySpawningLevels::SpawnLevel(LevelType my_type)
{
	Level* result = nullptr;

	if (my_type == level_ninemazes)
	{
		result = new LevelNineMazes(maze_columns_count, maze_rows_count);
	}
	else if (my_type == level_base)
	{
		result = new Level();
	}
	else
	{
		throw std::invalid_argument("Invalid level type passed to factory!\n");
	}
	return result;
}

void FactorySpawningLevels::SetMazeSize(int my_maze_columns_count, int my_maze_rows_count)
{
	maze_columns_count = my_maze_columns_count;
	maze_rows_count = my_maze_rows_count;
}