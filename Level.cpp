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
	std::map<LevelComponentType, std::vector<LevelComponent*>>* ptr_level_components = &level_component_types_vs_level_components;
	return ptr_level_components;
}

std::vector<LevelComponent*>* Level::TellPointerToSpecificComponentTypeArray(LevelComponentType my_type)
{
	std::vector<LevelComponent*>* result = nullptr;
	result = &(level_component_types_vs_level_components[my_type]);
	return result;
}

FactorySpawningLevelComponents* Level::CreateComponentsFactory()
{
	ptr_components_factory = new FactorySpawningLevelComponents(TellPointerToComponentsArray());
	return ptr_components_factory;
}

CreatureType Level::PickRandomObjectFromGiven(std::vector<CreatureType> my_creatures)
{
	int vector_size = static_cast<int>(my_creatures.size());
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

//*****************
//REMOVING COMPONENTS
//*****************

void Level::RemoveLevelComponent(LevelComponent* ptr_my_component)
{
	if (ptr_my_component != nullptr)
	{
		//Removing pointer from level components array
		LevelComponentType type = ptr_my_component->my_type;
		std::vector<LevelComponent*>* ptr_proper_array = TellPointerToSpecificComponentTypeArray(type);
		ptr_proper_array->erase(std::remove(ptr_proper_array->begin(),
			ptr_proper_array->end(), ptr_my_component), ptr_proper_array->end());
		//Deleting level component.
		delete ptr_my_component;
	}
	else
	{
		printf("Potinter to component is nullptr. Nothing to delete.\n");
	}
}

//**************
//CYCLIC ACTIONS
//**************

void Level::PerformCyclicActions()
{
	//Actions on Level plane
	for (std::function<void(Level*)> action : cyclic_actions)
	{
		action(this);
	}

	//Actions on Level Components plane
	MakeLevelComponentsPerformCyclicActions();
}

void Level::MakeLevelComponentsPerformCyclicActions()
{
	for (std::pair<LevelComponentType, std::vector<LevelComponent*>> element : level_component_types_vs_level_components)
	{
		std::vector<LevelComponent*> my_level_components = element.second;
		for (LevelComponent* ptr_my_level_component : my_level_components)
		{
			ptr_my_level_component->PerformCyclicActions();
		}
	}
}

std::vector<Creature*> Level::FindHeroColissionsInGivenComponent(LevelComponent* ptr_my_component, bool check_only_obstacles)
{
	return ptr_my_component->FindCollisionsWithMainCharacter(check_only_obstacles);
}

void Level::PerformActionsForTriggersHitByHero(LevelComponent* ptr_component_with_triggers)
{
	std::vector<std::string> received_signals = {};
	received_signals = ptr_component_with_triggers->RunTriggersHitByHero();
	for (std::string received_signal : received_signals)
	{
		signals_vs_events[received_signal]();
	}
}

//************
//RENDERING
//************

void Level::RenderAllPresentCreatures()
{
	// Such map should be automatically ordered by ascending key values
	std::map<int, std::vector<Creature*>> render_layers_vs_creatures = {};

	//Order creatures by their render layer
	for (std::pair<LevelComponentType, std::vector<LevelComponent*>> element : level_component_types_vs_level_components)
	{
		std::vector<LevelComponent*> my_level_components = element.second;
		for (LevelComponent* ptr_my_level_component : my_level_components)
		{
			for (Creature* ptr_member_creature : *ptr_my_level_component->TellPtrToCreaturesArray())
			{
				if (ptr_member_creature->AmIVisible())
				{
					render_layers_vs_creatures[ptr_member_creature->TellRenderLayer()].push_back(ptr_member_creature);
				}
			}
		}
	}

	//printf("%d creatures to render.\n", static_cast<int>(render_layers_vs_creatures.size()));

	//Render ordered creatures
	for (std::pair<int, std::vector<Creature*>> element : render_layers_vs_creatures)
	{
		std::vector<Creature*> creatures_to_render = element.second;
		for (Creature* ptr_creature_to_render : creatures_to_render)
		{
			RenderCreatureVisualComponent(ptr_creature_to_render);
		}
	}
}

void Level::RenderCreatureVisualComponent(Creature* ptr_my_creature)
{
	//#TODO przerobiæ na visual component

	if (ptr_my_creature == nullptr)
	{
		printf("Cannot render nullptr!\n");
		throw std::invalid_argument("Cannot render nullptr!\n");
	}

	if (ptr_my_creature->ptr_creature_sprite != nullptr)
	{
		ptr_my_creature->ptr_creature_sprite->Render();
		//printf("Rendered sprite.\n");
	}
	else if (ptr_my_creature->ptr_creature_vector != nullptr)
	{
		ptr_my_creature->ptr_creature_vector->Render();
		//printf("Rendered vector.\n");
	}
	else
	{
		printf("This creature has nothing to render!\n");
		throw "This creature has nothing to render!\n";
	}
}
