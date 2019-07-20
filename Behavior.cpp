#include <Creature.hpp>

double Behavior::MAX_RADIUS_FOR_FINDING_CLOSEST_AVAILABLE_CREATURE = 400;
double Behavior::DISTANCE_TO_KEEP_BETWEEN_HERO_AND_FOLLOWED_CREATURE = 30;

Behavior::Behavior()
{
	ptr_navigators_factory = new FactorySpawningNavigators();
}

Behavior::~Behavior()
{
	delete ptr_navigators_factory;
	delete ptr_navigator;
}

void Behavior::WhatToDo(Creature* ptr_my_creature)
{
	if (pattern == beh_pat_death_magnetic)
	{
		/*
		1. Default - navigate towards bad exit
		2. Upon follow closest carrier request, try it and hold this mode as long following is successfull
		3. Upon idle request while following carrier, set idle immediately, hold for given amount of time and return to bad exit navigation


		*Increase speed while getting closer 

		*Upon closing eyes, cast random spell -> teleport, navigation in correct direction, spawn multiple carriers
		
		*/

		//UPON ENTERING PATTERN

		if (was_pattern_changed == true)
		{
			was_pattern_changed = false;
			SetMode(beh_go_towards_fixed_point, 
				beh_pat_death_magnetic_destination->TellCenterPoint());
		}

		//SERVE MODE CHANGE REQUESTS

		if (current_requested_mode == beh_follow_closest_carrier)
		{
			SetMode(current_requested_mode);
			current_requested_mode = beh_none;
		}
		else if (current_requested_mode == beh_idle ||
			current_requested_mode == beh_go_towards_fixed_point)
		{
			SetMode(beh_go_towards_fixed_point, 
				beh_pat_death_magnetic_destination->TellCenterPoint());
			current_requested_mode = beh_none;
		}
		else
		{
			current_requested_mode = beh_none;
		}

		//TAKE ACTION

		if (mode == beh_follow_closest_carrier)
		{
			BehaviorActionResult result = PerformActionDefinedByMode(ptr_my_creature);
			if (result == beh_result_action_failed)
			{
				SetMode(beh_go_towards_fixed_point, 
					beh_pat_death_magnetic_destination->TellCenterPoint());
			}
		}
		else if (mode == beh_go_towards_fixed_point)
		{
			PerformActionDefinedByMode(ptr_my_creature);
		}
	}
	else if (pattern == beh_pat_stalker)
	{
		//UPON ENTERING PATTERN

		if (was_pattern_changed == true)
		{
			was_pattern_changed = false;
			SetMode(beh_wander_on_navmesh);
		}
        
		//SERVE MODE CHANGE REQUESTS

		if (current_requested_mode == beh_follow_certain_creature)
		{
			printf("Behavior pattern stalker agreed to follow creature %p at x: %f,y: %f.\n",
				ptr_current_requested_mode_destination_creature,
				ptr_current_requested_mode_destination_creature->TellCenterPoint().x,
				ptr_current_requested_mode_destination_creature->TellCenterPoint().y);
			SetMode(current_requested_mode, ptr_current_requested_mode_destination_creature);
			current_requested_mode = beh_none;
		}
		else if (current_requested_mode == beh_wander_on_navmesh)
		{
			SetMode(current_requested_mode);
			current_requested_mode = beh_none;
		}
		else if (current_requested_mode == beh_none)
		{
			;
		}
		else
		{
			printf("This mode will be not served in stalker behavior pattern.\n");
			printf("Setting none.\n");
			current_requested_mode = beh_none;
		}

		//TAKE ACTION
		if (mode == beh_wander_on_navmesh)
		{
			//Check if there is a carrier creature nearby
			Creature* ptr_closest_carrier = ptr_my_creature->FindClosestAccessibleCreatureOfGivenType(cre_carrier_a,
				MAX_RADIUS_FOR_FINDING_CLOSEST_AVAILABLE_CREATURE);

			//Creature* ptr_closest_carrier = nullptr;

			if (ptr_closest_carrier != nullptr)
			{
				printf("Accessible carrier found for stalker behavior pattern: %p.\n",
					ptr_closest_carrier);
				RequestMode(beh_follow_certain_creature, ptr_closest_carrier);
			}
			else
			{
				BehaviorActionResult result = PerformActionDefinedByMode(ptr_my_creature);
				if (result == beh_result_action_failed)
				{
					printf("Unable to wander on navmesh. Why?\n");
				}
				else if (result == beh_result_objective_complete)
				{
					printf("Reached end of random path in stalker mode. Chosing another one.\n");
					RequestMode(beh_wander_on_navmesh);
				}
			}
		}
		else if (mode == beh_follow_certain_creature)
		{
			BehaviorActionResult result = PerformActionDefinedByMode(ptr_my_creature);
			if (result == beh_result_action_failed)
			{
				printf("Failed to follow creature in stalker behavior pattern.\n");
				RequestMode(beh_wander_on_navmesh);
			}
		}
		else
		{
			//For all other behavior modes - do nothing
			;
		}
	}
	else if (pattern == beh_pat_none)
	{
		if (was_pattern_changed == true)
		{
			was_pattern_changed = false;
		}
		PerformActionDefinedByMode(ptr_my_creature);
	}
	else
	{
		printf("Unknown behavior pattern: %d!\n", pattern);
		throw std::invalid_argument("Unknown behavior pattern!\n");
	}
}

