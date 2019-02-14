#include <Creature.hpp>

//***********************************
//DEFINITIONS OF STATIC CLASS MEMBERS
//***********************************

std::vector <Creature*> Creature::current_environment;
std::vector <CreatureType> Creature::walls = { cre_flying_box, cre_spell_open_doors };
Creature* Creature::ptr_current_main_charater;
//**************
//STATIC METHODS
//**************


//************
//DEBUG
//************

void Creature::PrintStupidThings(Creature* ptr_to_creature)
{
    printf("Dupa\n");
}

//************
//CONSTRUCTORS
//************


Creature::Creature(PreciseRect* ptr_area)
{
	//printf("Invisible creature constructed.\n");
	//Hitbox == ptr_area. No margin is set.
	VectorDrawing* ptr_vector_drawing = new VectorDrawing(ptr_area);
	SetMyVisualComponent(ptr_vector_drawing);
	InitializeHitbox(*ptr_area, 0);
	ptr_behavior = new Behavior();
	//printf("Hitbox is: x: %f y: %f w: %f h: %f.\n", hitbox.x, hitbox.y, hitbox.w, hitbox.h);

}

Creature::Creature(Coordinates* ptr_my_center)
{
	//printf("Invisible creature constructed.\n");
	VectorDrawing* ptr_vector_drawing = new VectorDrawing(ptr_my_center);
	SetMyVisualComponent(ptr_vector_drawing);
	PreciseRect visua_component_center = ptr_vector_drawing->TellPosition();
	//Hitbox == area occupied by vector drawing. No margin is set.
	InitializeHitbox(visua_component_center, 0);
	ptr_behavior = new Behavior();
	//printf("Hitbox is: x: %f y: %f w: %f h: %f.\n", hitbox.x, hitbox.y, hitbox.w, hitbox.h);
}

//Constructor spawning a creature around CENTER coordinates given
Creature::Creature(SpriteType my_sprite_type, Coordinates* ptr_my_center, int hitbox_margin)
{
	ptr_sprites_factory = new FactorySpawningSprites();
	ptr_behavior = new Behavior();
	cyclic_actions.push_back(func_follow_behavior);
	cyclic_actions.push_back(func_follow_physics);
	Sprite* ptr_sprite = ptr_sprites_factory->SpawnSprite(my_sprite_type, ptr_my_center);
	SetMyVisualComponent(ptr_sprite);
	//Set the initial value to move upwards by (velocity * pixels)
	next_step.y = velocity * -1;
	//Initialize hitbox
	PreciseRect sprite_position = ptr_sprite->TellSpritePosition();
	InitializeHitbox(sprite_position, hitbox_margin);
	//Write entry in static vector class_instances
}

//************
//DESTRUCTOR
//************

Creature::~Creature()
{
	//printf("Destructor called for Creature %p.\n", this);
	DeleteAllVisualComponents();
	if (ptr_sprites_factory != nullptr)
	{
		delete ptr_sprites_factory;
		delete ptr_behavior;
	}
}

void Creature::DeleteAllVisualComponents()
{
	std::vector<VisualComponent*> visual_components_copy = visual_components;
	for (VisualComponent* ptr_my_visual_component : visual_components_copy)
	{
		delete ptr_my_visual_component;
	}
	visual_components.clear();
}

//******************************************
//SETTING MAIN CHARACTER AND OPERATIONS ON MAIN INSTANCES ARRAY
//******************************************

void Creature::SetMainCharacterToNull()
{
    Creature::ptr_current_main_charater = NULL;
}

void Creature::MakeMeMainCharacter()
{
    SetMainCharacterToNull();
    Creature::ptr_current_main_charater = this;
}

