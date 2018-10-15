#include <stdlib.h>
#include <ctime>
#include <Level.hpp>
#include <Creature.hpp>
#include <Screen.hpp>

//************
//CONSTRUCTORS
//************

Level::Level()
{
	Level::CreateLevelGrid();
	Level::CleanLevelGrid();
}

//************
//DESTRUCTOR
//************


Level::~Level()
{
	//#TODO Dopracowaæ!
	delete map;
}

//************
//TEST
//************


void Level::TestArray()
{
	Level::DetermineMapDimensions();

	//Declare
	int** grid = new int*[Level::level_height];
	for (int i = 0; i < Level::level_height; i++)
	{
		grid[i] = new int[Level::level_width];
	}
	//Populate
	int value = 0;

	for (int i = 0; i < Level::level_height; i++)
	{
		for (int j = 0; j < Level::level_width; j++)
		{
			grid[i][j] = value;
			value++;
		}
	}
	//Print
	printf("\n\n========Will print test array========\n\n");
	for (int i = 0; i < Level::level_height; i++)
	{
		printf("\n");
		for (int j = 0; j < Level::level_width; j++)
		{
			printf("[%d]",grid[i][j]);
		}
	}
	printf("\n\n========Printed test array========\n\n");


}

//************
//POPULATING MAP
//************


void Level::PrintMap()
{
	if (map == NULL)
	{
		throw std::invalid_argument("Trying to print a map which was not initialized yet!\n");
	}
	printf("\n\n========Will print map========\n\n");
	for (int i = 0; i < level_height; i++)
	{
		printf("\n");
		for (int j = 0; j < level_width; j++)
		{
			if (map[i][j] != cre_none)
			{
				CreatureType my_creature = map[i][j];
				printf("[%d]", my_creature);
			}
			else
			{
				printf("[ ]");
			}
		}
	}
	printf("\n\n========Printed map========\n\n");
}

void Level::InsertCreatureOntoMap(CreatureType my_type, SDL_Rect* ptr_my_position)
{
	if (map == NULL)
	{
		throw std::invalid_argument("Trying to insert a creature onto map which was not initialized yet!");
	};
	int x_tile = ptr_my_position->x;
	int y_tile = ptr_my_position->y;
	if (Level::CheckIfTileIsFree(y_tile, x_tile))
	{
		printf("Inserting a creature %d onto map. X: %d Y:%d.\n", my_type, x_tile, y_tile);
		map[y_tile][x_tile] = my_type;
	}
	else
	{
		printf("Could not place creature %d in tile y: %d x: %d. Tile is not free!\n", my_type, y_tile, x_tile);
	}
}

void Level::InsertStructureOntoMap(std::vector<std::vector<CreatureType>> my_structure, SDL_Rect* ptr_my_left_top_position)
{
//#TODO brakuje walidacji, czy taki obszar jest zaindeksowany na mapie
	int start_map_tile_x = ptr_my_left_top_position->x;
	int start_map_tile_y = ptr_my_left_top_position->y;
	int current_map_tile_y = start_map_tile_y;
	for (int i = 0; i < my_structure.size(); i++)
	{
		int current_map_tile_x = start_map_tile_x;
		for (int j = 0; j < my_structure[i].size(); j++)
		{
	        //#TODO nie sprawdza, czy miejsce by³o wolne!
			//printf("Inserting creature %d onto map. X: %d, Y; %d.\n", my_structure[i][j], current_map_tile_x, current_map_tile_y);
			map[current_map_tile_y][current_map_tile_x] = my_structure[i][j];
			current_map_tile_x++;
		}
		current_map_tile_y++;
	}
}

void Level::DrawMap()
{
	if (map == NULL)
	{
		throw std::invalid_argument("Trying to draw a map which was not initialized yet!");
	}
	for (int i = 0; i < level_height; i++)
	{
		for (int j = 0; j < level_width; j++)
		{
			if (map[i][j] != cre_none)
			{
				CreatureType my_creature = map[i][j];
				int position_x = (j - TellMapOffsetX()) * map_block_width;
				int position_y = (i - TellMapOffsetY()) * map_block_height;
				printf("Drawing a map. Coords: x: %d, y: %d, creature type: %d\n", position_x, position_y, my_creature);
				SDL_Rect my_position = {position_x, position_y, 0, 0};
				SDL_Rect* ptr_my_position = &my_position;
				Creature::SpawnCreature(my_creature, ptr_my_position);
			}
			else
			{
				//Nothing to draw
				;
			}
		}
	}
}

CreatureType Level::PickRandomObjectFromGiven(std::vector<CreatureType> my_creatures)
{
	int vector_size = my_creatures.size();
	srand(time(NULL));
	CreatureType chosen_creature = my_creatures[rand() % vector_size];
	printf("Picked random creature: %d.\n", chosen_creature);
	return chosen_creature;
}

