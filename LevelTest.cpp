#include <LevelTest.hpp>

LevelTest::LevelTest()
{
	PreciseRect core_area = { 0,0,0,0 };
	LevelComponent* ptr_core = ptr_components_factory->SpawnLevelComponent(levco_core, core_area);
	/*Coordinates bground_position = { 320,240 };
	Creature* ptr_bground = ptr_core->AddCreature(cre_blue_bground, &bground_position, merge);
	ptr_bground->MakeMeNotObstacle();
	Coordinates test_spell_position = { 390, 100 };
	Creature* ptr_test_spell = ptr_core->AddCreature(cre_spell_ball, &test_spell_position, merge);*/
	Coordinates hero_position = { 320,240 };
	Creature* ptr_hero = ptr_core->AddCreature(cre_clawy, &hero_position, force);
	Coordinates wall_1_position = { 100,100 };
	Creature* ptr_wall_1 = ptr_core->AddCreature(cre_flying_box, &wall_1_position, force);/**
	Coordinates wall_2_position = { 200,200 };
	Creature* ptr_wall_2 = ptr_core->AddCreature(cre_flying_box, &wall_2_position, force);

	ptr_wall_2->SetBehaviorMode(beh_projectile);*/

	ptr_hero->MakeMeMainCharacter();
}