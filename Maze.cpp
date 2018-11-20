#include <stdlib.h>
#include <ctime>
#include <algorithm>
#include <Maze.hpp>
#include <Level.hpp>
#include <Creature.hpp>

//************
//CONSTRUCTORS
//************

Maze::Maze(std::map<std::string, std::vector<LevelComponent*>>* my_peer_components,
	SDL_Rect* ptr_my_area, int my_index)
{
	//First argument contains pointers to all level components.
	;
}

/*Maze::Maze(Level* ptr_my_level)
{
	ptr_level = ptr_my_level;
	printf("Entered maze constructor.\n");
	SetMazeRegionToBeWholeMap();
	EnlargeMapDimensionsToFitMazeTiles();
	CalculateMazeDimensions();
	GenerateMaze();
}*/

void Maze::SetMazeRegionToBeWholeMap()
{
	maze_start_column = 0;
	maze_start_row = 0;
	maze_finish_column = ptr_level->TellLevelWidth() - 1;
	maze_finish_row = ptr_level->TellLevelHeight() - 1;
}

Maze::Maze(Level* ptr_my_level, SDL_Rect* ptr_map_piece_to_populate)
{
	ptr_level = ptr_my_level;
	printf("Entered maze piece-specific constructor.\n");
	maze_start_row = ptr_map_piece_to_populate->y;
	maze_start_column = ptr_map_piece_to_populate->x;
	maze_finish_row = ptr_map_piece_to_populate->y + ptr_map_piece_to_populate->h - 1;
	maze_finish_column = ptr_map_piece_to_populate->x + ptr_map_piece_to_populate->w - 1;
	// #TODO dodaæ walidacjê danych i wyj¹tki (np dla wartoœci ujemnych)
	while ((maze_finish_row - maze_start_row) % (maze_block_height - 1) != 0)
	{
	    maze_finish_row++;
		printf("Maze needs to be taller to fit grid, added a row.\n");
    }
	while ((maze_finish_column - maze_start_column) % (maze_block_width - 1) != 0)
	{
		maze_finish_column++;
		printf("Maze needs to be wider to fit grid, added a column.\n");
	}
	while (ptr_level->TellLevelWidth() < maze_finish_column+1)
	{
		ptr_level->InsertMapColumnAtEnd();
	}
	while (ptr_level->TellLevelHeight() < maze_finish_row + 1)
	{
		ptr_level->InsertMapRowAtEnd();
	}
	CalculateMazeDimensions();
	GenerateMaze();
}

//************
//DESTRUCTOR
//************


Maze::~Maze()
{
	//#TODO Dopracowaæ!
}

//************
//MAZE BUILDING
//************

void Maze::EnlargeMapDimensionsToFitMazeTiles()
{
	if (ptr_level == NULL)
	{
		throw std::invalid_argument("Trying to enlarge map to fit maze, but no level pointer was passed!");
	}
	while ((ptr_level->TellLevelHeight() - maze_block_height) % (maze_block_height - 1) != 0)
	{
		ptr_level->InsertMapRowAtEnd();
		printf("Added a row to fit maze tiles.\n");
	}
	while ((ptr_level->TellLevelWidth() - maze_block_width) % (maze_block_width - 1) != 0)
	{
		ptr_level->InsertMapColumnAtEnd();
		printf("Added a column to fit maze tiles.\n");
	}

}

void Maze::CalculateMazeDimensions()
{
	int my_maze_width = (((maze_finish_column - maze_start_column + 1) - maze_block_width) / (maze_block_width - 1)) + 1;
	int my_maze_height = (((maze_finish_row - maze_start_row + 1) - maze_block_height) / (maze_block_height - 1)) + 1;
	Maze::SetMazeDimensions(my_maze_width, my_maze_height);
}


void Maze::SetMazeDimensions(int my_maze_width, int my_maze_height)
{
	maze_height = my_maze_height;
	maze_width = my_maze_width;
	printf("Maze dimensions set: width %d, height %d\n", my_maze_width, my_maze_height);
}


