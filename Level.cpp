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
	//CreateLevelGrid();
	CreateComponentsFactory();
	//Level::CleanLevelGrid();

}

//************
//DESTRUCTOR
//************


Level::~Level()
{
	//#TODO Dopracowaæ!
	delete ptr_components_factory;
}

//*****************
//ADDING COMPONENTS
//*****************

//Every component holds its own creatures
//Level traces holds record of its components

std::map<LevelComponentType, std::vector<LevelComponent*>>* Level::TellPointerToComponentsArray()
{
	std::map<LevelComponentType, std::vector<LevelComponent*>>* ptr_level_components = &level_components;
	return ptr_level_components;
}

FactorySpawningLevelComponents* Level::CreateComponentsFactory()
{
	ptr_components_factory = new FactorySpawningLevelComponents(TellPointerToComponentsArray());
	return ptr_components_factory;
}


//************
//POPULATING MAP
//************

void Level::CreateLevelGrid()
{
	//Level::DetermineMapDimensions();
	MapEntity blank_entity = {cre_none, NULL};
	std::vector<std::vector<MapEntity>> level_grid(initial_level_height,std::vector<MapEntity>(initial_level_width, blank_entity));
	map = level_grid;
}

void Level::InsertMapRowAtEnd()
{
	MapEntity blank_entity = { cre_none, NULL };
	map.push_back(std::vector<MapEntity>(TellLevelWidth(), blank_entity));
	printf("Row inserted at map end.\n");
}

void Level::InsertMapRowAtBeginning()
{
	MapEntity blank_entity = { cre_none, NULL };
	map.insert(map.begin(), std::vector<MapEntity>(TellLevelWidth(), blank_entity));
	printf("Row inserted at map beginning.\n");
}

void Level::InsertMapColumnAtEnd()
{
	MapEntity blank_entity = { cre_none, NULL };
	for (int row = 0; row < TellLevelHeight(); row++)
	{
		map[row].push_back(blank_entity);
	}
	printf("Column inserted at map end.\n");
}

void Level::InsertMapColumnAtBeginning()
{
	MapEntity blank_entity = { cre_none, NULL };
	for (int row = 0; row < TellLevelHeight(); row++)
	{
		map[row].insert(map[row].begin(), blank_entity);
	}
	printf("Column inserted at map beginning.\n");
}

void Level::PrintMap()
{
	//if (map == NULL)
	//{
	//	throw std::invalid_argument("Trying to print a map which was not initialized yet!\n");
	//}
	printf("\n\n========Will print map========\n\n");
	for (int i = 0; i < TellLevelHeight(); i++)
	{
		printf("\n");
		for (int j = 0; j < TellLevelWidth(); j++)
		{
			if (map[i][j].type != cre_none)
			{
				MapEntity my_creature = map[i][j];
				printf("[%d]", my_creature.type);
			}
			else
			{
				printf("[ ]");
			}
		}
	}
	printf("\n\n========Printed map========\n\n");
}

Creature* Level::InsertCreatureOntoMap(CreatureType my_type, SDL_Rect* ptr_my_position, bool force)
{
	//if (map == NULL)
	//{
	//	throw std::invalid_argument("Trying to insert a creature onto map which was not initialized yet!");
	//};
	int x_tile = ptr_my_position->x;
	int y_tile = ptr_my_position->y;
	if (Level::CheckIfTileIsFree(y_tile, x_tile) || force == true)
	{
		printf("Inserting a creature %d onto map. X: %d Y:%d.\n", my_type, x_tile, y_tile);
		map[y_tile][x_tile].type = my_type;
		//return VivifyCreature(x_tile, y_tile, my_type);
	}
	else
	{
		printf("Could not place creature %d in tile y: %d x: %d. Tile is not free!\n", my_type, y_tile, x_tile);
		return nullptr;
	}
}

bool Level::RemoveCreatureFromMap(int map_column, int map_row)
{
    // #TODO Dopracowaæ walidacjê i zwracan¹ wartoœæ!
	printf("Trying to remove %d %d.\n", map_column, map_row);
	if (map[map_row][map_column].ptr_creature != NULL)
	{
		map[map_row][map_column].ptr_creature->~Creature();
		map[map_row][map_column].type = cre_none;
		return true;
	}
	else
	{
		return false;
	}
}

Creature* Level::TellPointerToCreatureInSlot(int x, int y)
{
	Creature* result = map[y][x].ptr_creature;
	return result;
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
		x_tile = rand() % (TellLevelWidth() - 1);
		y_tile = rand() % (TellLevelHeight() - 1);
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
	map[free_tile.y][free_tile.x].type = my_creature;
	printf("Random object of type %d was created on a map. Tile: x: %d, x: %d\n", my_creature, free_tile.x, free_tile.y);
}

bool Level::CheckIfTileIsFree(int row, int column)
{
	//printf("Checking for row: %d, column: %d.\n", row, column);
	if (row >= TellLevelHeight() || column >= TellLevelWidth())
	{
		throw std::invalid_argument("Given coordinates do not exist on the map!\n");
	}
	if (map[row][column].type == cre_none)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int Level::TellLevelWidth()
{
	return map[0].size();
}

int Level::TellLevelHeight()
{
	return map.size();
}

void Level::SetInitialLevelWidth(int width)
{
	initial_level_width = width;
}

void Level::SetInitialLevelHeight(int height)
{
	initial_level_height = height;
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

/*void Level::DetermineMapDimensions(float margin)
{
	int screen_cols_count = Screen::TellScreenWidth() / map_block_width;
	int screen_rows_count = Screen::TellScreenHeight() / map_block_height;
	SetMapOffsetX(screen_cols_count, margin);
	SetMapOffsetY(screen_rows_count, margin);
	int rows_count = screen_rows_count + 2 * (TellMapOffsetY());
	int cols_count = screen_cols_count + 2 * (TellMapOffsetX());
	printf("screen_rows_count: %d screen_cols_count: %d \n", screen_rows_count, screen_cols_count);
	printf("rows_count: %d cols_count: %d \n", rows_count, cols_count);
	Level::SetInitialLevelHeight(rows_count);
	Level::SetInitialLevelWidth(cols_count);
}*/

void Level::CleanLevelGrid()
{
	//if (map == NULL)
	//{
	//	throw std::invalid_argument("Trying to clean map which was not initialized yet!");
	//}
	for (int row = 0; row < TellLevelHeight(); row++)
	{
		for (int column = 0; column < TellLevelWidth(); column++)
		{
			map[row][column].type = cre_none;
			map[row][column].ptr_creature = NULL;
		}
	}
	printf("Level with given dimensions: %d by %d was cleared!\n", TellLevelWidth(), TellLevelHeight());
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
	for (std::pair<LevelComponentType, std::vector<LevelComponent*>> element : level_components)
	{
		std::vector<LevelComponent*> my_level_components = element.second;
		for (LevelComponent* ptr_my_level_component : my_level_components)
		{
			for (Creature* ptr_member_creature : *ptr_my_level_component->TellPtrToCreaturesArray())
			{
				ptr_member_creature->ptr_creature_sprite->Render();
			}
		}
	}
}