void Creature::RemoveAllEntriesFromEnvironmentExceptMainHero()
{
	printf("Currently present creatures: %d.\n", static_cast<int>(current_environment.size()));
	for (Creature* ptr_creature : current_environment)
	{
		printf("Creature present in environment: %p.\n", ptr_creature);
	}
	current_environment.erase(std::remove_if(current_environment.begin(),current_environment.end(),
		[](Creature* ptr_creature) 
	    {   
		     if (ptr_creature->AmIMainCharacter())
	         {
				 printf("Not hero - %p.\n", ptr_creature);
		         return false;
	         }
	         else
	         {
				 printf("Hero! %p.\n", ptr_creature);
		         return true;
	         } 
	     }
	),current_environment.end());

	printf("And now: %d.\n", static_cast<int>(current_environment.size()));
	printf("Hero is: %p\n", ptr_current_main_charater);
	for (Creature* ptr_creature : current_environment)
	{
		printf("Creature present in environment: %p.\n", ptr_creature);
	}
}

bool Creature::AmIMainCharacter()
{
    if (Creature::ptr_current_main_charater == this)
    {
        return true;
    }
    else
    {
        return false;
    }
}

Creature* Creature::WhoIsMainCharacter()
{
    return Creature::ptr_current_main_charater;
}


PreciseRect Creature::TellHitbox()
{
	return hitbox;
}

//**************
//SETTING AND TELLING PARAMS
//**************

void Creature::SetMyVisualComponent(VisualComponent* ptr_my_visual_component)
{
	if (visual_components.size() > 0)
	{
		DeleteAllVisualComponents();
	}
	visual_components.push_back(ptr_my_visual_component);
}

void Creature::AddVisualComponent(VisualComponent* ptr_my_visual_component)
{
	visual_components.push_back(ptr_my_visual_component);
}

void Creature::SetMyRenderLayer(int layer_number)
{
	render_layer = layer_number;
}

void Creature::SetVisibility(bool should_be_visible)
{
	is_visible = should_be_visible;
}

bool Creature::AmIVisible()
{
	return is_visible;
}

int Creature::TellRenderLayer()
{
	return render_layer;
}

//**********
//COLLISIONS
//**********

void Creature::MakeMeObstacle()
{
	is_obstacle = true;
}

void Creature::MakeMeNotObstacle()
{
	is_obstacle = false;
}

void Creature::InitializeHitbox(PreciseRect sprite_position, int margin_percent)
{
    //Protection against margin covering whole hitbox
    if (margin_percent >= 50)
    {
        throw std::invalid_argument("Incorrect argument passed to Creature::InitializeHitbox. Hitbox margin would consume whole hitbox.");
    }
    double x_margin = sprite_position.w*(static_cast<double>(margin_percent)/100);
    double y_margin = sprite_position.h*(static_cast<double>(margin_percent) /100);
    hitbox.x = sprite_position.x + x_margin;
    hitbox.w = sprite_position.w - x_margin;
    hitbox.y = sprite_position.y + y_margin;
    hitbox.h = sprite_position.h - y_margin;
    //printf("Hitbox is initialized at: x: %f, y: %f, w: %f, h: %f\n", hitbox.x, hitbox.y, hitbox.w, hitbox.h);
}

//********
//MOVEMENT
//********

void Creature::Turn(int turn_angle_degree)
{
	//printf("Current angle degree: %d.\n", current_angle_degree);
	//printf("Turning by %d degrees.\n", turn_angle_degree);
    // #TODO! mno¿yæ przez prêdkoœæ jeszcze przed wywo³aniem funkcji.
    current_angle_degree += static_cast<int>(turn_angle_degree * turn_speed);
	current_angle_degree = Angle::NormalizeAngle(current_angle_degree);
	for (VisualComponent* ptr_my_visual_component : visual_components)
	{
		ptr_my_visual_component->TurnByAngleDegrees(static_cast<int>(turn_quant_degree * turn_direction * turn_speed));
	}
	//printf("Current angle after normalization: %d.\n", current_angle_degree);
}

Coordinates Creature::TellNextStep()
{
	return next_step;
}


