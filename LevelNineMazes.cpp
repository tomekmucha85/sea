#include <LevelNineMazes.hpp>

LevelNineMazes::LevelNineMazes(int my_cols_count, int my_rows_count) : Level()
{

	//Core part generation
    // #TODO - uwspólniæ?
	SDL_Rect core_area = { 0,0,0,0 };
	SDL_Rect* ptr_core_area = &core_area;
	LevelComponent* ptr_core = ptr_components_factory->SpawnLevelComponent(levco_core, ptr_core_area);
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


	// Adding component for event triggers
	LevelComponent* ptr_event_triggers = ptr_components_factory->SpawnLevelComponent(levco_triggers);


	//Add action from common lambdas - firing all triggers currently being hit by hero
	cyclic_actions.push_back(func_fire_triggers);


	//Mazes part
	// #TODO ucywilizowaæ


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

	GenerateMazeNumber(5);
	//Central maze coordinates
	SDL_Rect central_maze_coordinates = LevelComponentMaze::CalculateMazeDimensions(maze_columns_count, maze_rows_count);

	// Defining trigger action
	std::function<void()> ptr_func = [this]()
	{
		printf("TRIGGER!\n");
		this->GenerateMazeNumber(8);
	};
	//Defining entry in dictionary
	std::string signal_for_maze_8_reload = "reload maze 8";

	//Creating trigger
	SDL_Rect event_area = ptr_maze5->TellComponentEdge(north);
	printf("Added trigger x: %d, y: %d, w: %d, h: %d.\n", event_area.x, event_area.y, event_area.w, event_area.h);
	SDL_Rect* ptr_event_area = &event_area;
	Creature* ptr_trigger_1 = ptr_event_triggers->AddCreature(cre_event_trigger, ptr_event_area, merge, signal_for_maze_8_reload);
	signals_vs_events["reload maze 8"] = ptr_func;



	for (int i = 1; i <= 9; i++)
	{
		GenerateMazeNumber(i);
	}
}

void LevelNineMazes::SetMazeRowsCount(int rows_num)
{
	maze_rows_count = rows_num;
}

