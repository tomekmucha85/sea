#include <LevelNineMazes.hpp>

LevelNineMazes::LevelNineMazes(int my_cols_count, int my_rows_count) : Level()
{

	//Core part generation
    // #TODO - uwspólniæ?
	SDL_Rect core_area = { 0,0,0,0 };
	LevelComponent* ptr_core = ptr_components_factory->SpawnLevelComponent(levco_core, core_area);
	SDL_Rect guy_area = { 10,10,0,0 };
	SDL_Rect* ptr_guy_area = &guy_area;
	Creature* hero = ptr_core->AddCreature(cre_clawy, ptr_guy_area, force);
	SDL_Rect box_area = { 20,20,0,0 };
	SDL_Rect* ptr_box_area = &box_area;
	ptr_core->AddCreature(cre_flying_box, ptr_box_area, merge);
	SDL_Rect hero_position = { 400,380,0,0 };
	SDL_Rect* ptr_hero_position = &hero_position;
	Creature* ptr_hero = ptr_core->AddCreature(cre_clawy, ptr_hero_position, force);
	ptr_hero->MakeMeMainCharacter();
	SetCurrentHero(ptr_hero);


	//Adding component for event triggers
	LevelComponent* ptr_event_triggers = ptr_components_factory->SpawnLevelComponent(levco_triggers);


	//Add action from common lambdas - firing all triggers currently being hit by hero
	cyclic_actions.push_back(func_fire_triggers);

	//How many map blocks will have a maze segment?
	SetMazeRowsCount(my_rows_count);
	SetMazeColsCount(my_cols_count);

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

	//SpawnInitialCentralMaze();

	for (int i = 1; i <= 4; i++)
	{
		GenerateMazeNumber(i);
	}

	for (int i = 6; i <= 9; i++)
	{
		GenerateMazeNumber(i);
	}

	// Defining trigger action
	std::function<void()> ptr_func = [this]()
	{
		this->ptr_current_central_maze = ptr_maze2;
		printf("TRIGGER!\n");
		//this->GenerateMazeNumberNewApproach(1);
		this->GenerateMazeNumber(2);
		//this->GenerateMazeNumberNewApproach(3);
		//this->DeleteMazeNumber(7);
		this->DeleteMazeNumber(8);
		//this->DeleteMazeNumber(9);
	};
	//Defining entry in dictionary
	std::string signal_for_maze_2_reload = "reload maze 2";

	//Creating trigger
	SDL_Rect event_area = ptr_maze5->TellComponentEdge(north);
	printf("Added trigger x: %d, y: %d, w: %d, h: %d.\n", event_area.x, event_area.y, event_area.w, event_area.h);
	SDL_Rect* ptr_event_area = &event_area;
	Creature* ptr_trigger_1 = ptr_event_triggers->AddCreature(cre_event_trigger, ptr_event_area, merge, signal_for_maze_2_reload);
	signals_vs_events["reload maze 2"] = ptr_func;
}

void LevelNineMazes::SetMazeRowsCount(int rows_num)
{
	maze_rows_count = rows_num;
}

void LevelNineMazes::SetMazeColsCount(int cols_num)
{
    maze_columns_count = cols_num;
}

void LevelNineMazes::SetCurrentCentralMaze(LevelComponent* ptr_my_maze)
{
	ptr_current_central_maze = ptr_my_maze;
}
LevelComponent* LevelNineMazes::TellCurrentCentralMaze()
{
	return ptr_current_central_maze;
}

