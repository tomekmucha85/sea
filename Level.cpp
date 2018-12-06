#include <Level.hpp>

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
	printf("Current hero is %p.\n", ptr_my_hero);
}

//**************
//CYCLIC ACTIONS
//**************

void Level::PerformCyclicActions()
{
	for (std::function<void(Level*)> action : cyclic_actions)
	{
		action(this);
	}
}


std::vector<Creature*> Level::FindHeroColissionsInGivenComponent(LevelComponent* ptr_my_component, bool check_only_obstacles)
{
	//printf("Will look for collisions in component %p, with creature array %p.\n", ptr_my_component, ptr_my_component->TellPtrToCreaturesArray());
	//printf("Pointer to current hero: %p.\n", ptr_hero);
	std::vector<Creature*> colliding_creatures = {};
	colliding_creatures = ptr_hero->FindCollisionsInSet(ptr_my_component->TellPtrToCreaturesArray(), check_only_obstacles);
	return colliding_creatures;
}

void Level::RunTriggersHitByHero(LevelComponent* ptr_component_with_triggers)
{
	std::vector<Creature*> hit_triggers = FindHeroColissionsInGivenComponent(ptr_component_with_triggers, false);
	if (hit_triggers.size() > 0)
	{
		printf("A trigger was hit!\n");
		for (Creature* trigger : hit_triggers)
		{
			trigger->FireEvent();
		}
	}
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
				if(ptr_member_creature->ptr_creature_sprite != nullptr)
				{ 
					ptr_member_creature->ptr_creature_sprite->Render();
					//printf("Rendered sprite.\n");
				}
				else if (ptr_member_creature->ptr_creature_vector != nullptr)
				{
					//printf("Will render vector.\n");
					ptr_member_creature->ptr_creature_vector->Render();
					//printf("Rendered vector.\n");
				}
				else
				{
					printf("This creature has nothing to render!\n");
					throw "This creature has nothing to render!\n";
				}
			}
		}
	}
}