void LevelNineMazes::SetMazeColsCount(int cols_num)
{
    maze_columns_count = cols_num;
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

void LevelNineMazes::GenerateMazeNumberNewApproach(int my_number)
{
	int map_block_width = LevelComponent::map_block_width;
	int map_block_height = LevelComponent::map_block_height;

	SDL_Rect central_maze_coordinates = LevelComponentMaze::CalculateMazeDimensions(maze_columns_count, maze_rows_count);

	//DeleteMazeNumber(my_number);

	if (my_number == 1)
	{
		maze_1 = central_maze_coordinates;
		maze_1.x += (central_maze_coordinates.w*-1) + map_block_width;
		maze_1.y += (central_maze_coordinates.h*-1) + map_block_height;
		SDL_Rect* ptr_area_maze_1 = &maze_1;
		ptr_maze1 = ptr_components_factory->SpawnLevelComponent(levco_maze, ptr_area_maze_1);
		ptr_maze1->SetBorderState(east, false);
		ptr_maze1->SetBorderState(south, false);
		ptr_maze1->GenerateMaze();
	}
	else if (my_number == 2)
	{
		maze_2 = central_maze_coordinates;
		maze_2.y += (central_maze_coordinates.h*-1) + map_block_height;
		SDL_Rect* ptr_area_maze_2 = &maze_2;
		ptr_maze2 = ptr_components_factory->SpawnLevelComponent(levco_maze, ptr_area_maze_2);
		ptr_maze2->SetBorderState(south, false);
		ptr_maze2->GenerateMaze();
	}
	else if (my_number == 3)
	{
		maze_3 = central_maze_coordinates;
		maze_3.x += central_maze_coordinates.w - map_block_width;
		maze_3.y += (central_maze_coordinates.h*-1) + map_block_height;
		SDL_Rect* ptr_area_maze_3 = &maze_3;
		ptr_maze3 = ptr_components_factory->SpawnLevelComponent(levco_maze, ptr_area_maze_3);
		ptr_maze3->SetBorderState(south, false);
		ptr_maze3->SetBorderState(west, false);
		ptr_maze3->GenerateMaze();
	}
	else if (my_number == 4)
	{
		maze_4 = central_maze_coordinates;
		maze_4.x += (central_maze_coordinates.w*-1) + map_block_width;
		SDL_Rect* ptr_area_maze_4 = &maze_4;
		ptr_maze4 = ptr_components_factory->SpawnLevelComponent(levco_maze, ptr_area_maze_4);
		ptr_maze4->SetBorderState(east, false);
		ptr_maze4->GenerateMaze();
	}
	else if (my_number == 5)
	{
		maze_5 = central_maze_coordinates;
		SDL_Rect* ptr_area_maze_5 = &maze_5;
		printf("Spawning maze in area: x: %d, y: %d, w: %d, h: %d.\n", ptr_area_maze_5->x, ptr_area_maze_5->y, ptr_area_maze_5->w, ptr_area_maze_5->h);
		ptr_maze5 = ptr_components_factory->SpawnLevelComponent(levco_maze, ptr_area_maze_5);
		ptr_maze5->GenerateMaze();
	}
	else if (my_number == 6)
	{
		maze_6 = central_maze_coordinates;
		maze_6.x += central_maze_coordinates.w - map_block_width;
		SDL_Rect* ptr_area_maze_6 = &maze_6;
		ptr_maze6 = ptr_components_factory->SpawnLevelComponent(levco_maze, ptr_area_maze_6);
		ptr_maze6->SetBorderState(west, false);
		ptr_maze6->GenerateMaze();
	}
	else if (my_number == 7)
	{
		maze_7 = central_maze_coordinates;
		maze_7.x += (central_maze_coordinates.w*-1) + map_block_width;
		maze_7.y += (central_maze_coordinates.h) - map_block_height;
		SDL_Rect* ptr_area_maze_7 = &maze_7;
		ptr_maze7 = ptr_components_factory->SpawnLevelComponent(levco_maze, ptr_area_maze_7);
		ptr_maze7->SetBorderState(east, false);
		ptr_maze7->SetBorderState(north, false);
		ptr_maze7->GenerateMaze();
	}
	else if (my_number == 8)
	{
		maze_8 = central_maze_coordinates;
		maze_8.y += (central_maze_coordinates.h) - map_block_height;
		SDL_Rect* ptr_area_maze_8 = &maze_8;
		ptr_maze8 = ptr_components_factory->SpawnLevelComponent(levco_maze, ptr_area_maze_8);
		ptr_maze8->SetBorderState(north, false);
		ptr_maze8->GenerateMaze();
	}
	else if (my_number == 9)
	{
		maze_9 = central_maze_coordinates;
		maze_9.y += (central_maze_coordinates.h * 2) - map_block_height;
		maze_9.x += central_maze_coordinates.w - map_block_width;
		SDL_Rect* ptr_area_maze_9 = &maze_9;
		ptr_maze9 = ptr_components_factory->SpawnLevelComponent(levco_maze, ptr_area_maze_9);
		ptr_maze9->SetBorderState(north, false);
		ptr_maze9->SetBorderState(west, false);
		ptr_maze9->GenerateMaze();
	}
	else
	{
		printf("Such maze number does not exist and cannot be generated!\n");
		throw std::invalid_argument("Such maze number does not exist!\n");
	}
}

void LevelNineMazes::GenerateMazeNumber(int my_number)
{
	int map_block_width = LevelComponent::map_block_width;
	int map_block_height = LevelComponent::map_block_height;
	long long int x_shift_caused_by_main_character_movement = Creature::TellXMainCharacterShift();
	long long int y_shift_caused_by_main_character_movement = Creature::TellYMainCharacterShift();

	SDL_Rect maze_5_dimensions = LevelComponentMaze::CalculateMazeDimensions(maze_columns_count, maze_rows_count);
	maze_5_dimensions.x = x_shift_caused_by_main_character_movement;
	maze_5_dimensions.y = y_shift_caused_by_main_character_movement;
	printf("X shift: %d, Y shift: %d.\n", x_shift_caused_by_main_character_movement, x_shift_caused_by_main_character_movement);
	
	DeleteMazeNumber(my_number);

	if (my_number == 1)
	{
		maze_1 = maze_5_dimensions;
		maze_1.x += (maze_5_dimensions.w*-1) + map_block_width;
		maze_1.y += (maze_5_dimensions.h*-1) + map_block_height;
		SDL_Rect* ptr_area_maze_1 = &maze_1;
		ptr_maze1 = ptr_components_factory->SpawnLevelComponent(levco_maze, ptr_area_maze_1);
		ptr_maze1->SetBorderState(east, false);
        ptr_maze1->SetBorderState(south, false);
		ptr_maze1->GenerateMaze();
	}
	else if (my_number == 2)
	{
		maze_2 = maze_5_dimensions;
		maze_2.y += (maze_5_dimensions.h*-1) + map_block_height;
		SDL_Rect* ptr_area_maze_2 = &maze_2;
		ptr_maze2 = ptr_components_factory->SpawnLevelComponent(levco_maze, ptr_area_maze_2);
		ptr_maze2->SetBorderState(south, false);
		ptr_maze2->GenerateMaze();
	}
	else if (my_number == 3)
	{
		maze_3 = maze_5_dimensions;
		maze_3.x += maze_5_dimensions.w - map_block_width;
		maze_3.y += (maze_5_dimensions.h*-1) + map_block_height;
		SDL_Rect* ptr_area_maze_3 = &maze_3;
		ptr_maze3 = ptr_components_factory->SpawnLevelComponent(levco_maze, ptr_area_maze_3);
		ptr_maze3->SetBorderState(south, false);
		ptr_maze3->SetBorderState(west, false);
		ptr_maze3->GenerateMaze();
	}
	else if (my_number == 4)
	{
		maze_4 = maze_5_dimensions;
		maze_4.x += (maze_5_dimensions.w*-1) + map_block_width;
		SDL_Rect* ptr_area_maze_4 = &maze_4;
		ptr_maze4 = ptr_components_factory->SpawnLevelComponent(levco_maze, ptr_area_maze_4);
		ptr_maze4->SetBorderState(east, false);
		ptr_maze4->GenerateMaze();
	}
	else if (my_number == 5)
	{
		maze_5 = maze_5_dimensions;
		SDL_Rect* ptr_area_maze_5 = &maze_5;
		printf("Spawning maze in area: x: %d, y: %d, w: %d, h: %d.\n", ptr_area_maze_5->x, ptr_area_maze_5->y, ptr_area_maze_5->w, ptr_area_maze_5->h);
		ptr_maze5 = ptr_components_factory->SpawnLevelComponent(levco_maze, ptr_area_maze_5);
		ptr_maze5->GenerateMaze();
	}
	else if (my_number == 6)
	{
		maze_6 = maze_5_dimensions;
		maze_6.x += maze_5_dimensions.w - map_block_width;
		SDL_Rect* ptr_area_maze_6 = &maze_6;
		ptr_maze6 = ptr_components_factory->SpawnLevelComponent(levco_maze, ptr_area_maze_6);
		ptr_maze6->SetBorderState(west, false);
		ptr_maze6->GenerateMaze();
	}
	else if (my_number == 7)
	{
		maze_7 = maze_5_dimensions;
		maze_7.x += (maze_5_dimensions.w*-1) + map_block_width;
		maze_7.y += (maze_5_dimensions.h) - map_block_height;
		SDL_Rect* ptr_area_maze_7 = &maze_7;
		ptr_maze7 = ptr_components_factory->SpawnLevelComponent(levco_maze, ptr_area_maze_7);
		ptr_maze7->SetBorderState(east, false);
		ptr_maze7->SetBorderState(north, false);
		ptr_maze7->GenerateMaze();
	}
	else if (my_number == 8)
	{
		maze_8 = maze_5_dimensions;
		maze_8.y += (maze_5_dimensions.h) - map_block_height;
		SDL_Rect* ptr_area_maze_8 = &maze_8;
		ptr_maze8 = ptr_components_factory->SpawnLevelComponent(levco_maze, ptr_area_maze_8);
		ptr_maze8->SetBorderState(north, false);
		ptr_maze8->GenerateMaze();
	}
	else if (my_number == 9)
	{
		maze_9 = maze_5_dimensions;
		maze_9.y += (maze_5_dimensions.h * 2) - map_block_height;
		maze_9.x += maze_5_dimensions.w - map_block_width;
		SDL_Rect* ptr_area_maze_9 = &maze_9;
		ptr_maze9 = ptr_components_factory->SpawnLevelComponent(levco_maze, ptr_area_maze_9);
		ptr_maze9->SetBorderState(north, false);
		ptr_maze9->SetBorderState(west, false);
		ptr_maze9->GenerateMaze();
	}
	else
	{
		printf("Such maze number does not exist and cannot be generated!\n");
		throw std::invalid_argument("Such maze number does not exist!\n");
	}
}