#include <LevelTest.hpp>

LevelTest::LevelTest()
{
	/*Coordinates bground_position = { 320,240 };
	Creature* ptr_bground = ptr_core->AddCreature(cre_blue_bground, &bground_position, merge);
	ptr_bground->MakeMeNotObstacle();
	Coordinates test_spell_position = { 390, 100 };
	Creature* ptr_test_spell = ptr_core->AddCreature(cre_spell_ball, &test_spell_position, merge);*/
	Coordinates wall_1_position = { 100,100 };
	Creature* ptr_wall_1 = ptr_initial_core_component->AddCreature(cre_flying_box, &wall_1_position, force);
	Coordinates other_guy_position = { 50,300 };
	Creature* ptr_other_guy = ptr_initial_core_component->AddCreature(cre_clawy, &other_guy_position, force);
	ptr_other_guy->SetBehaviorMode(beh_run_along_predefined_path);
	//Coordinates other_guy2_position = { 500,300 };
	//Creature* ptr_other_guy2 = ptr_initial_core_component->AddCreature(cre_clawy, &other_guy2_position, force);

    //ptr_other_guy2->SetBehaviorMode(beh_test_movement);
	//ptr_other_guy2->SetBehaviorMode(beh_run_in_circles);
	ptr_test_creature = ptr_wall_1;

	/*																			
	Coordinates wall_2_position = { 200,200 };
	Creature* ptr_wall_2 = ptr_core->AddCreature(cre_flying_box, &wall_2_position, force);

	ptr_wall_2->SetBehaviorMode(beh_projectile);*/

	//Trigger to win level
	std::string signal_to_win = "winning";
	PreciseRect event_area = {100,100,100,100};
	Creature* ptr_winning_trigger = AddTriggerUsingDefaultComponent(event_area, signal_to_win);
	signals_vs_events[signal_to_win] = ptr_func_win;
	//Red
	ptr_winning_trigger->ptr_creature_visual_component->SetColor({ 255,0,0,255 });
}

void LevelTest::FinishLevel(LevelEnding my_ending)
{
	Pause();
	if (my_ending == victory)
	{
		printf("You've beaten test level!\n");
	}
	else if (my_ending == defeat)
	{
		printf("You've been beaten.\n");
	}
	else
	{
		printf("Some other ending!\n");
	}
}