void Creature::TurnRight()
{
	//if (turn_direction == 0)
		turn_direction = 1;
   	/*printf("Sprite center: x: %f, y: %f.\n", ptr_creature_visual_component->center.x, ptr_creature_visual_component->center.y);
    printf("Sprite position: x: %f, y: %f, w: %f, h: %f\n", ptr_creature_visual_component->position.x, ptr_creature_visual_component->position.y,
		ptr_creature_visual_component->position.w, ptr_creature_visual_component->position.h);
	printf("Sprite angle: %f.\n", ptr_creature_visual_component->angle);
	printf("Creature hitbox: x: %f, y: %f, w: %f, h: %f.\n", hitbox.x, hitbox.y, hitbox.w, hitbox.h);
	printf("Creature hitbox center: x: %f y: %f.\n", hitbox.x + (hitbox.w /2), hitbox.y + (hitbox.h /2));*/
}


void Creature::TurnLeft()
{
	//if(turn_direction == 0)
		turn_direction = -1;
	/*printf("Sprite center: x: %f, y: %f.\n", ptr_creature_visual_component->center.x, ptr_creature_visual_component->center.y);
	printf("Sprite position: x: %f, y: %f, w: %f, h: %f\n", ptr_creature_visual_component->position.x, ptr_creature_visual_component->position.y,
		ptr_creature_visual_component->position.w, ptr_creature_visual_component->position.h);
	printf("Sprite angle: %f.\n", ptr_creature_visual_component->angle);
	printf("Creature hitbox: x: %f, y: %f, w: %f, h: %f.\n", hitbox.x, hitbox.y, hitbox.w, hitbox.h);
	printf("Creature hitbox center: x: %f y: %f.\n", hitbox.x + (hitbox.w / 2), hitbox.y + (hitbox.h / 2));*/
}

void Creature::TurnStop() 
{
	turn_direction = 0;
}

void Creature::RemoveNeighbors()
{
	my_neighbors.clear();
}

void Creature::AddToNeighbors(std::vector<Creature*> new_neighbors)
{
	my_neighbors.insert(std::end(my_neighbors), std::begin(new_neighbors), std::end(new_neighbors));
}

std::vector<Creature*> Creature::FindNeighborsInSet(std::vector<Creature*>* ptr_my_creatures_set, int radius)
/*This function limits number of objects against which collision checks will be performed.
Only objects with hitboxes within default_neighbor_radius will be checked.*/
{
	std::vector<Creature*> result;
	if (radius == NULL)
	{
		radius = default_neighbor_radius;
		//printf("Radius equals %d.\n", radius);
	}
	//printf("Set to be examined is sized %d.\n", ptr_my_creatures_set->size());
	Coordinates my_center = TellCenterPoint();
	for (Creature* ptr_creature : *ptr_my_creatures_set)
	{
		Coordinates creatures_center = ptr_creature->TellCenterPoint();
		double distance = Distance::CalculateDistanceBetweenPoints(my_center, creatures_center);

		if (static_cast<int>(distance) <= radius)
		{
			result.push_back(ptr_creature);
		}
		//printf("Number of neighbours found: %d.\n", result.size());
	}
	return result;
}

std::vector<Creature*> Creature::FindNeighborsInAreaInSet(std::vector<Creature*>* ptr_my_creatures_set, PreciseRect my_area)
{
	/*This function finds Creatures existing in given set in given area.
	Useful for collissions finding.*/
	{
		std::vector<Creature*> result;

		//printf("Set to be examined is sized %d.\n", ptr_my_creatures_set->size());
		Coordinates my_center = TellCenterPoint();
		for (Creature* ptr_creature : *ptr_my_creatures_set)
		{
			Coordinates creatures_center = ptr_creature->TellCenterPoint();

			if (Collisions::IsThisPointInsideRectangle(creatures_center, my_area))
			{
				result.push_back(ptr_creature);
			}
			//printf("Number of neighbours found: %d.\n", result.size());
		}
		return result;
	}
}

std::vector<Creature*> Creature::FindCollisionsInSet(std::vector<Creature*>* ptr_my_creatures_set, bool check_only_obstacles)
{
	std::vector<Creature*> result = {};
	//printf("I am %p. I will find collisions with me in set %p consisting of %d items.\n", this, ptr_my_creatures_set, ptr_my_creatures_set->size());
	for (Creature* ptr_creature : *ptr_my_creatures_set)
	{
		if (DoICollideWithThisCreature(ptr_creature, check_only_obstacles))
		{
			//printf("Found collision in set!\n");
			result.push_back(ptr_creature);
		}
	}
	//printf("Found %d collisions of %p in set %p.\n", result.size(), this, ptr_my_creatures_set);
	return result;
}