// #TODO rozwa¿yæ u¿ycie tablicy zamiast sztywnego switcha
void LevelNineMazes::DeleteMazeNumber(int my_number)
{
	if (my_number == 1)
	{
		RemoveLevelComponent(ptr_maze1);
		ptr_maze1 = nullptr;
	}
	else if (my_number == 2)
	{
		RemoveLevelComponent(ptr_maze2);
		ptr_maze2 = nullptr;
	}
	else if (my_number == 3)
	{
		RemoveLevelComponent(ptr_maze3);
		ptr_maze3 = nullptr;
	}
	else if (my_number == 4)
	{
		RemoveLevelComponent(ptr_maze4);
		ptr_maze4 = nullptr;
	}
	else if (my_number == 5)
	{
		RemoveLevelComponent(ptr_maze5);
		ptr_maze5 = nullptr;
	}
	else if (my_number == 6)
	{
		RemoveLevelComponent(ptr_maze6);
		ptr_maze6 = nullptr;
	}
	else if (my_number == 7)
	{
		RemoveLevelComponent(ptr_maze7);
		ptr_maze7 = nullptr;
	}
	else if (my_number == 8)
	{
		RemoveLevelComponent(ptr_maze8);
		ptr_maze8 = nullptr;
	}
	else if (my_number == 9)
	{
	RemoveLevelComponent(ptr_maze9);
	ptr_maze9 = nullptr;
	}
	else
	{
		printf("Such maze number does not exist!\n");
		throw std::invalid_argument("Such maze number does not exist!\n");
	}
}

SDL_Rect LevelNineMazes::CalculateInitialCentralMazeArea()
{
	int map_block_width = LevelComponent::map_block_width;
	int map_block_height = LevelComponent::map_block_height;
	SDL_Rect central_maze_initial_coordinates = LevelComponentMaze::CalculateMazeDimensions(maze_columns_count, maze_rows_count);
	return central_maze_initial_coordinates;
}

LevelComponent* LevelNineMazes::SpawnInitialCentralMaze()
{
	SDL_Rect maze_area = CalculateInitialCentralMazeArea();
	printf("Spawning maze in area: x: %d, y: %d, w: %d, h: %d.\n", maze_area.x, maze_area.y, maze_area.w, maze_area.h);
	LevelComponent* ptr_central_maze = ptr_components_factory->SpawnLevelComponent(levco_maze, maze_area);
	printf("WILL GENERATE INITIAL CENTRAL MAZE.\n");
	ptr_central_maze->GenerateMaze();
	SetCurrentCentralMaze(ptr_central_maze);
	ptr_maze5 = ptr_central_maze;
	return ptr_central_maze;
}

