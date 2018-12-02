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
	CreateComponentsFactory();
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

CreatureType Level::PickRandomObjectFromGiven(std::vector<CreatureType> my_creatures)
{
	int vector_size = my_creatures.size();
	//srand(time(NULL));
	CreatureType chosen_creature = my_creatures[rand() % vector_size];
	printf("Picked random creature: %d.\n", chosen_creature);
	return chosen_creature;
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

void Level::SetCurrentHero(Creature* ptr_my_hero)
{
    ptr_hero = ptr_my_hero;
}

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