void Creature::DetermineNextStep(double time_passed)
{
	//#TODO - przerobiæ next_step na wektor
	//printf("Time passed: %f.\n", time_passed);
	double current_angle_radian = Angle::DegreeToRadian(current_angle_degree);
	                                                             //Miliseconds into seconds
	double x_shift_dbl = sin(current_angle_radian) * velocity * (time_passed * 0.001);
	double y_shift_dbl = cos(current_angle_radian) * velocity * (time_passed * 0.001);
	next_step.x = x_shift_dbl;
	//Multiplied by -1 because negative value means moving upward the screen
	next_step.y = y_shift_dbl * -1;
	//    printf ("Turn degree is %d which equals %f radians\n", current_angle_degree, current_angle_radian);
	//    printf("Next step coordinates:\n");
	//    printf("X shift is %f\n", next_step.x);
	//    printf("Y shift is %f\n", next_step.y);
}

bool Creature::Move(double x, double y)
{
//Moves this object - in case it's a non-player object.
//Moves all the other creatures in case we're dealing with player object. This way screen scroll is achieved.
//Returns true if no collision occured or false, when creature collided with somehting.
    //printf("========[Move called by %p.]=============\n", this);

	bool did_i_move_successfully = true;
	//Determining environment - what neighbors do I have?
	RemoveNeighbors();
	std::vector<Creature*>current_neighbors = FindNeighborsInSet(&current_environment);
	//printf("Number of neighbors found in environment: %d.\n", current_neighbors.size());
	//printf("Environment size: %d.\n", current_environment.size());
	AddToNeighbors(current_neighbors);

	//If Main Character Creature is moved, in fact it stays in place and all the other Creatures on given level are moved.
    if (AmIMainCharacter())
    {
        //Now we're C++11 as fuck!
        for (Creature* ptr_creature : current_environment)
        {
            if (ptr_creature != this) /* Prevents moving the main character. */
            {
                ptr_creature->ShiftPositionAndRevertIfCollisionOccured(-x,-y,false);
				ptr_creature->MoveBehaviorComponent(-x, -y);
            }
        }
		//printf("Moving all map entities by x: %f, y: %f.\n", -x, -y);
        //printf("Checking main character collision.\n");
        if (DoICollideWithNeighbors())
        {
           //printf("Collision of main character detected\n");
            for (Creature* ptr_creature : current_environment)
            {
                if (ptr_creature != this) /* Prevents moving the main character. */
                {
                    //#TODO - consider snapshot approach so less calculations will be needed.
                    ptr_creature->ShiftPositionAndRevertIfCollisionOccured(x,y,false); /* Reverting changes made in last step.*/
                }
            }
			did_i_move_successfully = false;
        }
        else
        {
			;
        }
    }
	//If any other Creature than main character is moved.
    else
    {
        did_i_move_successfully = ShiftPositionAndRevertIfCollisionOccured(x,y,true);
    }
	return did_i_move_successfully;
}

bool Creature::ShiftPositionAndRevertIfCollisionOccured(double x, double y, bool check_collisions)
//Returns true is no collision occurred, or false in case of a collision.
{
	//#TODO - dorobiæ dzielenie ruchu na standardowe jednostki
	bool did_i_move_successfully = true;
    MoveComponents(x,y);
    if (DoICollideWithNeighbors() && check_collisions)
    {
        MoveComponents(-x,-y);
		did_i_move_successfully = false;
    }
	return(did_i_move_successfully);
}

void Creature::MoveComponents(double x, double y)
{
	MoveVisualComponent(x, y);
    MoveHitbox(x, y);
}

