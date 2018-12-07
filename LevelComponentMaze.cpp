#include <LevelComponentMaze.hpp>

LevelComponentMaze::LevelComponentMaze(std::map<LevelComponentType, std::vector<LevelComponent*>>* my_ptr_peer_level_components, 
	SDL_Rect my_component_area) :
	LevelComponent(my_ptr_peer_level_components, my_component_area)
{
	if (ValidateMazeArea(my_component_area) != true)
	{
		std::string exception_text = "Requested maze size w : " + std::to_string(my_component_area.w) + " h: " + std::to_string(my_component_area.w) +
			" cannot be divided into maze blocks of currently set width: " + std::to_string(maze_block_width*map_block_width) + " height: " +
			std::to_string(map_block_height*maze_block_width);
		throw std::invalid_argument(exception_text);
	}
	else
	{
		printf("Maze component area passed to constructor is x: %d, y: %d, w: %d, h: %d.\n",
			my_component_area.x, my_component_area.y, my_component_area.w, my_component_area.h);
		maze_x_start = my_component_area.x;
		maze_y_start = my_component_area.y;
		maze_w = my_component_area.w;
		maze_h = my_component_area.h;
		

	}
	maze_rows_count = CalculateRowsNumber();
	maze_columns_count = CalculateColumnsNumber();
	maze_blocks_count_vertical = CalculateBlocksCountVertically();
	maze_blocks_count_horizontal = CalculateBlocksCountHorizontally();
	printf("Maze rows: %d, maze columns: %d, maze blocks vertically: %d, maze blocks horizontally: %d.\n",
		maze_rows_count, maze_columns_count, maze_blocks_count_vertical, maze_blocks_count_horizontal);
	std::vector<Creature*>* ptr_to_creatures = TellPtrToCreaturesArray();
	//PrintBlueprint();
}

int LevelComponentMaze::CalculateRowsNumber()
{
	return (maze_h / map_block_height);
}

int LevelComponentMaze::CalculateColumnsNumber()
{
	return (maze_w / map_block_width);
}

int LevelComponentMaze::CalculateBlocksCountVertically()
{
	return (maze_rows_count / (maze_block_height-1));
}

int LevelComponentMaze::CalculateBlocksCountHorizontally()
{
	return (maze_columns_count / (maze_block_width-1));
}

bool LevelComponentMaze::ValidateMazeArea(SDL_Rect maze_area)
{
	int maze_h = maze_area.h;
	int maze_w = maze_area.w;
	//If maze height is not dividable into maze block
	if ((maze_h - maze_block_height * map_block_height) % (maze_block_height*map_block_height - map_block_height) != 0)
	{
		printf("Maze height %d not acceptable.\n", maze_h);
		return false;
	}
	else if ((maze_w - maze_block_width * map_block_width) % (maze_block_width*map_block_width - map_block_width) != 0)
	{
		printf("Maze width %d not acceptable.\n", maze_w);
		return false;
	}
	else
	{
		return true;
	}
}

void LevelComponentMaze::ClearBlueprint()
{
	blueprint.clear();
}

void LevelComponentMaze::PrepareMazeGrid()
//Method preparing grid on the blueprint - set of square rooms, which walls will be carved afterwards.
{
	//Insert all possible maze walls into blueprint
	for (int row = 0; row < maze_rows_count; row++)
	{
		blueprint.push_back({});
		for (int column = 0; column < maze_columns_count; column++)
		{
			if (column % (maze_block_width - 1) == 0 || row % (maze_block_height - 1) == 0)
			{
				//printf("Will insert tile: x: %d y: %d\n", column, row);
				blueprint[row].push_back(cre_flying_box);
			}
			else
			{
				blueprint[row].push_back(cre_none);
			}
		}
	}
}

void LevelComponentMaze::VivifyMaze()
//Method spawning creatures according to current maze blueprint.
{
	PrintBlueprint();
	for (int row = 0; row < maze_rows_count; row++)
	{
		for (int column = 0; column < maze_columns_count; column++)
		{
			CreatureType my_type = blueprint[row][column];
			if (my_type != cre_none)
			{
				SDL_Rect my_position = SDL_Rect{ maze_x_start+(column*map_block_width), 
					maze_y_start+(row*map_block_width),0,0 };
				SDL_Rect* ptr_my_position = &my_position;
				AddCreature(my_type, ptr_my_position, merge);
			}
		}
	}
}

