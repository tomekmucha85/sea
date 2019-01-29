#ifndef FACTORY_SPAWNING_NAVIGATORS_HPP
#define FACTORY_SPAWNING_NAVIGATORS_HPP
#include <vector>
#include <CustomDefinedTypes.hpp>
#include <Navigator.hpp>

class FactorySpawningNavigators
{
    private:


    public:
		Navigator* SpawnNavigator(NavigatorMode desired_mode, Coordinates circle_center, int radius, int angle_step_degrees);
		Navigator* SpawnNavigator(NavigatorMode desired_mode, Coordinates my_anchor_point, 
			std::vector<Coordinates> navigation_points, bool should_run_in_loop);

};


#endif //FACTORY_SPAWNING_NAVIGATORS_HPP