void Creature::MoveVisualComponent(double x, double y)
{
	for (VisualComponent* ptr_my_visual_component : visual_components)
	{
		ptr_my_visual_component->Move(x, y);
	}
}

void Creature::MoveHitbox(double x, double y)
{
    hitbox.x += x;
    hitbox.y += y;
    /*printf("New hitbox coordinates for %p: x: %f y: %f\n", this, hitbox.x, hitbox.y);
	printf("Sprite position: x: %f, y: %f.\n", this->ptr_creature_visual_component->position.x,
		this->ptr_creature_visual_component->position.y);
	printf("X offset: %f, Y: offset: %f.\n", hitbox.x - this->ptr_creature_visual_component->position.x,
		hitbox.y - this->ptr_creature_visual_component->position.y);*/
}

void Creature::MoveBehaviorComponent(double x, double y)
{
	if (ptr_behavior != nullptr)
	{
		//#TODO przerobiæ wszystkie funkcje Movexxx tak, by przyjmowa³y Coordinates.
		ptr_behavior->Move({ x,y });
	}
}

void Creature::ThrustForward(double velocity)
{
	if (velocity)
	{
		SetVelocity(velocity);
	}
	else
	{
		SetVelocity(default_velocity);
	}
}

void Creature::ThrustBackward(double velocity)
{
	if (velocity)
	{
		SetVelocity(-velocity);
	}
	else
	{
		SetVelocity(-default_velocity);
	}
}

void Creature::TurnTowardsPoint(Coordinates point)
{
	Coordinates my_center = TellCenterPoint();
	//printf("Running towards: x: %f, y: %f.\n", point.x, point.y);
	//Vector pointing north
	MathVector* ptr_vector_pointing_north = new MathVector(my_center.x, my_center.y, my_center.x, my_center.y - 100);
	//Vector pointing towards given point
	MathVector* ptr_vector_pointing_towards_given_point = new MathVector(my_center.x, my_center.y, point.x, point.y);

	double angle_between_vectors_radian = MathVector::TellRadianAngleBetweenVectors(*ptr_vector_pointing_north, *ptr_vector_pointing_towards_given_point);
	int angle_between_vectors_degrees = Angle::RadianToDegree(angle_between_vectors_radian);
	
	//Calculate if angle should be positive or negative
	int real_angle_between_vectors_degrees = 0;
	if (ptr_vector_pointing_towards_given_point->TellValue().x <= 0)
	{
		real_angle_between_vectors_degrees = 360 - angle_between_vectors_degrees;
	}
	else
	{
	    real_angle_between_vectors_degrees = angle_between_vectors_degrees;
	}

	//Set desired angle
	if (current_angle_degree != real_angle_between_vectors_degrees)
	{
		SetAngleDegree(real_angle_between_vectors_degrees);
	}
	// Cleanup
	delete ptr_vector_pointing_north;
	delete ptr_vector_pointing_towards_given_point;
}

void Creature::ThrustTowardsPoint(Coordinates destination)
{
	TurnTowardsPoint(destination);
	ThrustForward();
}

void Creature::SetVelocity(double my_velocity)
{
	velocity = my_velocity;
}

//#TODO! - naprawiæ strafe
bool Creature::Strafe(int sidestep_angle)
{
	bool did_i_move_successfully = true;
    Coordinates next_step_cache = next_step;
    double strafing_angle = Angle::DegreeToRadian(current_angle_degree+sidestep_angle);
    double x_shift_dbl = sin(strafing_angle) * velocity * Timer::loop_duration;
    double y_shift_dbl = cos(strafing_angle) * velocity * Timer::loop_duration;
    next_step.x = (int) x_shift_dbl;
    //Multiplied by -1 because negative value means moving upward the screen
    next_step.y = (int) y_shift_dbl * -1;
    did_i_move_successfully = Move(next_step.x, next_step.y);
    next_step = next_step_cache;
	return did_i_move_successfully;
}

bool Creature::StrafeLeft()
{
	bool did_i_move_successfully = true;
    did_i_move_successfully = Strafe(-90);
	return did_i_move_successfully;
}

