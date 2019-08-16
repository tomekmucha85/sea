#include <LevelTest.hpp>

LevelTest::LevelTest()
{

	SetMyType(level_test);
	Coordinates other_guy_position = { 50,300 };
	Creature* ptr_other_guy = ptr_initial_core_component->AddCreature(cre_carrier_a, &other_guy_position, force);
	ptr_other_guy->SetBehaviorMode(beh_wander_on_navmesh);

	//Trigger to win level
	/*std::string signal_to_win = "winning";
	PreciseRect event_area = {100,100,100,100};
	Creature* ptr_winning_trigger = AddTriggerUsingDefaultComponent(event_area, signal_to_win);
	signals_vs_events[signal_to_win] = ptr_func_win;*/
	//Red
	//ptr_winning_trigger->TellMainVisualComponent()->SetColor({ 255,0,0,255 });
	
	printf("Adding navgrid point!\n");
	//Test navigation grid creature
	Coordinates navgrid_point_center_1 = {600,700};
	CreatureNavGridNode* ptr_test_navgrid_node_1 = dynamic_cast<CreatureNavGridNode*>(AddNavigationNodeUsingDefaultComponent(navgrid_point_center_1));

	printf("Adding another navgrid point!\n");
	//Test navigation grid creature
	Coordinates navgrid_point_center_2 = { 100,700 };
	CreatureNavGridNode* ptr_test_navgrid_node_2 = dynamic_cast<CreatureNavGridNode*>(AddNavigationNodeUsingDefaultComponent(navgrid_point_center_2));

	printf("Adding third navgrid point!\n");
	//Test navigation grid creature
	Coordinates navgrid_point_center_3 = { 200,600 };
	CreatureNavGridNode* ptr_test_navgrid_node_3 = dynamic_cast<CreatureNavGridNode*>(AddNavigationNodeUsingDefaultComponent(navgrid_point_center_3));
	
	dynamic_cast<LevelComponentNavGrid*>(ptr_initial_navgrid_component)->ConnectNodes(400);
}

void LevelTest::FinishLevel(LevelEnding my_ending)
{
	Pause();
	if (my_ending == ending_victory)
	{
		printf("You've beaten test level!\n");
	}
	else if (my_ending == ending_defeat)
	{
		printf("You've been beaten.\n");
	}
	else
	{
		printf("Some other ending!\n");
	}
}