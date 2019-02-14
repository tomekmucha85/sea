#include <Level.hpp>

//************
//CONSTRUCTORS
//************

Level::Level()
{
	CreateComponentsFactory();
	//Core part generation
	ptr_initial_core_component = ptr_components_factory->SpawnLevelComponent(levco_core);
	//Triggers part generation
	ptr_initial_triggers_component = ptr_components_factory->SpawnLevelComponent(levco_triggers);
	//Navigation grid generation
	ptr_initial_navgrid_component = ptr_components_factory->SpawnLevelComponent(levco_navgrid);
	//Spawning hero if needed
	Creature* ptr_hero = SpawnHero();
	ptr_initial_core_component->AddExistingCreature(ptr_hero);
	//Spawning GUI
	ptr_gui = new GUI();
	//Adding default cyclic actions
	cyclic_actions.push_back(func_fire_triggers);
	cyclic_actions.push_back(func_manage_gui_for_main_character);
	cyclic_actions.push_back(func_check_and_react_if_player_won);
	cyclic_actions.push_back(func_check_and_react_if_player_lost);
	cyclic_actions.push_back(func_connect_nodes);
}

//************
//DESTRUCTOR
//************


Level::~Level()
{
	RemoveAllLevelComponents();
	delete ptr_gui;
	delete ptr_components_factory;
}

//************
//TELLERS
//************

GUI* Level::TellMyGui()
{
	return ptr_gui;
}

//************
//LOADER
//************

void Level::LoadLevel()
{
	;
}

void Level::LoadLevelCreaturesIntoEnvironment()
{
    ;
}

//*****************
//ADDING COMPONENTS
//*****************

//Every component holds its own creatures
//Level traces holds record of its components

Creature* Level::SpawnHero(CreatureType hero_type, Coordinates* ptr_hero_position)
{
	if (Creature::ptr_current_main_charater != nullptr)
	{
		printf("There is an existing main hero Creature. Will not spawn a new one.\n");
		if (ptr_hero_position != nullptr)
		{
			Creature::ptr_current_main_charater->SetPosition(*ptr_hero_position);
		}
		else
		{
			Creature::ptr_current_main_charater->SetPosition(default_hero_start_position);
		}
	}
	else
	{
		printf("There is no hero Creature yet. Will spawn a new one.\n");
		Creature* ptr_new_hero = nullptr;
		if (ptr_hero_position != nullptr)
		{
			ptr_new_hero = ptr_initial_core_component->AddCreature(hero_type, ptr_hero_position, force);
		}
		else
		{
			ptr_new_hero = ptr_initial_core_component->AddCreature(hero_type, &default_hero_start_position, force);
		}
        ptr_new_hero->MakeMeMainCharacter();
	}
	return Creature::ptr_current_main_charater;
}

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
		printf("Pointer to component is nullptr. Nothing to delete.\n");
	}
}

void Level::RemoveAllLevelComponents()
{
	std::map<LevelComponentType, std::vector<LevelComponent*>> level_component_types_vs_level_components_copy = level_component_types_vs_level_components;
	for (std::pair<LevelComponentType, std::vector<LevelComponent*>> element : level_component_types_vs_level_components_copy)
	{
		std::vector<LevelComponent*> my_level_components = element.second;
		for (LevelComponent* ptr_my_level_component : my_level_components)
		{
			RemoveLevelComponent(ptr_my_level_component);
		}
	}
}

//***********************************************************
//ADDING CREATURES - SHORTCUTS USING DEFAULT LEVEL COMPONENTS
//***********************************************************

Creature* Level::AddTriggerUsingDefaultComponent(PreciseRect my_trigger_area, std::string my_trigger_signal)
{
	Creature* result = nullptr;
	if (ptr_initial_triggers_component == nullptr)
	{
		throw std::invalid_argument("Default trigger level component set to nullptr!\n");
	}
	else
	{
		result = ptr_initial_triggers_component->AddCreature(cre_event_trigger, &my_trigger_area, merge, my_trigger_signal);
	}
	return result;
}


Creature* Level::AddNavigationNodeUsingDefaultComponent(Coordinates my_center)
{
	Creature* result = nullptr;
	if (ptr_initial_navgrid_component == nullptr)
	{
		throw std::invalid_argument("Default trigger level component set to nullptr!\n");
	}
	else
	{
		result = ptr_initial_navgrid_component->AddCreature(cre_navgrid_node, &my_center, merge);
	}
	return result;
}

//*******************************************
//WIN SOME LOOSE SOME - IT'S ALL A GAME TO ME
//*******************************************

void Level::Pause()
{
	is_paused = true;
}

void Level::UnPause()
{
	is_paused = false;
}

bool Level::TellIfPaused()
{
	return is_paused;
}

bool Level::TellIfPlayerWon()
{
	return has_player_won;
}

bool Level::TellIfPlayerLost()
{
	return has_player_lost;
}

void Level::Win()
{
	has_player_won = true;
}

void Level::Loose()
{
	has_player_lost = true;
}

bool Level::CheckIfPlayerIsAlive()
{
	if (Creature::ptr_current_main_charater->AmIAlive())
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Level::FinishLevel(LevelEnding my_ending)
{
	if (my_ending == victory)
	{
		printf("You won!\n");
	}
	else if (my_ending == defeat)
	{
		printf("You lost!\n");
	}
	else
	{
		printf("Level finished!\n");
	}
}

//**************
//CYCLIC ACTIONS
//**************

void Level::PerformCyclicActions()
{
	if (TellIfPaused() == false)
	{
		//Actions on Level plane
		for (std::function<void(Level*)> action : cyclic_actions)
		{
			action(this);
		}

		//Actions on Level Components plane
		MakeLevelComponentsPerformCyclicActions();
	}
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
	//#TODO - zoptymalizowaæ!
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
	if (ptr_my_creature == nullptr)
	{
		printf("Cannot render nullptr!\n");
		throw std::invalid_argument("Cannot render nullptr!\n");
	}
	for (VisualComponent* ptr_creature_visual_component : ptr_my_creature->visual_components)
	{
		ptr_creature_visual_component->Render();
	}
}

void Level::RenderGui()
{
	if (ptr_gui == nullptr)
	{
		printf("Cannot render nullptr!\n");
		throw std::invalid_argument("Cannot render nullptr!\n");
	}
	ptr_gui->RenderComponents();
}
