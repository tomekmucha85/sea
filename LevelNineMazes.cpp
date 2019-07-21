#include <LevelNineMazes.hpp>

LevelNineMazes::LevelNineMazes(int my_cols_count, int my_rows_count) : Level()
{

	//Adding component for event triggers
	ptr_border_triggers = ptr_components_factory->SpawnLevelComponent(levco_triggers);
	printf("Triggers component address: %p.\n", ptr_border_triggers);

	//Trigger to win level
    std::string signal_to_win = "winning";
    PreciseRect event_win_area = {1000,1000,100,100};
    Creature* ptr_winning_trigger = AddTriggerUsingDefaultComponent(event_win_area, signal_to_win);
    signals_vs_events[signal_to_win] = ptr_func_win;
    //Blue
    ptr_winning_trigger->TellMainVisualComponent()->SetColor({ 0,0,255,255 });
	Coordinates win_center = ptr_winning_trigger->TellMainVisualComponent()->TellCenter();

	//Trigger to loose level
	std::string signal_to_lose = "losing";
	PreciseRect event_lose_area = { 700,1000,100,100 };
	Creature* ptr_losing_trigger = AddTriggerUsingDefaultComponent(event_lose_area, signal_to_lose);
	signals_vs_events[signal_to_lose] = ptr_func_lose;
	//Red
	ptr_losing_trigger->TellMainVisualComponent()->SetColor({ 255,0,0,255 });
	Coordinates loose_center = ptr_losing_trigger->TellMainVisualComponent()->TellCenter();

	//MAIN CHARACTER BEHAVIOR
	//Creature::ptr_current_main_charater->SetBehaviorPattern(beh_pat_death_magnetic, ptr_losing_trigger);
	Creature::ptr_current_main_charater->SetBehaviorPattern(beh_pat_stalker);

	//How many map blocks will have a maze segment?
	SetMazeRowsCount(my_rows_count);
	SetMazeColsCount(my_cols_count);

	/*
       MAZES DISTRIBUTION:

	            T      T
				R      R
				I      I
				G      G
	            G      G
				E      E
				R      R
	    
		        W      E
				E      A
				S      S
	   			T      T
           ___________________
           |     |     |     |
		   |  1  |  2  |  3  |
		   |     |     |     |
           ------------------- TRIGGER_NORTH
		   |     |     |     |
		   |  4  |  5  |  6  |
		   |     |     |     |
		   ------------------- TRIGGER_SOUTH
		   |     |     |     |
		   |  7  |  8  |  9  |
		   |     |     |     |
		   -------------------

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

	//Creating trigger
	GenerateTrigger(north);
	GenerateTrigger(south);
	GenerateTrigger(east);
	GenerateTrigger(west);
	/*printf("Current mazes setup: 1: %p\n2: %p\n3: %p\n4: %p\n5: %p\n6: %p\n7: %p\n8: %p\n9: %p\n",
		ptr_maze1, ptr_maze2, ptr_maze3, ptr_maze4, ptr_current_central_maze, ptr_maze6, ptr_maze7, ptr_maze8, ptr_maze9);*/

	SpawnCarriers(20);

}

std::pair<Coordinates, Coordinates> LevelNineMazes::CalculateLevelConstraints()
{
	//Returns coordinates of top-left and bottom-right level corners.
	//(Respective corners of maze 1 and 9)
	std::pair<Coordinates, Coordinates> result;
	if (ptr_maze1 == nullptr || ptr_maze9 == nullptr)
	{
		printf("Could not calculate constraints.\n");
		throw std::runtime_error("Could not calculate constraints!\n");
	}
	else
	{
		PreciseRect maze_1_area = ptr_maze1->TellComponentArea();
		result.first = { maze_1_area.x, maze_1_area.y };
		PreciseRect maze_9_area = ptr_maze9->TellComponentArea();
		result.second = {maze_9_area.x+maze_9_area.w, maze_9_area.y+maze_9_area.h};
	}
	printf("Calculated following level constraints: TOP-LEFT: x: %f y: %f BOTTOM-RIGHT: x: %f y: %f\n",
		result.first.x, result.first.y, result.second.x, result.second.y);
	return result;
}