BehaviorActionResult Behavior::PerformActionDefinedByMode(Creature* ptr_my_creature)
//Returns: 	beh_result_action_succedeed - if action went through OK
//          beh_result_action_failed - if action failed
//          beh_result_objective_complete - if action succeeded and goal of specific mode was fulfilled
//          beh_result_objective_failed - if mode wasn't able to reach its goal.

{
	BehaviorActionResult present_action_result = beh_result_action_succedeed;

	if (mode == beh_idle)
	{
		if (was_mode_changed == true)
		{
			ptr_my_creature->SetVelocity(0);
			was_mode_changed = false;
		}
		else
		{
			;
		}
	}
	else if (mode == beh_chase_hero)
	{
		Coordinates my_center = ptr_my_creature->TellCenterPoint();
		Coordinates hero_center = Creature::ptr_current_main_charater->TellCenterPoint();
		double distance_from_hero = Distance::CalculateDistanceBetweenPoints(my_center, hero_center);
		if (distance_from_hero < 100)
		{
			ptr_my_creature->ThrustTowardsPoint(hero_center);
		}
		else
		{
			//Hero too far.
			present_action_result = beh_result_action_failed;
		}
	}
	else if (mode == beh_follow_closest_carrier)
	{
		if (was_mode_changed == true)
		{
			//When this behavior is initialized, find closest creature that can serve as character's "carrier" guiding through the maze.
			Creature* ptr_closest_carrier = ptr_my_creature->FindClosestAccessibleCreatureOfGivenType(cre_carrier_a, 
				MAX_RADIUS_FOR_FINDING_CLOSEST_AVAILABLE_CREATURE);
			ptr_followed_carrier = ptr_closest_carrier;
			was_mode_changed = false;
			if (ptr_followed_carrier != nullptr)
			{
				printf("Entered mode follow closest carrier. Carrier at: x: %f y: %f\n",
					ptr_followed_carrier->TellCenterPoint().x,
					ptr_followed_carrier->TellCenterPoint().y);
			}
			else
			{
				printf("No one to follow in follow closest carrier mode.\n");
				present_action_result = beh_result_action_failed;
			}
		}
		if (ptr_followed_carrier != nullptr && Creature::IsThisCreaturePresentInEnvironment(ptr_followed_carrier))
			//If carrier is set && it currently exists in environment (e.g. wasn't deleted after being chosen)
		{
			Coordinates followed_carriers_center = ptr_followed_carrier->TellCenterPoint();
			double distance_to_carrier = Distance::CalculateDistanceBetweenPoints(ptr_my_creature->TellCenterPoint(),
				followed_carriers_center);
			if (distance_to_carrier < DISTANCE_TO_KEEP_BETWEEN_HERO_AND_FOLLOWED_CREATURE)
			{
				//Moving with carrier's velocity if getting close to carrier.
				double carriers_velocity = ptr_followed_carrier->TellVelocity();
				ptr_my_creature->ThrustTowardsPoint(followed_carriers_center, carriers_velocity);
			}
			else
			{
				//Moving with default velocity
				ptr_my_creature->ThrustTowardsPoint(followed_carriers_center);
			}
		}
		else
		{
			printf("No one to follow - my carrier creature ceased to be.\n");
			present_action_result = beh_result_action_failed;
		}
	}
	else if (mode == beh_follow_certain_creature)
	{
		if (was_mode_changed == true)
		{
			//When this behavior is initialized, follow a creature, pointer to which has been provided as ptr_followed_creature.
			was_mode_changed = false;
			if (ptr_followed_creature != nullptr)
			{
				printf("Entered mode follow creature. Creature at: x: %f y: %f\n",
					ptr_followed_creature->TellCenterPoint().x,
					ptr_followed_creature->TellCenterPoint().y);
			}
			else
			{
				printf("No one to follow in follow certain creature mode.\n");
				present_action_result = beh_result_action_failed;
			}
		}
		if (ptr_followed_creature != nullptr && Creature::IsThisCreaturePresentInEnvironment(ptr_followed_creature))
			//If creature is set && it currently exists in environment (e.g. wasn't deleted after being chosen)
		{
			Coordinates followed_creature_center = ptr_followed_creature->TellCenterPoint();
			double distance_to_creature = Distance::CalculateDistanceBetweenPoints(ptr_my_creature->TellCenterPoint(),
				followed_creature_center);
			if (distance_to_creature < DISTANCE_TO_KEEP_BETWEEN_HERO_AND_FOLLOWED_CREATURE)
			{
				//Moving with creature's velocity if getting close to it.
				double creature_velocity = ptr_followed_creature->TellVelocity();
				ptr_my_creature->ThrustTowardsPoint(followed_creature_center, creature_velocity);
			}
			else
			{
				//Moving with default velocity
				ptr_my_creature->ThrustTowardsPoint(followed_creature_center);
			}
		}
		else
		{
			printf("No one to follow - my followed creature ceased to be.\n");
			present_action_result = beh_result_action_failed;
		}
	}
	else if (mode == beh_wander_on_navmesh)
	{
	    if (was_mode_changed == true)
	    {
			was_mode_changed = false;
			if (ptr_navigator != nullptr)
			{
				//Cleanup
				delete ptr_navigator;
				ptr_navigator = nullptr;
			}
			ptr_my_creature->PlaceRandomPathRequest(5);
			was_wander_path_request_placed = true;
		}
		else
		{
			if (ptr_navigator == nullptr)
			{
				if (was_wander_path_request_placed)
				{
					printf("There is no navigator but path request was already placed.\n");
				}
				else
				{
					printf("There is no navigator and no path request placed!\n");
				}
			}
			else
			{
				Coordinates my_center = ptr_my_creature->TellCenterPoint();
				if (ptr_navigator->TellMyState() == active)
				{
					if (ptr_navigator->WasCurrentWaypointReached(my_center, 10))
					{
						ptr_navigator->SetNextWaypoint();
					}
					ptr_my_creature->ThrustTowardsPoint(ptr_navigator->TellCurrentWaypoint());
				}
				else
				{
					ptr_my_creature->SetVelocity(0);
					present_action_result = beh_result_objective_complete;
					delete ptr_navigator;
					ptr_navigator = nullptr;
				}
			}
		}
	}
	else if (mode == beh_go_towards_fixed_point)
	{
		if (was_mode_changed == true)
		{
			printf("Creature %p will go towards fixed point: x: %f y: %f.\n",
				ptr_my_creature,
				destination_point.x,
				destination_point.y);
			delete ptr_navigator;
			ptr_navigator = nullptr;
			ptr_my_creature->PlacePointToPointPathRequest(destination_point);
			was_mode_changed = false;
		}
		if (ptr_navigator != nullptr && ptr_navigator->TellMyState() == active)
			//First condition in case navigator was requested but wasn't delivered yet.
		{
			if (ptr_navigator->WasCurrentWaypointReached(ptr_my_creature->TellCenterPoint(), 10))
			{
				ptr_navigator->SetNextWaypoint();
			}
			ptr_my_creature->ThrustTowardsPoint(ptr_navigator->TellCurrentWaypoint());
		}
		else
		{
			ptr_my_creature->SetVelocity(0);
		}
	}
	else if (mode == beh_run_along_predefined_path)
	{
		Coordinates my_center = ptr_my_creature->TellCenterPoint();
		std::vector<Coordinates> example_path = { {0,0}, {640,0}, {640,480}, {0,480} };
		Coordinates anchor = {0,0};
		if (ptr_navigator == nullptr)
		{
			printf("Spawning a Navigator!\n");
			printf("Anchor point x: %f, y: %f.\n", anchor.x, anchor.y);
			ptr_navigator = ptr_navigators_factory->SpawnNavigator(navig_coordinates_list, 
				anchor, example_path, false);
			ptr_my_creature->ThrustTowardsPoint(ptr_navigator->TellCurrentWaypoint());
		}
		if (ptr_navigator->TellMyState() == active)
		{
			ptr_my_creature->ThrustTowardsPoint(ptr_navigator->TellCurrentWaypoint());
			if (ptr_navigator->WasCurrentWaypointReached(my_center))
			{
				ptr_navigator->SetNextWaypoint();
				ptr_my_creature->ThrustTowardsPoint(ptr_navigator->TellCurrentWaypoint());
			}
		}
		else
		{
			ptr_my_creature->SetVelocity(0);
			present_action_result = beh_result_objective_complete;
		}
	}
	else if (mode == beh_run_in_circles)
	{
		Coordinates my_center = ptr_my_creature->TellCenterPoint();
		if (ptr_navigator == nullptr)
		{
			printf("Spawning a Navigator!\n");
			printf("Center point x: %f, y: %f.\n", my_center.x, my_center.y);
			ptr_navigator = ptr_navigators_factory->SpawnNavigator(navig_circle, my_center, 300, 15);
			ptr_navigator->SetNextWaypoint();
			ptr_my_creature->ThrustTowardsPoint(ptr_navigator->TellCurrentWaypoint());
		}
		ptr_my_creature->ThrustTowardsPoint(ptr_navigator->TellCurrentWaypoint());
		if (ptr_navigator->WasCurrentWaypointReached(my_center))
		{
			ptr_navigator->SetNextWaypoint();
			ptr_my_creature->ThrustTowardsPoint(ptr_navigator->TellCurrentWaypoint());
		}
	}
	else if (mode == beh_projectile)
	{
		ptr_my_creature->ThrustForward();
	}

	return present_action_result;
}

