#ifndef LEVEL_TEST_HPP
#define LEVEL_TEST_HPP
#include <Level.hpp>

class LevelTest : public Level
{
    private:

    public:
		LevelTest();
		Creature* ptr_test_creature = nullptr;
};

#endif //LEVEL_TEST_HPP