void LevelNineMazes::SpawnCarriers(unsigned int carriers_number)
{
	std::pair<Coordinates, Coordinates> level_constraints = CalculateLevelConstraints();
	int min_carrier_start_point_x = level_constraints.first.x + 1000;
	int min_carrier_start_point_y = level_constraints.first.y + 1000;
	int max_carrier_start_point_x = level_constraints.second.x - 1000;
	int max_carrier_start_point_y = level_constraints.second.y - 1000;

	for (unsigned int i = 0; i < carriers_number; i++)
	{
		bool was_carrier_created = false;
		while (was_carrier_created == false)
		{
			//RANDOM NUMBER IN RANGE:
			//output = min + (rand() % (int)(max - min + 1))

			Coordinates carrier_start_point = 
			{ 
				min_carrier_start_point_x + rand() % (max_carrier_start_point_x - min_carrier_start_point_x + 1),
				min_carrier_start_point_y + rand() % (max_carrier_start_point_y - min_carrier_start_point_y + 1)
			};

			double carrier_destination_x = 0;
			double carrier_destination_y = 0;
			int min_offset = 1000;
			int max_offset = 2000;
			if (carrier_start_point.x >= 0)
			{
				carrier_destination_x = 
					carrier_start_point.x + (min_offset*-1 + rand() % (max_offset - min_offset + 1))*-1;
			}
			else
			{
				carrier_destination_x = 
					carrier_start_point.x + min_offset + rand() % (max_offset - min_offset + 1);
			}

			if (carrier_start_point.y >= 0)
			{
				carrier_destination_y = 
					carrier_start_point.y + (min_offset*-1 + rand() % (max_offset - min_offset + 1))*-1;
			}
			else
			{
				carrier_destination_y =
					carrier_start_point.y + min_offset + rand() % (max_offset - min_offset + 1);
			}
			Coordinates carrier_destination = 
			{ 
				carrier_destination_x, carrier_destination_y
			};
			Creature* ptr_carrier = ptr_initial_core_component->AddCreature(cre_carrier_a, &carrier_start_point, safe);
			if (ptr_carrier != nullptr)
			{
				was_carrier_created = true;
				ptr_carrier->SetBehaviorMode(beh_go_towards_fixed_point, &carrier_destination);
			}
			else
			{
				printf("Carrier reroll.\n");
			}
		}
	}
}