void Behavior::SetPattern(BehaviorPattern pattern_to_be_set)
{
	if (pattern_to_be_set == beh_pat_none)
	{
		pattern = pattern_to_be_set;
		was_pattern_changed = true;
	}
	else if (pattern_to_be_set == beh_pat_stalker)
	{
		pattern = pattern_to_be_set;
		was_pattern_changed = true;
	}
	else
	{
		printf("Wrong arguments set for this behavior pattern!\n");
		throw std::invalid_argument("Wrong arguments set for this behavior pattern!\n");
	}
}

void Behavior::SetPattern(BehaviorPattern pattern_to_be_set, Creature* ptr_my_destiny)
{
	if (pattern_to_be_set == beh_pat_death_magnetic)
	{
		pattern = pattern_to_be_set;
		beh_pat_death_magnetic_destination = ptr_my_destiny;
		was_pattern_changed = true;
	}
	else
	{
	    printf("Wrong arguments set for this behavior pattern!\n");
	    throw std::invalid_argument("Wrong arguments set for this behavior pattern!\n");
	}
}

void Behavior::SetMode(BehaviorMode mode_to_be_set)
{
	//printf("Behavior mode was changed.\n");
	// #TODO - wprowadziæ listê behavior modes niewymagaj¹cych argumentów i iterowaæ po niej tu oraz w RequestMode
	if (mode_to_be_set == beh_chase_hero ||
		mode_to_be_set == beh_follow_closest_carrier ||
		mode_to_be_set == beh_idle ||
		mode_to_be_set == beh_none ||
		mode_to_be_set == beh_projectile ||
		mode_to_be_set == beh_run_in_circles ||
		mode_to_be_set == beh_wander_on_navmesh)
	{
		was_mode_changed = true;
		mode = mode_to_be_set;
	}
	else
	{
		printf("Invalid arguments for setting behavior mode %d.\n", mode_to_be_set);
		throw std::invalid_argument("Invalid arguments for setting behavior mode");
	}
}

