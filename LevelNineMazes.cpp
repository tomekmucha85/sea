#include <LevelNineMazes.hpp>

LevelNineMazes::LevelNineMazes(int my_cols_count, int my_rows_count) : Level()
{

	//Core part generation
    // #TODO - uwspólniæ?
	SDL_Rect core_area = { 0,0,0,0 };
	SDL_Rect* ptr_core_area = &core_area;
	LevelComponent* ptr_core = ptr_components_factory->SpawnLevelComponent(levco_core, 0, ptr_core_area);
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
	LevelComponent* ptr_event_triggers = ptr_components_factory->SpawnLevelComponent(levco_test, 0);

    // Adding event trigger
	std::function<void()> ptr_func = [] {printf("TRIGGER!\n"); };
	SDL_Rect event_area = { 0,0,800,10 };
	SDL_Rect* ptr_event_area = &event_area;
	ptr_event_triggers->AddCreature(cre_event_trigger, ptr_event_area, merge, ptr_func);

	//Adding cyclic action to fire triggers hit by hero
	//std::function<void()> ptr_test_action = [] {printf("ACTION PERFORMED!\n"); };
	std::function<void(Level*)> func_fire_triggers = [=] (Level* ptr_level) 
	{ 
		std::map<LevelComponentType, std::vector<LevelComponent*>>* ptr_components_array = ptr_level->TellPointerToComponentsArray();
		std::vector<LevelComponent*> ptr_levco_test_vector = ptr_components_array->operator[](levco_test);
		LevelComponent* ptr_levco_test_containing_triggers = ptr_levco_test_vector[0];
		ptr_level->RunTriggersHitByHero(ptr_levco_test_containing_triggers);
	};

	cyclic_actions.push_back(func_fire_triggers);


	//Mazes part
	// #TODO ucywilizowaæ

	// #TODO definiowaæ tylko w jednym miejscu
	int map_block_width = LevelComponent::map_block_width;
	int map_block_height = LevelComponent::map_block_height;

	SetMazeRowsCount(my_rows_count);
	SetMazeColsCount(my_cols_count);
	SDL_Rect dimensions = LevelComponentMaze::CalculateMazeDimensions(maze_columns_count,maze_rows_count);

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

	Maze_1 = dimensions;
	Maze_1.x = (dimensions.w*-1) + map_block_width;
	Maze_1.y = (dimensions.h*-1) + map_block_height;
	SDL_Rect* ptr_area_maze_1 = &Maze_1;

	Maze_2 = dimensions;
	Maze_2.y = (dimensions.h*-1) + map_block_height;
	SDL_Rect* ptr_area_maze_2 = &Maze_2;

	Maze_3 = dimensions;
	Maze_3.x = dimensions.w - map_block_width;
	Maze_3.y = (dimensions.h*-1) + map_block_height;
	SDL_Rect* ptr_area_maze_3 = &Maze_3;

	Maze_4 = dimensions;
	Maze_4.x = (dimensions.w*-1) + map_block_width;
	SDL_Rect* ptr_area_maze_4 = &Maze_4;

	Maze_5 = dimensions;
	SDL_Rect* ptr_area_maze_5 = &Maze_5;

	Maze_6 = dimensions;
	Maze_6.x = dimensions.w - map_block_width;
	SDL_Rect* ptr_area_maze_6 = &Maze_6;

	Maze_7 = dimensions;
	Maze_7.x = (dimensions.w*-1) + map_block_width;
	Maze_7.y = (dimensions.h) - map_block_height;
	SDL_Rect* ptr_area_maze_7 = &Maze_7;

	Maze_8 = dimensions;
	Maze_8.y = (dimensions.h) - map_block_height;
	SDL_Rect* ptr_area_maze_8 = &Maze_8;

	Maze_9 = dimensions;
	Maze_9.y = (dimensions.h * 2) - map_block_height;
	Maze_9.x = dimensions.w - map_block_width;
	SDL_Rect* ptr_area_maze_9 = &Maze_9;

	LevelComponent* ptr_maze1 = ptr_components_factory->SpawnLevelComponent(levco_maze, 0, ptr_area_maze_1);
	LevelComponent* ptr_maze2 = ptr_components_factory->SpawnLevelComponent(levco_maze, 1, ptr_area_maze_2);
	LevelComponent* ptr_maze3 = ptr_components_factory->SpawnLevelComponent(levco_maze, 2, ptr_area_maze_3);
	LevelComponent* ptr_maze4 = ptr_components_factory->SpawnLevelComponent(levco_maze, 3, ptr_area_maze_4);
	LevelComponent* ptr_maze5 = ptr_components_factory->SpawnLevelComponent(levco_maze, 4, ptr_area_maze_5);
	LevelComponent* ptr_maze6 = ptr_components_factory->SpawnLevelComponent(levco_maze, 5, ptr_area_maze_6);
	LevelComponent* ptr_maze7 = ptr_components_factory->SpawnLevelComponent(levco_maze, 6, ptr_area_maze_7);
	LevelComponent* ptr_maze8 = ptr_components_factory->SpawnLevelComponent(levco_maze, 7, ptr_area_maze_8);
	LevelComponent* ptr_maze9 = ptr_components_factory->SpawnLevelComponent(levco_maze, 8, ptr_area_maze_9);
	
	ptr_maze1->SetBorderState(east, false);
	ptr_maze1->SetBorderState(south, false);
	ptr_maze2->SetBorderState(south, false);
	ptr_maze3->SetBorderState(south, false);
	ptr_maze3->SetBorderState(west, false);
	ptr_maze4->SetBorderState(east,false);
	ptr_maze6->SetBorderState(west, false);
	ptr_maze7->SetBorderState(east, false);
	ptr_maze7->SetBorderState(north, false);
	ptr_maze8->SetBorderState(north, false);
	ptr_maze9->SetBorderState(north, false);
	ptr_maze9->SetBorderState(west, false);

	ptr_maze1->GenerateMaze();
	ptr_maze2->GenerateMaze();
	ptr_maze3->GenerateMaze();
	ptr_maze4->GenerateMaze();
	ptr_maze5->GenerateMaze();
	ptr_maze6->GenerateMaze();
	ptr_maze7->GenerateMaze();
	ptr_maze8->GenerateMaze();
	ptr_maze9->GenerateMaze();
}

void LevelNineMazes::SetMazeRowsCount(int rows_num)
{
	maze_rows_count = rows_num;
}

void LevelNineMazes::SetMazeColsCount(int cols_num)
{
    maze_columns_count = cols_num;
}