bool Creature::StrafeRight()
{
	bool did_i_move_successfully = true;
	did_i_move_successfully = Strafe(90);
	return did_i_move_successfully;
}


void Creature::SetPosition(Coordinates new_center_position)
{
	for (VisualComponent* ptr_my_visual_component : visual_components)
	{
		ptr_my_visual_component->SetCenter(new_center_position);
	}
}

int Creature::TellCurrentAngleDegree()
{
	return current_angle_degree;
}

Coordinates Creature::TellCenterPoint()
{
	Coordinates result = {hitbox.x+(hitbox.w/2), hitbox.y+(hitbox.w/2)};
	return result;
}

void Creature::SetAngleDegree(int my_degree)
{
	current_angle_degree = my_degree;
	for (VisualComponent* ptr_my_visual_component : visual_components)
	{
		ptr_my_visual_component->SetAngleDegrees(my_degree);
	}
}

Coordinates Creature::CalculatePointInGivenDistanceFromCreatureCenter(unsigned int distance)
{
	PreciseRect current_coordinates = TellHitbox();
	Coordinates result = {0,0};
	double creature_center_x = current_coordinates.x + (current_coordinates.w / 2);
	double creature_center_y = current_coordinates.y + (current_coordinates.h / 2);
	int current_angle_degree = TellCurrentAngleDegree();
	double current_angle_radian = Angle::DegreeToRadian(current_angle_degree);
	/*
	
	Notice, that the grid is as following:

315degrees     45 degrees
		  \	| /
	       \|/
	------------------->X
	        |0,0
			|
	        V
            Y
	*/

	result.x = creature_center_x + (sin(current_angle_radian) * distance);
	result.y = creature_center_y + (cos(current_angle_radian) * distance * -1);


	//printf("Calculated point in distance of %d from creature center x: %d y: %d angle: %d was x: %d y: %d.\n",
	//	distance, creature_center_x, creature_center_y, current_angle_degree, result.x, result.y);

	return result;
}

//**********
//COLLISIONS
//**********


bool Creature::DoICollideWithThisCreature(Creature* ptr_my_creature, bool check_only_obstacles)
{
	bool result = false;
	//printf("DoICollideWithNeighbors called for %p.\n", this);

	//Checking collisions with creatures.
	//Depending on flag "check_only_obstacles":
	//    - collisions are checked only with Creature entities with "is_obstacle" flag set to true 
	//    - or with all Creature beings.

	if (ptr_my_creature != this /* Prevents checking collision with itself. */ && 
		((ptr_my_creature->is_obstacle == true && check_only_obstacles==true)) || (check_only_obstacles == false))
	{

		if (Collisions::DoTheseRectanglesOverlap(hitbox, ptr_my_creature->TellHitbox()))
		{
			result = true;
		}
	}
	return result;
}

std::vector<Creature*> Creature::WhichNeighborsDoICollideWith()
{
	std::vector<Creature*> result = {};
	for (Creature* ptr_neighbor : my_neighbors)
	{
		if (DoICollideWithThisCreature(ptr_neighbor))
		{
			result.push_back(ptr_neighbor);
		}
	}
	return result;
}

bool Creature::DoICollideWithNeighbors(int margin)
{
    bool result = false;
    //printf("DoICollideWithNeighbors called for %p.\n", this);
	for (Creature* ptr_creature : this->my_neighbors)
	{
		if (ptr_creature != this /* Prevents checking collision with itself. */ && ptr_creature->is_obstacle == true)
		{
			if (DoICollideWithThisCreature(ptr_creature))
			{
				result = true;
			}
		}
	}
	return result;
}