void Maze::SetMazeBlockHeight(int height)
{
	maze_block_height = height;
}

void Maze::SetMazeBlockWidth(int width)
{
	maze_block_width = width;
}

void Maze::AddCreatureToMazeWalls(Creature* ptr_my_creature)
{
	maze_walls.push_back(ptr_my_creature);
}

void Maze::RemoveCreatureFromMazeWalls(Creature* ptr_my_creature)
{
	// #TODO dodaæ zachowanie w razie nieznaleznienia takiego wskaŸnika w tablicye.
	maze_walls.erase(std::remove(maze_walls.begin(), maze_walls.end(), ptr_my_creature), maze_walls.end());
}

void Maze::PrepareMazeGrid()
{
	//if (ptr_level->map == NULL)
	//{
	//	throw std::invalid_argument("Trying to prepare maze grid on a map which was not initialized yet!");
	//}
	ptr_level->PrintMap();
	printf("Will generate maze. Level height: %d, level width: %d\n", ptr_level->TellLevelHeight(),
		ptr_level->TellLevelWidth());
	printf("Maze start column: %d, maze start row: %d.\n", maze_start_column, maze_start_row);
	printf("Maze finish column: %d, maze finish row: %d.\n", maze_finish_column, maze_finish_row);
	for (int row = 0; row <= maze_finish_row - maze_start_row; row++)
	{
		for (int column = 0; column <= maze_finish_column - maze_start_column; column++)
		{
			if (column % (maze_block_width - 1) == 0 || row % (maze_block_height - 1) == 0)
			{
				printf("Will insert tile: x: %d y: %d\n", maze_start_column + column, maze_start_row + row);
				SDL_Rect tile_position = {maze_start_column + column, maze_start_row + row, 0, 0};
				SDL_Rect* ptr_tile_position = &tile_position;
				Creature* ptr_inserted_wall = ptr_level->InsertCreatureOntoMap(cre_flying_box, ptr_tile_position, true);
				AddCreatureToMazeWalls(ptr_inserted_wall);
			}
		}
	}
	printf("Prepared maze grid.\n");
	ptr_level->PrintMap();
}