void Behavior::SetMode(BehaviorMode mode_to_be_set, Coordinates my_destination_point)
{
	if (mode_to_be_set != beh_go_towards_fixed_point)
	{
		printf("No use of specified destination point!\n");
		throw("No use of specified destination point!\n");
	}
	was_mode_changed = true;
	mode = mode_to_be_set;
	destination_point = my_destination_point;
}

void Behavior::SetMode(BehaviorMode mode_to_be_set, Creature* ptr_my_destiny)
{
	if (mode_to_be_set == beh_follow_certain_creature)
	{
		was_mode_changed = true;
		mode = mode_to_be_set;
		ptr_followed_creature = ptr_my_destiny;
	}
	else
	{
		printf("No use of specified destination creature!\n");
		throw std::invalid_argument("No use of specified destination creature!");
	}
}

void Behavior::RequestMode(BehaviorMode mode_to_be_requested)
{
	if (mode_to_be_requested == beh_chase_hero ||
		mode_to_be_requested == beh_follow_closest_carrier ||
		mode_to_be_requested == beh_idle ||
		mode_to_be_requested == beh_none ||
		mode_to_be_requested == beh_projectile ||
		mode_to_be_requested == beh_run_in_circles ||
		mode_to_be_requested == beh_wander_on_navmesh)
	{
		current_requested_mode = mode_to_be_requested;
	}
	else
	{
		printf("Not enough arguments to request chosen behavior mode %d!\n", mode_to_be_requested);
		throw std::invalid_argument("Not enough arguments to request chosen behavior mode!\n");
	}
}

