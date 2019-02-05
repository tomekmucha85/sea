#ifndef LEVEL_COMPONENT_TEST_HPP
#define LEVEL_COMPONENT_TEST_HPP
#include <LevelComponent.hpp>
class LevelComponentTest : public LevelComponent
{
    private:
	
    public:
	    LevelComponentTest(std::map<LevelComponentType, std::vector<LevelComponent*>>* my_ptr_peer_level_components);
};
#endif // LEVEL_COMPONENT_TEST_HPP

