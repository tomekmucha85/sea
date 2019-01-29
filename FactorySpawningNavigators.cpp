#include <FactorySpawningNavigators.hpp>

Navigator* FactorySpawningNavigators::SpawnNavigator(NavigatorMode desired_mode, Coordinates circle_center, int radius, int angle_step_degrees)
{
	Navigator* result = nullptr;
	if (desired_mode == navig_circle)
	{
		result = new Navigator(circle_center, radius, angle_step_degrees);
	}
	else
	{
		printf("This should've been a circle navigator!\n");
		throw std::invalid_argument("This should've been a circle navigator!\n");
	}
	return result;
}

Navigator* FactorySpawningNavigators::SpawnNavigator(NavigatorMode desired_mode, Coordinates my_anchor_point,
	std::vector<Coordinates> navigation_points, bool should_run_in_loop)
{
	Navigator* result = nullptr;
	if (desired_mode == navig_coordinates_list)
	{
		result = new Navigator(my_anchor_point, navigation_points, should_run_in_loop);
	}
	else
	{
		printf("This should've been a point list navigator!\n");
		throw std::invalid_argument("This should've been a point list navigator!\n");
	}
	return result;
}