void LevelNineMazes::FinishLevel(LevelEnding my_ending)
{
	Pause();
	if (my_ending == victory)
	{
		printf("You've beaten nine mazes!\n");
	}
	else if (my_ending == defeat)
	{
		printf("You've been beaten.\n");
	}
	else
	{
		printf("Some other ending!\n");
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

void LevelNineMazes::SetCurrentCentralMaze(LevelComponent* ptr_my_maze)
{
	ptr_current_central_maze = ptr_my_maze;
}
LevelComponent* LevelNineMazes::TellCurrentCentralMaze()
{
	return ptr_current_central_maze;
}

void LevelNineMazes::DeleteNotNeededMazes()
{
	RemoveLevelComponent(ptr_maze_to_remove1);
	RemoveLevelComponent(ptr_maze_to_remove2);
	RemoveLevelComponent(ptr_maze_to_remove3);
	ptr_maze_to_remove1 = nullptr;
	ptr_maze_to_remove2 = nullptr;
	ptr_maze_to_remove3 = nullptr;
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
		RemoveLevelComponent(ptr_current_central_maze);
		ptr_current_central_maze = nullptr;
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

PreciseRect LevelNineMazes::CalculateInitialCentralMazeArea()
{
	int map_block_width = LevelComponent::map_block_width;
	int map_block_height = LevelComponent::map_block_height;
	PreciseRect central_maze_initial_coordinates = LevelComponentMaze::CalculateMazeDimensions(maze_columns_count, maze_rows_count);
	return central_maze_initial_coordinates;
}

LevelComponent* LevelNineMazes::SpawnInitialCentralMaze()
{
	PreciseRect maze_area = CalculateInitialCentralMazeArea();
	printf("Spawning maze in area: x: %f, y: %f, w: %f, h: %f.\n", maze_area.x, maze_area.y, maze_area.w, maze_area.h);
	LevelComponent* ptr_central_maze = ptr_components_factory->SpawnLevelComponent(levco_maze, maze_area);
	printf("WILL GENERATE INITIAL CENTRAL MAZE.\n");
	ptr_central_maze->GenerateMaze();
	SetCurrentCentralMaze(ptr_central_maze);
	ptr_current_central_maze = ptr_central_maze;
	return ptr_central_maze;
}

void LevelNineMazes::GenerateMazeNumber(int my_number)
{
	int map_block_width = LevelComponent::map_block_width;
	int map_block_height = LevelComponent::map_block_height;
	
	PreciseRect current_central_maze_coordinates;

	//If there's no central maze for reference - spawn one!
	if (ptr_current_central_maze == nullptr)
	{
		printf("No central maze for reference. Spawning maze 5.\n");
		SpawnInitialCentralMaze();
	}

	current_central_maze_coordinates = ptr_current_central_maze->TellComponentArea();
	/*printf("Current central component area: x: %f, y: %f, w: %f, h: %f.\n", current_central_maze_coordinates.x,
		current_central_maze_coordinates.y, current_central_maze_coordinates.w, current_central_maze_coordinates.h);*/

	if (my_number == 1)
	{
		maze_1_area = current_central_maze_coordinates;
		maze_1_area.x += (current_central_maze_coordinates.w*-1) + map_block_width;
		maze_1_area.y += (current_central_maze_coordinates.h*-1) + map_block_height;
		ptr_maze1 = ptr_components_factory->SpawnLevelComponent(levco_maze, maze_1_area);
		//printf("WILL GENERATE MAZE %d.\n", my_number);
		if (ptr_maze2 != nullptr)
		{
			if (ptr_maze2->TellBorderState(west) == true)
			{
				ptr_maze1->SetBorderState(east, false);
			}
		}
		else
		{
			ptr_maze1->SetBorderState(east, false);
		}
		if (ptr_maze4 != nullptr)
		{
			if (ptr_maze4->TellBorderState(north) == true)
			{
				ptr_maze1->SetBorderState(south, false);
			}
		}
		else
		{
			ptr_maze1->SetBorderState(south, false);
		}
		ptr_maze1->GenerateMaze();
	}
	else if (my_number == 2)
	{

		maze_2_area = current_central_maze_coordinates;
		maze_2_area.y += (current_central_maze_coordinates.h*-1) + map_block_height;
		ptr_maze2 = ptr_components_factory->SpawnLevelComponent(levco_maze, maze_2_area);
		//printf("WILL GENERATE MAZE %d.\n", my_number);
		if (ptr_current_central_maze != nullptr)
		{
			if (ptr_current_central_maze->TellBorderState(north) == true)
			{
				ptr_maze2->SetBorderState(south, false);
			}
		}
		else
		{
			ptr_maze2->SetBorderState(south, false);
		}
		ptr_maze2->SetBorderState(south, false);
		ptr_maze2->GenerateMaze();
	}
	else if (my_number == 3)
	{
		maze_3_area = current_central_maze_coordinates;
		maze_3_area.x += current_central_maze_coordinates.w - map_block_width;
		maze_3_area.y += (current_central_maze_coordinates.h*-1) + map_block_height;
		ptr_maze3 = ptr_components_factory->SpawnLevelComponent(levco_maze, maze_3_area);
		//printf("WILL GENERATE MAZE %d.\n", my_number);
		if (ptr_maze6 != nullptr)
		{
			if (ptr_maze6->TellBorderState(north) == true)
			{
				ptr_maze3->SetBorderState(south, false);
			}
		}
		else
		{
			ptr_maze3->SetBorderState(south, false);
		}
		if (ptr_maze2 != nullptr)
		{
			if (ptr_maze2->TellBorderState(east) == true)
			{
				ptr_maze3->SetBorderState(west, false);
			}
		}
		else
		{
			ptr_maze3->SetBorderState(west, false);
		}
		ptr_maze3->GenerateMaze();
	}
	else if (my_number == 4)
	{
		maze_4_area = current_central_maze_coordinates;
		maze_4_area.x += (current_central_maze_coordinates.w*-1) + map_block_width;
		ptr_maze4 = ptr_components_factory->SpawnLevelComponent(levco_maze, maze_4_area);
		//printf("WILL GENERATE MAZE %d.\n", my_number);
		if (ptr_current_central_maze != nullptr)
		{
			if (ptr_current_central_maze->TellBorderState(west) == true)
			{
				ptr_maze4->SetBorderState(east, false);
			}
		}
		else
		{
			ptr_maze4->SetBorderState(east, false);
		}
		ptr_maze4->GenerateMaze();
	}
	else if (my_number == 5)
	{
		maze_5_area = current_central_maze_coordinates;
		ptr_current_central_maze = ptr_components_factory->SpawnLevelComponent(levco_maze, maze_5_area);
		//printf("WILL GENERATE MAZE %d.\n", my_number);
		ptr_current_central_maze->GenerateMaze();
	}
	else if (my_number == 6)
	{
		maze_6_area = current_central_maze_coordinates;
		maze_6_area.x += current_central_maze_coordinates.w - map_block_width;
		ptr_maze6 = ptr_components_factory->SpawnLevelComponent(levco_maze, maze_6_area);
		//printf("WILL GENERATE MAZE %d.\n", my_number);
		if (ptr_current_central_maze != nullptr)
		{
			if (ptr_current_central_maze->TellBorderState(east) == true)
			{
				ptr_maze6->SetBorderState(west, false);
			}
		}
		else
		{
			ptr_maze6->SetBorderState(west, false);
		}
		ptr_maze6->GenerateMaze();
	}
	else if (my_number == 7)
	{
		maze_7_area = current_central_maze_coordinates;
		maze_7_area.x += (current_central_maze_coordinates.w*-1) + map_block_width;
		maze_7_area.y += (current_central_maze_coordinates.h) - map_block_height;
		ptr_maze7 = ptr_components_factory->SpawnLevelComponent(levco_maze, maze_7_area);
		//printf("WILL GENERATE MAZE %d.\n", my_number);
		if (ptr_maze8 != nullptr)
		{
			if (ptr_maze8->TellBorderState(west) == true)
			{
				ptr_maze7->SetBorderState(east, false);
			}
		}
		else
		{
			ptr_maze7->SetBorderState(east, false);
		}
		if (ptr_maze4 != nullptr)
		{
			if (ptr_maze4->TellBorderState(south) == true)
			{
				ptr_maze7->SetBorderState(north, false);
			}
		}
		else
		{
			ptr_maze7->SetBorderState(north, false);
		}
		ptr_maze7->GenerateMaze();
	}
	else if (my_number == 8)
	{
		maze_8_area = current_central_maze_coordinates;
		maze_8_area.y += current_central_maze_coordinates.h - map_block_height;
		ptr_maze8 = ptr_components_factory->SpawnLevelComponent(levco_maze, maze_8_area);
		//printf("WILL GENERATE MAZE %d.\n", my_number);
		if (ptr_current_central_maze != nullptr)
		{
			if (ptr_current_central_maze->TellBorderState(south) == true)
			{
				ptr_maze8->SetBorderState(north, false);
			}
		}
		else
		{
			ptr_maze8->SetBorderState(north, false);
		}
		ptr_maze8->GenerateMaze();
	}
	else if (my_number == 9)
	{
		maze_9_area = current_central_maze_coordinates;
		maze_9_area.y += current_central_maze_coordinates.h - map_block_height;
		maze_9_area.x += current_central_maze_coordinates.w - map_block_width;
		ptr_maze9 = ptr_components_factory->SpawnLevelComponent(levco_maze, maze_9_area);
		//printf("WILL GENERATE MAZE %d.\n", my_number);
		if (ptr_maze6 != nullptr)
		{
			if (ptr_maze6->TellBorderState(south) == true)
			{
				ptr_maze9->SetBorderState(north, false);
			}
		}
		else
		{
			ptr_maze9->SetBorderState(north, false);
		}
		if (ptr_maze8 != nullptr)
		{
			if (ptr_maze8->TellBorderState(east) == true)
			{
				ptr_maze9->SetBorderState(west, false);
			}
		}
		else
		{
			ptr_maze9->SetBorderState(west, false);
		}
		ptr_maze9->GenerateMaze();
	}
	else
	{
		printf("Such maze number does not exist and cannot be generated!\n");
		throw std::invalid_argument("Such maze number does not exist!\n");
	}
}

void LevelNineMazes::MoveWorldNorth()
{
    //Mark mazes to remove
	ptr_maze_to_remove1 = ptr_maze7;
	ptr_maze_to_remove2 = ptr_maze8;
	ptr_maze_to_remove3 = ptr_maze9;
	//printf("Mazes to remove: maze 7: %p, maze 8: %p, maze 9: %p.\n", ptr_maze_to_remove1, ptr_maze_to_remove2, ptr_maze_to_remove3);
	//Shift maze pointers upward
	ptr_maze7 = ptr_maze4;
	ptr_maze8 = ptr_current_central_maze;
	ptr_maze9 = ptr_maze6;
    ptr_maze4 = ptr_maze1;
	ptr_current_central_maze = ptr_maze2;
    ptr_maze6 = ptr_maze3;
    //Generate new top row of mazes
    GenerateMazeNumber(1);
    GenerateMazeNumber(2);
    GenerateMazeNumber(3);
    //Delete bottom row of mazes
	DeleteNotNeededMazes();
	/*printf("Current mazes setup: 1: %p\n2: %p\n3: %p\n4: %p\n5: %p\n6: %p\n7: %p\n8: %p\n9: %p\n",
		ptr_maze1, ptr_maze2, ptr_maze3, ptr_maze4, ptr_current_central_maze, ptr_maze6, ptr_maze7, ptr_maze8, ptr_maze9);*/
	//Creating new north trigger
	GenerateTrigger(north);
	//Deleting old triggers
	DeleteTrigger(south);
	DeleteTrigger(east);
	DeleteTrigger(west);
	//Creating new south trigger just behind main character's back.
	GenerateTrigger(south, { 0,character_offset_y,0,0 });
	GenerateTrigger(west);
	GenerateTrigger(east);
}

void LevelNineMazes::MoveWorldSouth()
{
	//Mark mazes to remove
	ptr_maze_to_remove1 = ptr_maze1;
	ptr_maze_to_remove2 = ptr_maze2;
	ptr_maze_to_remove3 = ptr_maze3;
	/*printf("Mazes to remove: maze 1: %p, maze 2: %p, maze 3: %p.\n", ptr_maze_to_remove1, ptr_maze_to_remove2, ptr_maze_to_remove3);*/
	//Shift maze pointers downward
	ptr_maze1 = ptr_maze4;
	ptr_maze2 = ptr_current_central_maze;
	ptr_maze3 = ptr_maze6;
	ptr_maze4 = ptr_maze7;
	ptr_current_central_maze = ptr_maze8;
	ptr_maze6 = ptr_maze9;
	//Generate new top row of mazes
	GenerateMazeNumber(7);
	GenerateMazeNumber(8);
	GenerateMazeNumber(9);
	//Delete bottom row of mazes
	DeleteNotNeededMazes();
	/*printf("Current mazes setup: 1: %p\n2: %p\n3: %p\n4: %p\n5: %p\n6: %p\n7: %p\n8: %p\n9: %p\n",
		ptr_maze1, ptr_maze2, ptr_maze3, ptr_maze4, ptr_current_central_maze, ptr_maze6, ptr_maze7, ptr_maze8, ptr_maze9);*/
	//Creating new south trigger
	GenerateTrigger(south);
	//Deleting old triggers
	DeleteTrigger(north);
	DeleteTrigger(east);
	DeleteTrigger(west);
	//Creating new north trigger just behind main character's back.
	GenerateTrigger(north, { 0,-character_offset_y,0,0 });
	GenerateTrigger(west);
	GenerateTrigger(east);

}

void LevelNineMazes::MoveWorldEast()
{
	//Mark mazes to remove
	ptr_maze_to_remove1 = ptr_maze1;
	ptr_maze_to_remove2 = ptr_maze4;
	ptr_maze_to_remove3 = ptr_maze7;
	//printf("Mazes to remove: maze 1: %p, maze 2: %p, maze 3: %p.\n", ptr_maze_to_remove1, ptr_maze_to_remove2, ptr_maze_to_remove3);
	//Shift maze pointers right
	ptr_maze1 = ptr_maze2;
	ptr_maze4 = ptr_current_central_maze;
	ptr_maze7 = ptr_maze8;
	ptr_maze2 = ptr_maze3;
	ptr_current_central_maze = ptr_maze6;
	ptr_maze8 = ptr_maze9;
	//Generate new eastern row of mazes
	GenerateMazeNumber(3);
	GenerateMazeNumber(6);
	GenerateMazeNumber(9);
	//Delete western column of mazes
	DeleteNotNeededMazes();
	/*printf("Current mazes setup: 1: %p\n2: %p\n3: %p\n4: %p\n5: %p\n6: %p\n7: %p\n8: %p\n9: %p\n",
		ptr_maze1, ptr_maze2, ptr_maze3, ptr_maze4, ptr_current_central_maze, ptr_maze6, ptr_maze7, ptr_maze8, ptr_maze9);*/
	//Creating new eastern trigger
	GenerateTrigger(east);
	//Deleting old western trigger
	DeleteTrigger(west);
	DeleteTrigger(north);
	DeleteTrigger(south);
	//Creating new western trigger just behind main character's back.
	GenerateTrigger(west, { - character_offset_x,0,0,0 });
	GenerateTrigger(north);
	GenerateTrigger(south);
}

void LevelNineMazes::MoveWorldWest()
{
	//Mark mazes to remove
	ptr_maze_to_remove1 = ptr_maze3;
	ptr_maze_to_remove2 = ptr_maze6;
	ptr_maze_to_remove3 = ptr_maze9;
	//Shift maze pointers left
	ptr_maze3 = ptr_maze2;
	ptr_maze6 = ptr_current_central_maze;
	ptr_maze9 = ptr_maze8;
	ptr_maze2 = ptr_maze1;
	ptr_current_central_maze = ptr_maze4;
	ptr_maze8 = ptr_maze7;
	//Generate new western row of mazes
	GenerateMazeNumber(1);
	GenerateMazeNumber(4);
	GenerateMazeNumber(7);
	//Delete eastern column of mazes
	DeleteNotNeededMazes();
	//Creating new western trigger
	GenerateTrigger(west);
	//Deleting old triggers
	DeleteTrigger(east);
	DeleteTrigger(north);
	DeleteTrigger(south);
	//Creating new western trigger just behind main character's back.
	GenerateTrigger(east, { character_offset_x,0,0,0 });
	GenerateTrigger(north);
	GenerateTrigger(south);
}

void LevelNineMazes::GenerateTrigger(Directions my_direction, PreciseRect offset_from_component_border)
{
	if (my_direction == north)
	{
		//Creating trigger
		PreciseRect event_area = ptr_current_central_maze->TellComponentEdge(my_direction);
		event_area.x += offset_from_component_border.x;
		event_area.y += offset_from_component_border.y;
		PreciseRect* ptr_event_area = &event_area;
		Creature* ptr_my_trigger_north = ptr_border_triggers->AddCreature(cre_event_trigger, ptr_event_area, merge, signal_for_regenerating_northern_row);
		signals_vs_events[signal_for_regenerating_northern_row] = ptr_func_trigger_north;
		//printf("Added trigger NORTH %p x: %f, y: %f, w: %f, h: %f.\n", ptr_my_trigger_north, event_area.x, event_area.y, event_area.w, event_area.h);
		//Red
		ptr_my_trigger_north->TellMainVisualComponent()->SetColor({ 255,0,0,255 });
		ptr_trigger_north = ptr_my_trigger_north;
	}
	else if (my_direction == south)
	{
		//Creating trigger
		PreciseRect event_area = ptr_current_central_maze->TellComponentEdge(my_direction);
		event_area.x += offset_from_component_border.x;
		event_area.y += offset_from_component_border.y;
		PreciseRect* ptr_event_area = &event_area;
		Creature* ptr_my_trigger_south = ptr_border_triggers->AddCreature(cre_event_trigger, ptr_event_area, merge, signal_for_regenerating_southern_row);
		signals_vs_events[signal_for_regenerating_southern_row] = ptr_func_trigger_south;
		//printf("Added trigger SOUTH %p x: %f, y: %f, w: %f, h: %f.\n", ptr_my_trigger_south, event_area.x, event_area.y, event_area.w, event_area.h);
		//Green
		ptr_my_trigger_south->TellMainVisualComponent()->SetColor({ 0,255,0,255 });
		ptr_trigger_south = ptr_my_trigger_south;
	}
	else if (my_direction == east)
	{
		//Creating trigger
		PreciseRect event_area = ptr_current_central_maze->TellComponentEdge(my_direction);
		event_area.x += offset_from_component_border.x;
		event_area.y += offset_from_component_border.y;
		//printf("Added trigger EAST x: %f, y: %f, w: %f, h: %f.\n", event_area.x, event_area.y, event_area.w, event_area.h);
		PreciseRect* ptr_event_area = &event_area;
		Creature* ptr_my_trigger_east = ptr_border_triggers->AddCreature(cre_event_trigger, ptr_event_area, merge, signal_for_regenerating_eastern_column);
		signals_vs_events[signal_for_regenerating_eastern_column] = ptr_func_trigger_east;
		//Blue
		ptr_my_trigger_east->TellMainVisualComponent()->SetColor({ 0,0,255,255 });
		ptr_trigger_east = ptr_my_trigger_east;
	}
	else if (my_direction == west)
	{
		//Creating trigger
		PreciseRect event_area = ptr_current_central_maze->TellComponentEdge(my_direction);
		event_area.x += offset_from_component_border.x;
		event_area.y += offset_from_component_border.y;
		//printf("Added trigger WEST x: %f, y: %f, w: %f, h: %f.\n", event_area.x, event_area.y, event_area.w, event_area.h);
		PreciseRect* ptr_event_area = &event_area;
		Creature* ptr_my_trigger_west = ptr_border_triggers->AddCreature(cre_event_trigger, ptr_event_area, merge, signal_for_regenerating_western_column);
		signals_vs_events[signal_for_regenerating_western_column] = ptr_func_trigger_west;
		//Black
		ptr_my_trigger_west->TellMainVisualComponent()->SetColor({ 0,0,0,255 });
		ptr_trigger_west = ptr_my_trigger_west;
	}
	else
	{
		printf("Unknown direction!\n");
	    throw "Unknown direction!\n";
    }
}

void LevelNineMazes::DeleteTrigger(Directions my_direction)
{
	if (my_direction == north)
	{
		ptr_border_triggers->RemoveCreature(ptr_trigger_north);
		ptr_trigger_north = nullptr;
	}
	else if (my_direction == south)
	{
		ptr_border_triggers->RemoveCreature(ptr_trigger_south);
		ptr_trigger_south = nullptr;
	}
	else if (my_direction == west)
	{
		ptr_border_triggers->RemoveCreature(ptr_trigger_west);
		ptr_trigger_west = nullptr;
	}
	else if (my_direction == east)
	{
		ptr_border_triggers->RemoveCreature(ptr_trigger_east);
		ptr_trigger_east = nullptr;
	}
	else
	{
		throw "Unknown direction!\n";
	}
}