void Maze::GenerateMaze()
{
	PrepareMazeGrid();
	/*
	How does this work?
	Initial cell for maze generation is chosen.
    While there is at least one not visited cell:
        -> Random direction is chosen
        -> It is checked if neighbor in this direction exists and wasn't visited yet:
           ->If yes: we're removing wall between current cell and neighbor cell. Current cell is changed to
             neighbor. Previous cell is added to trace vector.
           ->If not: next direction is drawn from available ones.
	       ->If there are no unvisited neighbors - we're backtracing to previous cell from trace vector.
     */
	std::vector <Coordinates> trace;
	//Pick random maze cell
	srand(time(NULL));
	//int chosen_maze_block_x = rand() % (maze_finish_column - maze_start_column + 1) + maze_start_column;
	//int chosen_maze_block_y = rand() % (maze_finish_row - maze_start_row + 1) + maze_start_row;
	int chosen_maze_block_x = rand() % maze_width;
	int chosen_maze_block_y = rand() % maze_height;
	//Variable storing currently evaluated cell
	Coordinates current_cell = { chosen_maze_block_x, chosen_maze_block_y };
	CreateEmptyVisitedCellsGrid();
	int visited_count = 1; //Initially set to 1, cause cell where we begin is visited at start.
	std::vector<Directions> possible_directions = { north, west, east, south };
	while (visited_count < maze_height * maze_width)
	{
		visited_cells[current_cell.y][current_cell.x] = 1;
		srand(time(NULL));
		int chosen_direction_index = rand() % possible_directions.size();
		Directions chosen_direction = possible_directions[chosen_direction_index];
		printf("Current cell in maze genereation: x:%d y:%d.\n", current_cell.x, current_cell.y);
		printf("Direction chosen for maze generation: %d.\n", chosen_direction);
		printf("Visited cells count: %d.\n", visited_count);
		if (CheckIfNeighbourIsAvailable(current_cell, chosen_direction))
		{
			visited_count++;
			trace.push_back(current_cell);
			printf("Cell x:%d y:%d pushed back into trace.\n", current_cell.x, current_cell.y);
			if (chosen_direction == north)
			{
				RemoveCellWall(current_cell, chosen_direction);
				current_cell.y -= 1;
				possible_directions = { north, west, east, south };
				printf("Maze trace moved north.\n");
			}
			else if (chosen_direction == south)
			{
				RemoveCellWall(current_cell, chosen_direction);
				current_cell.y += 1;
				possible_directions = { north, west, east, south };
				printf("Maze trace moved south.\n");
			}
			else if (chosen_direction == west)
			{
				RemoveCellWall(current_cell, chosen_direction);
				current_cell.x -= 1;
				possible_directions = { north, west, east, south };
				printf("Maze trace moved west.\n");
			}
			else if (chosen_direction == east)
			{
				RemoveCellWall(current_cell, chosen_direction);
				current_cell.x += 1;
				possible_directions = { north, west, east, south };
				printf("Maze trace moved east.\n");
			}
			else
			{
				throw std::invalid_argument("Chosen direction is not north, south, east or west.");
			}
		}
		else if (possible_directions.size() > 1)
		{
			printf("Possible directions left before deletion: %d\n", possible_directions.size());
			possible_directions.erase(std::remove(possible_directions.begin(), possible_directions.end(), chosen_direction), possible_directions.end());
		}
		else if (trace.size() > 1)
		{
			printf("BACKTRACKING!\n");
			for (int i = 0; i < trace.size(); i++)
			{
				printf("Position %d in trace vector: x:%d, y:%d\n", i, trace[i].x, trace[i].y);
			}
			trace.pop_back();
			printf("AFTER LAST ELEMENT DELETION:\n");
			for (int i = 0; i < trace.size(); i++)
			{
				printf("Position %d in trace vector: x:%d, y:%d\n", i, trace[i].x, trace[i].y);
			}
			current_cell = trace.back();
			printf("Backtracking. Current cell set for x: %d, y: %d.\n", current_cell.x, current_cell.y);
		}
		else
        //# TODO - bug z cofaniem do ostatniego elementu trace
		{
			printf("Something went wrong during backtracing.\n");
			break;
		}
	}
	ptr_level->PrintMap();
	printf("Maze generated.\n");
}