bool Creature::IsThisCreatureWithinSight(Creature* ptr_other_creature, double distance_limit)
{
	Coordinates other_creature_center = ptr_other_creature->TellCenterPoint();
	Coordinates my_center = TellCenterPoint();
	double distance_between_creatures = Distance::CalculateDistanceBetweenPoints(my_center, other_creature_center);
	RemoveNeighbors();
	std::vector<Creature*>current_neighbors = {};
	if (distance_limit == 0)
	{
		current_neighbors = FindNeighborsInSet(&current_environment, static_cast<int>(distance_between_creatures));
	}
	else
	{
		PreciseRect area_to_check = 
		{
			std::min(my_center.x, other_creature_center.x),
			std::min(my_center.y, other_creature_center.y),
			abs(my_center.x - other_creature_center.x),
			abs(my_center.y - other_creature_center.y)
		};
		if (distance_limit < distance_between_creatures)
		{
			return false;
		}
		current_neighbors = FindNeighborsInAreaInSet(&current_environment, area_to_check);
	}

	for (Creature* ptr_neighbor : current_neighbors)
	{
		if (ptr_neighbor != ptr_other_creature && ptr_neighbor->is_obstacle == true)
		{
			if (Collisions::DoesSegmentIntersectRectangle(my_center, other_creature_center, ptr_neighbor->TellHitbox()))
			{
				return false;
			}
		}

	}
	return true;
}

//**************
//SPAWN REQUESTS
//**************

void Creature::PushIntoSpawnRequests(CreatureSpawnRequest my_request)
{
	spawn_requests.push_back(my_request);
}


//**************
//CYCLIC ACTIONS
//**************

void Creature::PerformCyclicActions()
{
	for (std::function<void(Creature*)> action : cyclic_actions)
	{
		action(this);
	}
}

void Creature::AddCyclicAction(std::function<void(Creature*)> my_cyclic_action)
{
	cyclic_actions.push_back(my_cyclic_action);
}

std::vector<CreatureSpawnRequest>* Creature::TellSpawnRequests()
{
	return &spawn_requests;
}

//***********************************
//MATTER OF LIFE AND DEATH
//***********************************

void Creature::Kill()
{
	am_i_alive = false;
};

void Creature::Resurrect()
{
	am_i_alive = true;
}

bool Creature::AmIAlive()
{
	return am_i_alive;
}

//********************************************
//PHYSICS
//********************************************

void Creature::FollowPhysics()
{
	// Forward/backward move
	if (velocity != 0)
	{
		double time_passed = Timer::loop_duration;
		DetermineNextStep(time_passed);
		Move(next_step.x, next_step.y);

		/*if (this == Creature::ptr_current_main_charater)
		{
			printf("Main character has velocity of %f.\n", velocity);
			printf("Next step x: %f, y: %f.\n");
		}*/
	}

	// Rotation
	Turn(turn_quant_degree * turn_direction);
}


//********************************************
//BEHAVIOR
//********************************************


void Creature::SetBehaviorMode(BehaviorMode behavior_to_be_set)
{
	ptr_behavior->SetMode(behavior_to_be_set);
}

void Creature::FollowBehavior()
{
	ptr_behavior->WhatToDo(this);
}

//********************************************
//MAGIC
//********************************************

void Creature::CastSpell(SpellName my_spell_name)
{
	Magic::CastSpell(my_spell_name, this);
}

void Creature::ChangeManaLevel(int change_amount)
{
	mana += change_amount;
}

int Creature::TellManaLevel()
{
	return mana;
}

void Creature::SetManaLevel(int new_level)
{
	mana = new_level;
}

//********************************************
//DEFAULT IMPLEMENTATIONS OF VIRTUAL FUNCTIONS
//********************************************

void Creature::ArmTrigger()
{
	printf("Default implementation of ArmTrigger called!\n");
}

void Creature::DisarmTrigger()
{
	printf("Default implementation of DisarmTrigger called!\n");
}

std::string Creature::GiveSignal()
{
	printf("Default implementation of GiveSignal called!\n");
	return std::string("");
}

bool Creature::AmIArmed()
{
	printf("Default implementation of AmIArmed called!\n");
	return true;
}
/*
void Creature::MakeDisposable()
{
	printf("Default implementation of MakeDisposable called!\n");;
}

void Creature::MakePermanent()
{
	printf("Default implementation of MakePermanent called!\n");
}

bool Creature::AmIDisposable()
{
	printf("Default implementation of AmIDisposable called!\n");
	return true;
}
*/