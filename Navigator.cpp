#include <Navigator.hpp>

Navigator::Navigator(Coordinates my_anchor, std::vector<Coordinates> my_waypoints_set, bool should_run_in_loop)
{
	mode = navig_coordinates_list;
	anchor_point = my_anchor;
	waypoints_set = my_waypoints_set;
	run_in_loop = should_run_in_loop;
}

Navigator::Navigator(Coordinates my_circle_center, int my_radius, int my_angle_step_degrees)
{
	mode = navig_circle;
	anchor_point = my_circle_center;
	radius = my_radius;
	angle_step_degrees = my_angle_step_degrees;
}

void Navigator::SetNextWaypoint()
{
	Coordinates result = { 0,0 };
	if (state == active)
	{
		if (mode == navig_circle)
		{
			//printf("Entered circle calculation.\n");
			result.x = sin(Angle::DegreeToRadian(current_angle_degrees))*radius + anchor_point.x;
			result.y = cos(Angle::DegreeToRadian(current_angle_degrees))*radius + anchor_point.y;
			current_angle_degrees += angle_step_degrees;
			current_angle_degrees = Angle::NormalizeAngle(current_angle_degrees);
			printf("Current angle in circle navigation: %d.\n", current_angle_degrees);
		}
		else if (mode == navig_coordinates_list)
		{
			//printf("Entered waypoints list navigation mode.\n");
			if (current_waypoint_index < waypoints_set.size()-1)
			{
				result = waypoints_set[current_waypoint_index];
				result.x += anchor_point.x;
				result.y += anchor_point.y;
				printf("Setting new waypoint from list.\n");
				current_waypoint = result;
				current_waypoint_index++;
			}
			else if (current_waypoint_index == waypoints_set.size() - 1 && run_in_loop == true)
			{
				result = waypoints_set[current_waypoint_index];
				result.x += anchor_point.x;
				result.y += anchor_point.y;
				current_waypoint = result;
				printf("Resetting waypoint index.\n");
				current_waypoint_index = 0;
			}
			else if (current_waypoint_index == waypoints_set.size() - 1 && run_in_loop == false)
			{
				result = waypoints_set[current_waypoint_index];
				current_waypoint = result;
				current_waypoint_index++;
			}
			else
			{
				//#TODO - niezbyt bezpieczny pomys³ z wyjechaniem poza indeks
				printf("Resetting waypoint index.\n");
				current_waypoint_index = 0;
				printf("Deactivating navigator.\n");
				SetMyState(inactive);
			}
		}
	}
	else
	{
		printf("Navigator is inactive!\n");
	}
}

Coordinates Navigator::TellCurrentWaypoint()
{
	return current_waypoint;
}

void Navigator::SetMyMode(NavigatorMode my_mode)
{
	mode = my_mode;
}


NavigatorMode Navigator::TellMyMode()
{
	return mode;
}

bool Navigator::WasCurrentWaypointReached(Coordinates user_position, int radius)
{
	//Radius tells us how much tolerance is allowed when calculating if waypoint was reached.
	double distance_between_user_and_waypoint = Distance::CalculateDistanceBetweenPoints(user_position,current_waypoint);
	if (static_cast<int>(distance_between_user_and_waypoint) <= radius)
	{
		printf("Waypoint x: %f y: %f reached!.\n", current_waypoint.x, current_waypoint.y);
		return true;
	}
	else
	{
		return false;
	}
}

void Navigator::Move(Coordinates movement)
{
	anchor_point.x += movement.x;
	anchor_point.y += movement.y;
	current_waypoint.x += movement.x;
	current_waypoint.y += movement.y;
}

void Navigator::SetMyState(NavigationState my_state)
{
	state = my_state;
}

NavigationState Navigator::TellMyState()
{
	return state;
}