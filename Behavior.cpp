#include <Creature.hpp>

Behavior::Behavior()
{
	ptr_navigators_factory = new FactorySpawningNavigators();
}

Behavior::~Behavior()
{
	delete ptr_navigators_factory;
	delete ptr_navigator;
}

void Behavior::WhatToDo(Creature* ptr_my_creature)
{
	//Main character should not have any AI behavior.
	if (ptr_my_creature != Creature::ptr_current_main_charater)
	{
		if (mode == beh_idle)
		{
			if (ptr_my_creature->my_type == cre_spell_ball)
			{
				//#TODO - abstrakcja animacji wy¿ej?
				//#TODO - niebezpieczne za³o¿enie z castem!
				Sprite* ptr_sprite = dynamic_cast<Sprite*>(ptr_my_creature->visual_components[0]);
				ptr_sprite->IdleAnimation();
			}
		}
		else if (mode == beh_chase_hero)
		{
			Coordinates my_center = ptr_my_creature->TellCenterPoint();
			Coordinates hero_center = Creature::ptr_current_main_charater->TellCenterPoint();
			double distance_from_hero = Distance::CalculateDistanceBetweenPoints(my_center, hero_center);
			if (distance_from_hero < 100)
			{
				ptr_my_creature->ThrustTowardsPoint(hero_center);
			}
			else
			{
				ptr_my_creature->SetVelocity(0);
			}
		}
		else if (mode == beh_wander_on_navmesh)
		{
			Coordinates my_center = ptr_my_creature->TellCenterPoint();
			if (ptr_navigator == nullptr)
			{
				ptr_my_creature->PlaceRandomPathRequest(10);
			}
			else
			{
				if (ptr_navigator->TellMyState() == active)
				{
					ptr_my_creature->ThrustTowardsPoint(ptr_navigator->TellCurrentWaypoint());
					if (ptr_navigator->WasCurrentWaypointReached(my_center))
					{
						ptr_navigator->SetNextWaypoint();
						ptr_my_creature->ThrustTowardsPoint(ptr_navigator->TellCurrentWaypoint());
					}
				}
				else
				{
					ptr_my_creature->SetVelocity(0);
				}
			}
		}
		else if (mode == beh_run_along_predefined_path)
		{
			Coordinates my_center = ptr_my_creature->TellCenterPoint();
			std::vector<Coordinates> example_path = { {0,0}, {640,0}, {640,480}, {0,480} };
			Coordinates anchor = {0,0};
			if (ptr_navigator == nullptr)
			{
				printf("Spawning a Navigator!\n");
				printf("Anchor point x: %f, y: %f.\n", anchor.x, anchor.y);
				ptr_navigator = ptr_navigators_factory->SpawnNavigator(navig_coordinates_list, 
					anchor, example_path, false);
				ptr_my_creature->ThrustTowardsPoint(ptr_navigator->TellCurrentWaypoint());
			}
			if (ptr_navigator->TellMyState() == active)
			{
				ptr_my_creature->ThrustTowardsPoint(ptr_navigator->TellCurrentWaypoint());
				if (ptr_navigator->WasCurrentWaypointReached(my_center))
				{
					ptr_navigator->SetNextWaypoint();
					ptr_my_creature->ThrustTowardsPoint(ptr_navigator->TellCurrentWaypoint());
				}
			}
			else
			{
				ptr_my_creature->SetVelocity(0);
			}
		}
		else if (mode == beh_run_in_circles)
		{
			Coordinates my_center = ptr_my_creature->TellCenterPoint();
			if (ptr_navigator == nullptr)
			{
				printf("Spawning a Navigator!\n");
				printf("Center point x: %f, y: %f.\n", my_center.x, my_center.y);
				ptr_navigator = ptr_navigators_factory->SpawnNavigator(navig_circle, my_center, 300, 15);
				ptr_navigator->SetNextWaypoint();
				ptr_my_creature->ThrustTowardsPoint(ptr_navigator->TellCurrentWaypoint());
			}
			ptr_my_creature->ThrustTowardsPoint(ptr_navigator->TellCurrentWaypoint());
			if (ptr_navigator->WasCurrentWaypointReached(my_center))
			{
				ptr_navigator->SetNextWaypoint();
				ptr_my_creature->ThrustTowardsPoint(ptr_navigator->TellCurrentWaypoint());
			}
		}
		else if (mode == beh_projectile)
		{
			ptr_my_creature->ThrustForward();
		}
		else if (mode == beh_test_movement)
		{
			static int i = 0;
			if (i % 5 == 0)
			{
				ptr_my_creature->Move(0, -5);
			}
			i++;

		}
		else if (mode == beh_wander_on_navmesh)
		{
			;
		}
	}
}

void Behavior::SetMode(BehaviorMode mode_to_be_set)
{
	mode = mode_to_be_set;
	delete ptr_navigator;
}

//#TODO - ucywilizowaæ
void Behavior::Move(Coordinates movement)
{
	if (ptr_navigator != nullptr)
	{
		ptr_navigator->Move(movement);
	}
}


void Behavior::MakeUseOfPathResponse(RandomPathResponse my_response)
{
	//#TODO! Ogarn¹æ kasowanie nawigatorów!
	delete ptr_navigator;
	//#TODO - czy towrzenie anchora potrzebne?
	Coordinates anchor = { 0, 0 };
	printf("Received plan from nav grid. Plan:\n");
	for (Coordinates point : my_response.navigation_path)
	{
		printf("x: %f y: %f\n", point.x, point.y);
	}
	if (my_response.navigation_path.size() != 0)
	{
		ptr_navigator = ptr_navigators_factory->SpawnNavigator(navig_coordinates_list,
			anchor, my_response.navigation_path, false);
	}
	else
	{
		printf("Received empty path!\n");
	}
}