void LevelComponentMaze::PrintBlueprint()
{
	printf("\n\n========Will print maze blueprint========\n\n");
	for (int row = 0; row < maze_rows_count; row++)
	{
		printf("\n");
		for (int column = 0; column < maze_columns_count; column++)
		{
			if (blueprint[row][column] != cre_none)
			{
				CreatureType my_creature = blueprint[row][column];
				printf("[%d]", my_creature);
			}
			else
			{
				printf("[ ]");
			}
		}
	}
	printf("\n\n========Printed blueprint========\n\n");
}

void LevelComponentMaze::GenerateMaze()
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
	int chosen_maze_block_x = rand() % maze_blocks_count_horizontal;
	int chosen_maze_block_y = rand() % maze_blocks_count_vertical;
	//Variable storing currently evaluated cell
	Coordinates current_cell = { chosen_maze_block_x, chosen_maze_block_y };
	CreateEmptyVisitedCellsGrid();
	int visited_count = 1; //Initially set to 1, cause cell where we begin is visited at start.
	std::vector<Directions> possible_directions = { north, west, east, south };
	while (visited_count < maze_blocks_count_horizontal * maze_blocks_count_vertical)
	{
		visited_cells[current_cell.y][current_cell.x] = 1;
		int chosen_direction_index = rand() % possible_directions.size();
		Directions chosen_direction = possible_directions[chosen_direction_index];
		//printf("Current cell in maze genereation: x:%d y:%d.\n", current_cell.x, current_cell.y);
		//printf("Direction chosen for maze generation: %d.\n", chosen_direction);
		//printf("Visited cells count: %d.\n", visited_count);
		if (CheckIfNeighbourIsAvailable(current_cell, chosen_direction))
		{
			visited_count++;
			trace.push_back(current_cell);
			//printf("Cell x:%d y:%d pushed back into trace.\n", current_cell.x, current_cell.y);
			if (chosen_direction == north)
			{
				RemoveCellWall(current_cell, chosen_direction);
				current_cell.y -= 1;
				possible_directions = { north, west, east, south };
				//printf("Maze trace moved north.\n");
			}
			else if (chosen_direction == south)
			{
				RemoveCellWall(current_cell, chosen_direction);
				current_cell.y += 1;
				possible_directions = { north, west, east, south };
				//printf("Maze trace moved south.\n");
			}
			else if (chosen_direction == west)
			{
				RemoveCellWall(current_cell, chosen_direction);
				current_cell.x -= 1;
				possible_directions = { north, west, east, south };
				//printf("Maze trace moved west.\n");
			}
			else if (chosen_direction == east)
			{
				RemoveCellWall(current_cell, chosen_direction);
				current_cell.x += 1;
				possible_directions = { north, west, east, south };
				//printf("Maze trace moved east.\n");
			}
			else
			{
				throw std::invalid_argument("Chosen direction is not north, south, east or west.");
			}
		}
		else if (possible_directions.size() > 1)
		{
			//printf("Possible directions left before deletion: %d\n", possible_directions.size());
			possible_directions.erase(std::remove(possible_directions.begin(), possible_directions.end(), chosen_direction), possible_directions.end());
		}
		else if (trace.size() > 1)
		{
			//printf("BACKTRACKING!\n");
			//for (int i = 0; i < trace.size(); i++)
			//{
				//printf("Position %d in trace vector: x:%d, y:%d\n", i, trace[i].x, trace[i].y);
			//}
			trace.pop_back();
			//printf("AFTER LAST ELEMENT DELETION:\n");
			//for (int i = 0; i < trace.size(); i++)
			//{
				//printf("Position %d in trace vector: x:%d, y:%d\n", i, trace[i].x, trace[i].y);
			//}
			current_cell = trace.back();
			possible_directions = { north, west, east, south };
			//printf("Backtracking. Current cell set for x: %d, y: %d.\n", current_cell.x, current_cell.y);
		}
		else
		{
			printf("Something went wrong during backtracing.\n");
			break;
		}
	}
	CarveExitsFromMaze();
	ManageBorders();
	VivifyMaze();
	printf("Maze generated.\n");
}