void LevelNineMazes::GenerateMazeNumber(int my_number)
{
	int map_block_width = LevelComponent::map_block_width;
	int map_block_height = LevelComponent::map_block_height;
	
	SDL_Rect current_central_maze_coordinates = {};

	//DeleteMazeNumber(my_number);

	//If there's no central maze for reference - spawn one!
	if (ptr_current_central_maze == nullptr)
	{
		printf("No central maze for reference. Spawning maze 5.\n");
		SpawnInitialCentralMaze();
	}

	current_central_maze_coordinates = ptr_current_central_maze->TellComponentArea();
	printf("Current central component area: x: %d, y: %d, w: %d, h: %d.\n", current_central_maze_coordinates.x,
		current_central_maze_coordinates.y, current_central_maze_coordinates.w, current_central_maze_coordinates.h);

	if (my_number == 1)
	{
		maze_1_area = current_central_maze_coordinates;
		maze_1_area.x += (current_central_maze_coordinates.w*-1) + map_block_width;
		maze_1_area.y += (current_central_maze_coordinates.h*-1) + map_block_height;
		ptr_maze1 = ptr_components_factory->SpawnLevelComponent(levco_maze, maze_1_area);
		ptr_maze1->SetBorderState(east, false);
		ptr_maze1->SetBorderState(south, false);
		printf("WILL GENERATE MAZE %d.\n", my_number);
		ptr_maze1->GenerateMaze();
	}
	else if (my_number == 2)
	{

		maze_2_area = current_central_maze_coordinates;
		maze_2_area.y += (current_central_maze_coordinates.h*-1) + map_block_height;
		ptr_maze2 = ptr_components_factory->SpawnLevelComponent(levco_maze, maze_2_area);
		ptr_maze2->SetBorderState(south, false);
		printf("WILL GENERATE MAZE %d.\n", my_number);
		ptr_maze2->GenerateMaze();
	}
	else if (my_number == 3)
	{
		maze_3_area = current_central_maze_coordinates;
		maze_3_area.x += current_central_maze_coordinates.w - map_block_width;
		maze_3_area.y += (current_central_maze_coordinates.h*-1) + map_block_height;
		ptr_maze3 = ptr_components_factory->SpawnLevelComponent(levco_maze, maze_3_area);
		ptr_maze3->SetBorderState(south, false);
		ptr_maze3->SetBorderState(west, false);
		printf("WILL GENERATE MAZE %d.\n", my_number);
		ptr_maze3->GenerateMaze();
	}
	else if (my_number == 4)
	{
		maze_4_area = current_central_maze_coordinates;
		maze_4_area.x += (current_central_maze_coordinates.w*-1) + map_block_width;
		ptr_maze4 = ptr_components_factory->SpawnLevelComponent(levco_maze, maze_4_area);
		ptr_maze4->SetBorderState(east, false);
		printf("WILL GENERATE MAZE %d.\n", my_number);
		ptr_maze4->GenerateMaze();
	}
	else if (my_number == 5)
	{
		maze_5_area = current_central_maze_coordinates;
		ptr_maze5 = ptr_components_factory->SpawnLevelComponent(levco_maze, maze_5_area);
		printf("WILL GENERATE MAZE %d.\n", my_number);
		ptr_maze5->GenerateMaze();
	}
	else if (my_number == 6)
	{
		maze_6_area = current_central_maze_coordinates;
		maze_6_area.x += current_central_maze_coordinates.w - map_block_width;
		ptr_maze6 = ptr_components_factory->SpawnLevelComponent(levco_maze, maze_6_area);
		ptr_maze6->SetBorderState(west, false);
		printf("WILL GENERATE MAZE %d.\n", my_number);
		ptr_maze6->GenerateMaze();
	}
	else if (my_number == 7)
	{
		maze_7_area = current_central_maze_coordinates;
		maze_7_area.x += (current_central_maze_coordinates.w*-1) + map_block_width;
		maze_7_area.y += (current_central_maze_coordinates.h) - map_block_height;
		ptr_maze7 = ptr_components_factory->SpawnLevelComponent(levco_maze, maze_7_area);
		ptr_maze7->SetBorderState(east, false);
		ptr_maze7->SetBorderState(north, false);
		printf("WILL GENERATE MAZE %d.\n", my_number);
		ptr_maze7->GenerateMaze();
	}
	else if (my_number == 8)
	{
		maze_8_area = current_central_maze_coordinates;
		maze_8_area.y += (current_central_maze_coordinates.h) - map_block_height;
		ptr_maze8 = ptr_components_factory->SpawnLevelComponent(levco_maze, maze_8_area);
		ptr_maze8->SetBorderState(north, false);
		printf("WILL GENERATE MAZE %d.\n", my_number);
		ptr_maze8->GenerateMaze();
	}
	else if (my_number == 9)
	{
		maze_9_area = current_central_maze_coordinates;
		maze_9_area.y += (current_central_maze_coordinates.h * 2) - map_block_height;
		maze_9_area.x += current_central_maze_coordinates.w - map_block_width;
		ptr_maze9 = ptr_components_factory->SpawnLevelComponent(levco_maze, maze_9_area);
		ptr_maze9->SetBorderState(north, false);
		ptr_maze9->SetBorderState(west, false);
		printf("WILL GENERATE MAZE %d.\n", my_number);
		ptr_maze9->GenerateMaze();
	}
	else
	{
		printf("Such maze number does not exist and cannot be generated!\n");
		throw std::invalid_argument("Such maze number does not exist!\n");
	}
}
