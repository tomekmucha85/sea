#include <LevelTest.hpp>

LevelTest::LevelTest()
{
	/*Coordinates bground_position = { 320,240 };
	Creature* ptr_bground = ptr_core->AddCreature(cre_blue_bground, &bground_position, merge);
	ptr_bground->MakeMeNotObstacle();*/

	Coordinates other_guy_position = { 50,300 };
	Creature* ptr_other_guy = ptr_initial_core_component->AddCreature(cre_clawy, &other_guy_position, force);
	ptr_other_guy->SetBehaviorMode(beh_wander_on_navmesh);

	//Coordinates other_guy2_position = { 500,300 };
	//Creature* ptr_other_guy2 = ptr_initial_core_component->AddCreature(cre_clawy, &other_guy2_position, force);
	//ptr_other_guy->SetBehaviorMode(beh_wander_on_navmesh);


	//Trigger to win level
	/*std::string signal_to_win = "winning";
	PreciseRect event_area = {100,100,100,100};
	Creature* ptr_winning_trigger = AddTriggerUsingDefaultComponent(event_area, signal_to_win);
	signals_vs_events[signal_to_win] = ptr_func_win;*/
	//Red
	//#TODO - przeszukaæ wszystkie przypadki u¿ycia visual_components[0]
	//ptr_winning_trigger->visual_components[0]->SetColor({ 255,0,0,255 });


	/*Coordinates wall_1_position = { 100,100 };
	Creature* ptr_wall_1 = ptr_initial_core_component->AddCreature(cre_flying_box, &wall_1_position, force,1);
	Coordinates wall_2_position = { 148,100 };
	Creature* ptr_wall_2 = ptr_initial_core_component->AddCreature(cre_flying_box, &wall_2_position, force,1);
	ptr_test_creature = ptr_wall_1;*/

	
	printf("Adding navgrid point!\n");
	//Test navigation grid creature
	Coordinates navgrid_point_center_1 = {300,400};
	CreatureNavGridNode* ptr_test_navgrid_node_1 = dynamic_cast<CreatureNavGridNode*>(AddNavigationNodeUsingDefaultComponent(navgrid_point_center_1));

	printf("Adding another navgrid point!\n");
	//Test navigation grid creature
	Coordinates navgrid_point_center_2 = { 100,400 };
	CreatureNavGridNode* ptr_test_navgrid_node_2 = dynamic_cast<CreatureNavGridNode*>(AddNavigationNodeUsingDefaultComponent(navgrid_point_center_2));

	printf("Adding third navgrid point!\n");
	//Test navigation grid creature
	Coordinates navgrid_point_center_3 = { 200,200 };
	CreatureNavGridNode* ptr_test_navgrid_node_3 = dynamic_cast<CreatureNavGridNode*>(AddNavigationNodeUsingDefaultComponent(navgrid_point_center_3));
	
	dynamic_cast<LevelComponentNavGrid*>(ptr_initial_navgrid_component)->ConnectNodes(400);
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