void LevelComponentMaze::CreateEmptyVisitedCellsGrid()
{
	//Creating a 2D dynamically sized array
	//#TODO - destruct!
	int** my_visited_cells = new int*[maze_blocks_count_vertical];
	for (int row = 0; row < maze_blocks_count_vertical; row++)
	{
		my_visited_cells[row] = new int[maze_blocks_count_horizontal];
		for (int column = 0; column < maze_blocks_count_horizontal; column++)
		{
			my_visited_cells[row][column] = 0;
		}
	}
	visited_cells = my_visited_cells;
}

bool LevelComponentMaze::CheckIfNeighbourIsAvailable(Coordinates my_current_cell, Directions my_direction)
{
	if (my_direction == south && my_current_cell.y + 1 < maze_blocks_count_vertical)
	{
		//printf("South. Visited?: %d\n", visited_cells[my_current_cell.y + 1][my_current_cell.x]);
		if (visited_cells[my_current_cell.y + 1][my_current_cell.x] == 0)
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
		//printf("North. Visited?: %d\n", visited_cells[my_current_cell.y - 1][my_current_cell.x]);
		if (visited_cells[my_current_cell.y - 1][my_current_cell.x] == 0)
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
		//printf("West. Visited?: %d\n", visited_cells[my_current_cell.y][my_current_cell.x - 1]);
		if (visited_cells[my_current_cell.y][my_current_cell.x - 1] == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	if (my_direction == east && my_current_cell.x + 1 < maze_blocks_count_horizontal)
	{
		//printf("East. Visited?: %d\n", visited_cells[my_current_cell.y][my_current_cell.x + 1]);
		if (visited_cells[my_current_cell.y][my_current_cell.x + 1] == 0)
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
		//printf("Chosen direction %d unavailable. 0=north, 1=east, 2=south, 3=west\n", my_direction);
		return false;
	}

}

void LevelComponentMaze::RemoveCellWall(Coordinates my_current_cell, Directions my_direction)
{
	if (my_direction == north)
	{
		for (int i = 1; i < maze_block_width - 1; i++)
		{
			int carved_block_column = (my_current_cell.x)*(maze_block_width - 1) + i;
			int carved_block_row = (my_current_cell.y)*(maze_block_height - 1);
			blueprint[carved_block_row][carved_block_column] = cre_none;
			// #TODO ustaliæ, czy przekazujemy koordynaty wskaŸnikiem czy przez dwie liczby
		}
	}
	else if (my_direction == south)
	{
		for (int i = 1; i < maze_block_width - 1; i++)
		{
			int carved_block_column = (my_current_cell.x)*(maze_block_width - 1) + i;
			int carved_block_row = (my_current_cell.y + 1)*(maze_block_height - 1);
			blueprint[carved_block_row][carved_block_column] = cre_none;
		}
	}
	else if (my_direction == east)
	{
		for (int i = 1; i < maze_block_height - 1; i++)
		{
			int carved_block_column = (my_current_cell.x + 1)*(maze_block_width - 1);
			int carved_block_row = (my_current_cell.y)*(maze_block_height - 1) + i;
			blueprint[carved_block_row][carved_block_column] = cre_none;
		}
	}
	else if (my_direction == west)
	{
		for (int i = 1; i < maze_block_height - 1; i++)
		{
			int carved_block_column = (my_current_cell.x)*(maze_block_width - 1);
			int carved_block_row = (my_current_cell.y)*(maze_block_height - 1) + i;
			blueprint[carved_block_row][carved_block_column] = cre_none;
		}
	}
	else
	{
		throw std::invalid_argument("Chosen direction is not north, south, east or west.");
	}
}

void LevelComponentMaze::CarveExitsFromMaze()
{
	//Function providing an exit in every maze outer wall.
	//Pick random cell from northern boundary
	int chosen_maze_block_x = rand() % maze_blocks_count_horizontal;
	for (int i = 1; i < maze_block_width - 1; i++)
	{
		int carved_block_column = (chosen_maze_block_x)*(maze_block_width - 1) + i;
		int carved_block_row = 0;
		blueprint[carved_block_row][carved_block_column] = cre_none;
	}
	//Pick random cell from southern boundary
	chosen_maze_block_x = rand() % maze_blocks_count_horizontal;
	for (int i = 1; i < maze_block_width - 1; i++)
	{
		int carved_block_column = (chosen_maze_block_x)*(maze_block_width - 1) + i;
		int carved_block_row = maze_rows_count-1;
		blueprint[carved_block_row][carved_block_column] = cre_none;
	}
	//Pick random cell from eastern boundary
	int chosen_maze_block_y = rand() % maze_blocks_count_vertical;
	for (int i = 1; i < maze_block_height - 1; i++)
	{
		int carved_block_column = 0;
		int carved_block_row = (chosen_maze_block_y)*(maze_block_height - 1) + i;
		blueprint[carved_block_row][carved_block_column] = cre_none;
	}
	//Pick random cell from western boundary
	chosen_maze_block_y = rand() % maze_blocks_count_vertical;
	for (int i = 1; i < maze_block_height - 1; i++)
	{
		int carved_block_column = maze_columns_count-1;
		int carved_block_row = (chosen_maze_block_y)*(maze_block_height - 1) + i;
		blueprint[carved_block_row][carved_block_column] = cre_none;
	}
}

void LevelComponentMaze::SetBorderState(Directions border_side, bool value)
{
	if (border_side == east)
	{
		eastern_border = value;
	}
	else if (border_side == west)
	{
		western_border = value;
	}
	else if (border_side == south)
	{
		southern_border = value;
	}
	else if (border_side == north)
	{
		northern_border = value;
	}
	else
	{
		throw std::invalid_argument("Wrong direction value passed to SetBorderState.\n");
	}
}


void LevelComponentMaze::ManageBorders()
{
	if (western_border == false)
	{
		for (int row = 0; row < maze_rows_count; row++)
		{
			blueprint[row][0] = cre_none;
		}
	}
	else if (eastern_border == false)
	{
		for (int row = 0; row < maze_rows_count; row++)
		{
			blueprint[row][maze_columns_count - 1] = cre_none;
		}
	}
	else if (northern_border == false)
	{
		for (int column = 0; column < maze_columns_count; column++)
		{
			blueprint[0][column] = cre_none;
		}
	}
	else if (southern_border == false)
	{
		for (int column = 0; column < maze_columns_count; column++)
		{
			blueprint[maze_rows_count-1][column] = cre_none;
		}
	}
}
void LevelComponentMaze::ClearMaze()
{
	std::vector<Creature*>* ptr_to_creatures = TellPtrToCreaturesArray();
	printf("Current creatures count on level: %d.\n", ptr_to_creatures->size());
	RemoveAllCreatures();
	ClearBlueprint();
}


//#######################
//STATIC HELPERS
//#######################

SDL_Rect LevelComponentMaze::CalculateMazeDimensions(int w, int h, int my_maze_block_width, int my_maze_block_height)
{
	//Calculates area in pixels that would be occupied by maze consisting of w*h maze blocks.
	//Maze block dimensions are expressed in map_block units. Map block is the smallest building brick with dimensions expressed in pixels.
	int map_block_width = LevelComponent::map_block_width;
	int map_block_height = LevelComponent::map_block_height;
	//w and h are expresses in maze blocks
	printf("Input for calculation: w: %d h: %d.\n", w, h);
	int pixel_width = (my_maze_block_width*map_block_width) + ((my_maze_block_width - 1)*map_block_width)*(w-1);
	int pixel_height = (my_maze_block_height*map_block_width) + ((my_maze_block_height - 1)*map_block_width)*(h-1);
	printf("Calculated pixel height: %d.\n", pixel_height);
	printf("Calculated pixel width: %d.\n", pixel_width);
	SDL_Rect result = { 0,0,pixel_width,pixel_height };
	return result;
}