bool Maze::CheckIfNeighbourIsAvailable(Coordinates my_current_cell, Directions my_direction)
{
	if (my_direction == south && my_current_cell.y+1 < maze_height)
	{
		printf("South. Visited?: %d\n", visited_cells[my_current_cell.y + 1][my_current_cell.x]);
		if (visited_cells[my_current_cell.y+1][my_current_cell.x] == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	if (my_direction == north && my_current_cell.y - 1 >= 0)
	{
		printf("North. Visited?: %d\n", visited_cells[my_current_cell.y - 1][my_current_cell.x]);
		if (visited_cells[my_current_cell.y-1][my_current_cell.x] == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	if (my_direction == west && my_current_cell.x - 1 >= 0)
	{
		printf("West. Visited?: %d\n", visited_cells[my_current_cell.y][my_current_cell.x-1]);
		if (visited_cells[my_current_cell.y][my_current_cell.x-1] == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	if (my_direction == east && my_current_cell.x + 1 < maze_width)
	{
		printf("East. Visited?: %d\n", visited_cells[my_current_cell.y][my_current_cell.x+1]);
		if (visited_cells[my_current_cell.y][my_current_cell.x+1] == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		printf("Chosen direction %d unavailable. 0=north, 1=east, 2=south, 3=west\n", my_direction);
		return false;
	}

}

void Maze::RemoveCellWall(Coordinates my_current_cell, Directions my_direction)
{
	if (my_direction == north)
	{
		for (int i = 1; i < maze_block_width - 1; i++)
		{
			int carved_block_column = maze_start_column + (my_current_cell.x)*(maze_block_width - 1) + i;
			int carved_block_row = maze_start_row + (my_current_cell.y)*(maze_block_height - 1);
			SDL_Rect carved_block_position = { carved_block_column, carved_block_row };
			SDL_Rect* ptr_carved_block_position = &carved_block_position;
			// #TODO ustaliæ, czy przekazujemy koordynaty wskaŸnikiem czy przez dwie liczby
			CarveWall(ptr_carved_block_position);
		}
	}
	else if (my_direction == south)
	{
		for (int i = 1; i < maze_block_width - 1; i++)
		{
			int carved_block_column = maze_start_column + (my_current_cell.x)*(maze_block_width - 1) + i;
			int carved_block_row = maze_start_row + (my_current_cell.y + 1)*(maze_block_height - 1);
			SDL_Rect carved_block_position = { carved_block_column, carved_block_row };
			SDL_Rect* ptr_carved_block_position = &carved_block_position;
			CarveWall(ptr_carved_block_position);
		}
	}
	else if (my_direction == east)
	{
		for (int i = 1; i < maze_block_height - 1; i++)
		{
			int carved_block_column = maze_start_column + (my_current_cell.x + 1)*(maze_block_width - 1);
			int carved_block_row = maze_start_row + (my_current_cell.y)*(maze_block_height - 1) + i;
			SDL_Rect carved_block_position = { carved_block_column, carved_block_row };
			SDL_Rect* ptr_carved_block_position = &carved_block_position;
			CarveWall(ptr_carved_block_position);
		}
	}
	else if (my_direction == west)
	{
		for (int i = 1; i < maze_block_height - 1; i++)
		{
			int carved_block_column = maze_start_column + (my_current_cell.x)*(maze_block_width - 1);
			int carved_block_row = maze_start_row + (my_current_cell.y)*(maze_block_height - 1) + i;
			SDL_Rect carved_block_position = { carved_block_column, carved_block_row };
			SDL_Rect* ptr_carved_block_position = &carved_block_position;
			CarveWall(ptr_carved_block_position);
		}
	}
	else
	{
		throw std::invalid_argument("Chosen direction is not north, south, east or west.");
	}
}

void Maze::CarveWall(SDL_Rect* ptr_position_of_block_to_carve)
{
    //# TODO - walidacja?
	//Method deleting a single block from map and its pointer from the vector holding all maze walls.
	int row = ptr_position_of_block_to_carve->y;
	int column = ptr_position_of_block_to_carve->x;
	Creature* ptr_block_to_carve = ptr_level->TellPointerToCreatureInSlot(column, row);
	RemoveCreatureFromMazeWalls(ptr_block_to_carve);
	ptr_level->RemoveCreatureFromMap(column, row);
	printf("Carved x: %d, y: %d\n", column, row);
}


void Maze::CreateEmptyVisitedCellsGrid()
{
	//Creating a 2D dynamically sized array
	int** my_visited_cells = new int*[maze_height];
	for (int row = 0; row < maze_height; row++)
	{
		my_visited_cells[row] = new int[maze_width];
		for (int column = 0; column < maze_width; column++)
		{
			my_visited_cells[row][column] = 0;
		}
	}
	visited_cells = my_visited_cells;
}

void Maze::ClearMaze()
{
	//Destroying every creature from maze walls vector
	for (Creature* ptr_creature : maze_walls)
	{
		ptr_creature->~Creature();
	}
	//Erasing maze_walls content
	maze_walls.clear();
	//Erasing visited cells grid
	CreateEmptyVisitedCellsGrid();
}