SDL_Rect Level::FindFreeTile()
{

	bool check = false;
	int x_tile = NULL;
	int y_tile = NULL;
	srand(time(NULL));
	while (check != true)
	{
		x_tile = rand() % (level_width - 1);
		y_tile = rand() % (level_height - 1);
		printf("Chosen tile: x - %d y - %d\n", x_tile, y_tile);
		check = Level::CheckIfTileIsFree(y_tile, x_tile);
		printf("Check result: %d\n", check);
	}
	SDL_Rect result = {x_tile, y_tile, 0, 0};
	return result;
}


void Level::GenerateRandomObjectOnMap()
{
	SDL_Rect free_tile = Level::FindFreeTile();
	//#TODO Now selecting only from walls
	CreatureType my_creature = Level::PickRandomObjectFromGiven(Creature::walls);
	map[free_tile.y][free_tile.x] = my_creature;
	printf("Random object of type %d was created on a map. Tile: x: %d, x: %d\n", my_creature, free_tile.x, free_tile.y);
}

bool Level::CheckIfTileIsFree(int row, int column)
{
	if (map[row][column] == cre_none)
	{
		return true;
	}
	else
	{
		return false;
	}
}


void Level::SetLevelWidth(int width)
{
	level_width = width;
}

void Level::SetLevelHeight(int height)
{
	level_height = height;
}

void Level::SetMapOffsetX(int columns_count, float margin)
{
	map_offset_x = int(columns_count*margin);
}

void Level::SetMapOffsetY(int rows_count, float margin)
{
	map_offset_y = int(rows_count*margin);
}

int Level::TellMapOffsetX()
{
	return map_offset_x;
}
int Level::TellMapOffsetY()
{
	return map_offset_y;
}

void Level::DetermineMapDimensions(float margin)
{
	int screen_cols_count = Screen::TellScreenWidth() / map_block_width;
	int screen_rows_count = Screen::TellScreenHeight() / map_block_height;
	SetMapOffsetX(screen_cols_count, margin);
	SetMapOffsetY(screen_rows_count, margin);
	//float fl_rows_count_with_margin = screen_rows_count + (screen_rows_count * margin);
	//float fl_cols_count_with_margin = screen_cols_count + (screen_cols_count * margin);
	int rows_count = screen_rows_count + 2 * (TellMapOffsetY());
	int cols_count = screen_cols_count + 2 * (TellMapOffsetX());
	printf("screen_rows_count: %d screen_cols_count: %d \n", screen_rows_count, screen_cols_count);
	printf("rows_count: %d cols_count: %d \n", rows_count, cols_count);
	Level::SetLevelHeight(rows_count);
	Level::SetLevelWidth(cols_count);
}

void Level::CreateLevelGrid()
{
	Level::DetermineMapDimensions();
	//Creating a 2D dynamically sized array
	CreatureType** level_grid = new CreatureType*[Level::level_height];
	for (int i = 0; i < Level::level_height; i++)
	{
		level_grid[i] = new CreatureType[Level::level_width];
	}

	map = level_grid;
}


void Level::CleanLevelGrid()
{
	if (map == NULL)
	{
		throw std::invalid_argument("Trying to clean map which was not initialized yet!");
	}
	for (int i = 0; i < level_height; i++)
	{
		for (int j = 0; j < level_width; j++)
		{
			map[i][j] = cre_none;
		}
	}
	printf("Level with given dimensions: %d by %d was cleared!\n", level_width, level_height);
}


void Level::SetMazeBlockHeight(int height)
{
	maze_block_height = height;
}

void Level::SetMazeBlockWidth(int width)
{
	maze_block_width = width;
}

void Level::PrepareMazeGrid()
{
	if (map == NULL)
	{
		throw std::invalid_argument("Trying to prepare maze grid on a map which was not initialized yet!");
	}
	for (int i = 0; i < level_height; i++)
	{
		for (int j = 0; j < level_width; j++)
		{
			if (j%(maze_block_width-1) == 0)
			{
				map[i][j] = cre_flying_box;
			}
			if (i%(maze_block_height-1) == 0)
			{
				map[i][j] = cre_flying_box;
			}
		}
	}
}
/*
//EXAMPLE
int main() {
// dimensions
int N = 3;
int M = 3;

// dynamic allocation
int** ary = new int*[N];
for(int i = 0; i < N; ++i)
ary[i] = new int[M];

// fill
for(int i = 0; i < N; ++i)
for(int j = 0; j < M; ++j)
ary[i][j] = i;

// print
for(int i = 0; i < N; ++i)
for(int j = 0; j < M; ++j)
std::cout << ary[i][j] << "\n";

// free
for(int i = 0; i < N; ++i)
delete [] ary[i];
delete [] ary;

return 0;
}

*/


//************
//RENDERING
//************

void Level::RenderAllPresentCreatures()
{
	for (Creature* ptr_creature : Creature::class_instances)
	{
		ptr_creature->ptr_creature_sprite->Render();
	}

}
