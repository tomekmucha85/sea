#ifndef LEVEL_NINE_MAZES_HPP
#define LEVEL_NINE_MAZES_HPP
#include <Level.hpp>
class LevelNineMazes : public Level
{
    private:
		int maze_rows_count = 0;
		int maze_columns_count = 0;

		SDL_Rect** mazes_areas[9];

		int character_offset_x = 50;
		int character_offset_y = 50;

		LevelComponent* ptr_border_triggers = nullptr;
		Creature* ptr_trigger_south;
		Creature* ptr_trigger_north;

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
		SDL_Rect maze_1_area = {};
		SDL_Rect maze_2_area = {};
		SDL_Rect maze_3_area = {};
		SDL_Rect maze_4_area = {};
		SDL_Rect maze_5_area = {};
		SDL_Rect maze_6_area = {};
		SDL_Rect maze_7_area = {};
		SDL_Rect maze_8_area = {};
		SDL_Rect maze_9_area = {};


		LevelComponent* ptr_maze_to_remove1 = nullptr;
		LevelComponent* ptr_maze_to_remove2 = nullptr;
		LevelComponent* ptr_maze_to_remove3 = nullptr;
		LevelComponent* ptr_maze1 = nullptr;
		LevelComponent* ptr_maze2 = nullptr;
		LevelComponent* ptr_maze3 = nullptr;
		LevelComponent* ptr_maze4 = nullptr;
		LevelComponent* ptr_current_central_maze = nullptr;
		LevelComponent* ptr_maze6 = nullptr;
		LevelComponent* ptr_maze7 = nullptr;
		LevelComponent* ptr_maze8 = nullptr;
		LevelComponent* ptr_maze9 = nullptr;

		//Defining entry in dictionary
		std::string signal_for_regenerating_northern_row = "generate mazes north";
		std::string signal_for_regenerating_southern_row = "generate mazes south";
		std::string signal_for_regenerating_eastern_column = "generate mazes east";
		std::string signal_for_regenerating_western_column = "generate mazes west";

    public:
	    LevelNineMazes(int my_cols_count, int my_rows_count);
		void SetMazeRowsCount(int rows_num);
		void SetMazeColsCount(int cols_num);
		SDL_Rect CalculateInitialCentralMazeArea();
		LevelComponent* SpawnInitialCentralMaze();
		void DeleteMazeNumber(int my_number);
		void DeleteNotNeededMazes();
		void GenerateMazeNumber(int my_number);
		void SetCurrentCentralMaze( LevelComponent* ptr_my_maze);
		LevelComponent* TellCurrentCentralMaze();
		void MoveWorldNorth();
		void MoveWorldSouth();
		void MoveWorldEast();
		void MoveWorldWest();
		void GenerateTrigger(Directions my_direction, SDL_Rect offset_from_component_border = {0,0,0,0});

		//###################
		// COMMON LAMBDAS
		//###################

		std::function<void()> ptr_func_trigger_north = [this]()
		{
			printf("TRIGGER!\n");
			this->MoveWorldNorth();
			this->GenerateTrigger(north);
			//Creating new south trigger just behind main character's back.
			this->GenerateTrigger(south, {0,character_offset_y,0,0});
		};

		std::function<void()> ptr_func_trigger_south = [this]()
		{
			printf("TRIGGER!\n");
			this->MoveWorldSouth();
			this->GenerateTrigger(south);
			//Creating new south trigger just behind main character's back.
			this->GenerateTrigger(north, { 0,-character_offset_y,0,0 });
		};

		std::function<void()> ptr_func_trigger_east = [this]()
		{
			printf("TRIGGER!\n");
			this->MoveWorldEast();
			this->GenerateTrigger(east);
			//Creating new south trigger just behind main character's back.
			this->GenerateTrigger(west, { -character_offset_x,0,0,0 });
		};

		std::function<void()> ptr_func_trigger_west = [this]()
		{
			printf("TRIGGER!\n");
			this->MoveWorldWest();
			this->GenerateTrigger(west);
			//Creating new south trigger just behind main character's back.
			this->GenerateTrigger(east, { character_offset_x,0,0,0 });
		};
};
#endif // LEVEL_NINE_MAZES_HPP
