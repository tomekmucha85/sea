#ifndef NAVIGATOR_HPP
#define NAVIGATOR_HPP
#include <vector>
#include <math.h>
#include <CommonMath.hpp>
#include <CustomDefinedTypes.hpp>

class Navigator
{

    private:
		Coordinates anchor_point = {0,0};
		NavigatorMode mode = navig_undetermined;
		//Where does the navigator point now?
		Coordinates current_waypoint = {0,0};
		NavigationState state = active;

		//For circle-like path
		int radius = 0;
		int angle_step_degrees = 20;
		int current_angle_degrees = 0;

		//For predefined points
		std::vector<Coordinates> waypoints_set = {};
		bool run_in_loop = true;
		unsigned int current_waypoint_index = 0;

    public:
		Navigator(Coordinates my_anchor, std::vector<Coordinates> my_waypoints_set, bool should_run_in_loop);
		Navigator(Coordinates my_circle_center, int my_radius, int my_angle_step_degrees=0);
		Coordinates TellCurrentWaypoint();
		void SetNextWaypoint();
		bool WasCurrentWaypointReached(Coordinates user_position, int radius = 40);
		void SetMyMode(NavigatorMode my_mode);
		NavigatorMode TellMyMode();
		void Move(Coordinates movement);
		void SetMyState(NavigationState my_state);
		NavigationState TellMyState();
};

#endif //NAVIGATOR_HPP
