#ifndef LEVEL_COMPONENT_HPP
#define LEVEL_COMPONENT_HPP
#include <map>
#include <SDL.h>
#include <vector>
#include <iterator>
#include <algorithm>
#include <functional>
#include <string>
#include <Creature.hpp>
#include <FactorySpawningCreatures.hpp>



class LevelComponent
{
    private:
		//###################
		//Variables
		//###################
		std::vector<Creature*> creatures = {};
		std::map<LevelComponentType, std::vector<LevelComponent*>>* ptr_peer_level_components;
		//Invisible creature serving as "mask" of level component. Applied only to level components with defined area.
		Creature* ptr_component_outline = nullptr;
		//Contains actions associated to specific creature which will be performed during every game loop / every n game loops.
		std::vector<std::function<void(LevelComponent*)>> cyclic_actions = {};
		//std::vector<CreatureSpawnRequest> external_spawn_requests = {};
		//#TODO - powr�ci� do jednoargumentowego spawn requestu
		std::vector<CreatureSpawnRequest> external_spawn_requests= {};
		std::vector<CreatureDestructionInGivenAreaRequest> external_destruction_requests = {};
		//###################
        //Functions
        //###################
		void AddLevelComponentOutline(PreciseRect my_component_area);
    public:
		//###################
        //Variables
        //###################
		LevelComponentType my_type = levco_core;
		FactorySpawningCreatures* ptr_creatures_factory = nullptr;
		//Expressed in pixels
		static int map_block_width;
		static int map_block_height;

		//###################
		//Functions
		//###################
		LevelComponent(std::map<LevelComponentType, std::vector<LevelComponent*>>* my_ptr_peer_level_components, 
			PreciseRect my_component_area = {0,0,0,0});
		virtual ~LevelComponent();
		std::vector<Creature*>* TellPtrToCreaturesArray();
		void SetPointerToPeerComponentsIndex(std::map<LevelComponentType, std::vector<LevelComponent*>>* my_ptr_peer_level_components);
		std::map<LevelComponentType, std::vector<LevelComponent*>>* TellPtrToPeerLevelComponentsArray();
		std::map<Creature*, LevelComponent*> FindCreatureNeighborsInAllLevelComponents(Creature* ptr_my_creature);
		Creature* AddCreature(CreatureType my_type, PreciseRect* ptr_my_position, InsertionMode my_mode, std::string my_trigger_signal="");
		Creature* AddCreature(CreatureType my_type, Coordinates* ptr_my_center, InsertionMode my_mode, int my_render_layer = 0);
		void AddExistingCreature(Creature* ptr_my_creature);
		bool DetermineIfCreatureCanBeLeftOnMap(Creature* ptr_my_creature, InsertionMode my_mode);
		void ServeSpawnRequest(CreatureSpawnRequest my_request);
		void ServeInternalSpawnRequest(CreatureSpawnRequest my_request);
		void ServeExternalSpawnRequest(CreatureSpawnRequest my_request);
		virtual void ServeExternalDestructionRequest(CreatureDestructionInGivenAreaRequest my_request);
		void SendSpawnRequestToPeerComponent(CreatureSpawnRequest my_request, LevelComponent* ptr_peer_component);
		void PushIntoExternalSpawnRequests(CreatureSpawnRequest my_request);
		void SendDestructionRequestToPeerComponent(CreatureDestructionInGivenAreaRequest my_request, LevelComponent* ptr_peer_component);
		void PushIntoExternalDestructionRequests(CreatureDestructionInGivenAreaRequest my_request);
		void RemoveCreature(Creature* ptr_my_creature);
		void RemoveAllCreatures();
		void RemoveAllCreaturesExceptHero();
		PreciseRect TellComponentArea();
		PreciseRect TellComponentEdge(Directions my_direction);
		void SetVisibilityForAllCreatures(bool should_be_visible);
		std::vector<Creature*> FindCollisionsWithMainCharacter(bool check_only_obstacles = true);
		void MakeCreaturesPerformCyclicActions();
		void PerformCyclicActions();
		//###################
        //Virtual Functions
        //###################
		virtual void ClearMaze();
		virtual void GenerateMaze();
		virtual void SetBorderState(Directions border_side, bool value);
		virtual bool TellBorderState(Directions border_side);
		virtual std::vector<std::string> RunTriggersHitByHero();

		//###################
        // COMMON LAMBDAS
        //###################

		/*
		//Cyclic action to loop through all Creatures in a component
		std::function<void(LevelComponent*)> func_element_loop = [](LevelComponent* ptr_level_component)
		{
			for (Creature* ptr_my_creature : *(ptr_level_component->TellPtrToCreaturesArray()))
			{
				if (!ptr_my_creature->AmIAlive())
				{
					printf("Headshot!\n");
					ptr_level_component->RemoveCreature(ptr_my_creature);
				}
			}
		};*/



        //Cyclic action to delete all dead creatures
		std::function<void(LevelComponent*)> func_reaper = [](LevelComponent* ptr_level_component)
		{
			for (Creature* ptr_my_creature : *(ptr_level_component->TellPtrToCreaturesArray()))
			{
				if (!ptr_my_creature->AmIAlive())
				{
					printf("Headshot!\n");
					ptr_level_component->RemoveCreature(ptr_my_creature);
				}
			}
		};
		//Cyclic action to serve creature spawn requests placed by peer level components
		std::function<void(LevelComponent*)> func_spawn_creatures_on_peer_component_demand = [](LevelComponent* ptr_level_component)
		{
			for (CreatureSpawnRequest request_to_serve : ptr_level_component->external_spawn_requests)
			{
				ptr_level_component->ServeExternalSpawnRequest(request_to_serve);
			}
			ptr_level_component->external_spawn_requests.clear();
		};

		//Cyclic action to serve creature spawn requests placed by creatures
		std::function<void(LevelComponent*)> func_spawn_creatures_on_other_creature_demand = [](LevelComponent* ptr_level_component)
		{
			std::vector<CreatureSpawnRequest> requests_to_serve = {};
			for (Creature* ptr_my_creature : *(ptr_level_component->TellPtrToCreaturesArray()))
			{
				//printf("Creature: %p, main creature: %p.\n", ptr_my_creature, Creature::WhoIsMainCharacter());
				std::vector<CreatureSpawnRequest>* ptr_spawn_requests = ptr_my_creature->TellSpawnRequests();
				int spawn_requests_number = static_cast<int>(ptr_spawn_requests->size());
				if (spawn_requests_number > 0)
				{
					for (CreatureSpawnRequest my_request : *(ptr_spawn_requests))
					{
						requests_to_serve.push_back(my_request);
					}
					ptr_spawn_requests->clear();
				}
			}

			for (CreatureSpawnRequest request_to_serve : requests_to_serve)
			{
				ptr_level_component->ServeInternalSpawnRequest(request_to_serve);
			}
		};

		//Cyclic action to serve destruction requests
		std::function<void(LevelComponent*)> func_destroy_creatures_on_peer_component_demand = [](LevelComponent* ptr_level_component)
		{
			for (CreatureDestructionInGivenAreaRequest my_request : ptr_level_component->external_destruction_requests)
			{
				printf("Received destruction request!\n");
				ptr_level_component->ServeExternalDestructionRequest(my_request);
			}
			ptr_level_component->external_destruction_requests.clear();
		};

};

#endif LEVEL_COMPONENT_HPP