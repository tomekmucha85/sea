#include <Creature.hpp>

double Behavior::MAX_RADIUS_FOR_FINDING_CLOSEST_AVAILABLE_CREATURE = 400;
double Behavior::DISTANCE_TO_KEEP_BETWEEN_HERO_AND_FOLLOWED_CARRIER = 30;

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
			}
		}
		if (ptr_followed_carrier != nullptr && Creature::IsThisCreaturePresentInEnvironment(ptr_followed_carrier))
			//If carrier is set && it currently exists in environment (e.g. wasn't deleted after being chosen)
		{
			Coordinates followed_carriers_center = ptr_followed_carrier->TellCenterPoint();
			double distance_to_carrier = Distance::CalculateDistanceBetweenPoints(ptr_my_creature->TellCenterPoint(),
				followed_carriers_center);
			if (distance_to_carrier < DISTANCE_TO_KEEP_BETWEEN_HERO_AND_FOLLOWED_CARRIER)
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
			printf("No one to follow.\n");
			present_action_result = beh_result_action_failed;
		}
	}
	else if (mode == beh_wander_on_navmesh)
	{
		Coordinates my_center = ptr_my_creature->TellCenterPoint();
		if (ptr_navigator == nullptr)
		{
			static bool was_request_placed = false;
			if (was_request_placed == false)
			{
				ptr_my_creature->PlaceRandomPathRequest(5);
				was_request_placed = true;
			}
			else
			{
				was_request_placed = false;
			}

		}
		else
		{
			if (ptr_navigator->TellMyState() == active)
			{
				if (ptr_navigator->WasCurrentWaypointReached(my_center,10))
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
	}
	else if (mode == beh_go_towards_fixed_point)
	{
		if (was_mode_changed == true)
		{
			printf("Will go towards fixed point: x: %f y: %f.\n", destination_point.x,
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
	if (mode_to_be_set == beh_go_towards_fixed_point)
	{
		printf("No destination point specified!\n");
		throw("No destination point specified!\n");
	}
	was_mode_changed = true;
	mode = mode_to_be_set;
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

void Behavior::RequestMode(BehaviorMode mode_to_be_requested)
{
	if (mode_to_be_requested == beh_go_towards_fixed_point)
	{
		printf("No destination point specified!\n");
		throw("No destination point specified!\n");
	}
	current_requested_mode = mode_to_be_requested;
}

void Behavior::RequestMode(BehaviorMode mode_to_be_requested, Coordinates my_destination_point)
{
	if (mode_to_be_requested != beh_go_towards_fixed_point)
	{
		printf("No use of specified destination point!\n");
		throw("No use of specified destination point!\n");
	}
	current_requested_mode = mode_to_be_requested;
	current_requested_mode_destination = my_destination_point;
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
	//#TODO - czy towrzenie anchora potrzebne?
	Coordinates anchor = { 0, 0 };
	printf("Received plan from nav grid. Plan:\n");
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
	printf("Received plan from nav grid. Plan:\n");
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