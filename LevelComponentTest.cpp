#include <LevelComponentTest.hpp>

LevelComponentTest::LevelComponentTest(std::map<LevelComponentType, std::vector<LevelComponent*>>* my_ptr_peer_level_components) :
	LevelComponent(my_ptr_peer_level_components)
{
	;
}