void Behavior::RequestMode(BehaviorMode mode_to_be_requested, Coordinates my_destination_point)
{
	if (mode_to_be_requested == beh_go_towards_fixed_point)
	{
		current_requested_mode = mode_to_be_requested;
		current_requested_mode_destination = my_destination_point;
	}
	else
	{
		printf("No use of specified destination point!\n");
		throw("No use of specified destination point!\n");
	}

}

void Behavior::RequestMode(BehaviorMode mode_to_be_requested, Creature* ptr_destination_creature)
{
	if (mode_to_be_requested == beh_follow_certain_creature)
	{
		Logger::Log("Requested mode beh_follow_certain_creature.");
		std::string message = "Certain creature at x: " +
			std::to_string(ptr_destination_creature->TellCenterPoint().x) +
			" y: " +
			std::to_string(ptr_destination_creature->TellCenterPoint().y);
		Logger::Log(message, debug_full);
		current_requested_mode = mode_to_be_requested;
		ptr_current_requested_mode_destination_creature = ptr_destination_creature;
	}
	else
	{
		printf("No use of specified destination creature!\n");
		throw std::invalid_argument("No use of specified destination creature!");
	}
}

//#TODO - ucywilizowaæ
void Behavior::Move(Coordinates movement)
{
	if (ptr_navigator != nullptr)
	{
		ptr_navigator->Move(movement);
	}
}


void Behavior::MakeUseOfPathResponse(RandomPathResponse my_response)
{
	//#TODO! Ogarn¹æ kasowanie nawigatorów!
	delete ptr_navigator;
	//#TODO - czy tworzenie anchora potrzebne?
	Coordinates anchor = { 0, 0 };
	//printf("Received plan from nav grid. Plan:\n");
	Logger::Log("Received plan from nav grid. Plan:");
	for (Coordinates point : my_response.navigation_path)
	{
		printf("x: %f y: %f\n", point.x, point.y);
	}
	if (my_response.navigation_path.size() != 0)
	{
		ptr_navigator = ptr_navigators_factory->SpawnNavigator(navig_coordinates_list,
			anchor, my_response.navigation_path, false);
	}
	else
	{
		printf("Received empty path!\n");
	}
}

void Behavior::MakeUseOfPathResponse(PointToPointPathResponse my_response)
{
	//#TODO! Ogarn¹æ kasowanie nawigatorów!
	//#TODO - duplikacja kodu wzglêdem poprzedniej funkcji. Ogarn¹æ.
	delete ptr_navigator;
	//#TODO - czy towrzenie anchora potrzebne?
	Coordinates anchor = { 0, 0 };
	//printf("Received plan from nav grid. Plan:\n");
	Logger::Log("Received plan from nav grid. Plan:\n");
	for (Coordinates point : my_response.navigation_path)
	{
		//printf("x: %f y: %f\n", point.x, point.y);
		std::string message = "x: " + std::to_string(point.x) + " y: " + std::to_string(point.y);
		Logger::Log(message);
	}
	if (my_response.navigation_path.size() != 0)
	{
		ptr_navigator = ptr_navigators_factory->SpawnNavigator(navig_coordinates_list,
			anchor, my_response.navigation_path, false);
	}
	else
	{
		printf("Received empty path!\n");
	}
}