#include <Creature.hpp>

//***********************************
//DEFINITIONS OF STATIC CLASS MEMBERS
//***********************************

std::vector <Creature*> Creature::current_environment;
std::vector <CreatureType> Creature::walls = { cre_flying_box, cre_spell_open_doors };
Creature* Creature::ptr_current_main_charater;
const double Creature::MARGIN_FOR_LINE_OF_SIGHT_CHECKS = 48;
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
	PreciseRect visual_component_center = ptr_vector_drawing->TellPosition();
	//Hitbox == area occupied by vector drawing. No margin is set.
	InitializeHitbox(visual_component_center, 0);
	ptr_behavior = new Behavior();
	//printf("Hitbox is: x: %f y: %f w: %f h: %f.\n", hitbox.x, hitbox.y, hitbox.w, hitbox.h);
}

//Constructor spawning a creature around CENTER coordinates given
Creature::Creature(SpriteType my_sprite_type, Coordinates* ptr_my_center, int hitbox_margin)
{
	ptr_sprites_factory = new FactorySpawningSprites();
	ptr_behavior = new Behavior();
	Sprite* ptr_sprite = ptr_sprites_factory->SpawnSprite(my_sprite_type, ptr_my_center);
	SetMyVisualComponent(ptr_sprite);
	cyclic_actions.push_back(func_follow_physics);
	cyclic_actions.push_back(func_follow_behavior);
	cyclic_actions.push_back(func_play_current_animation_for_visual_components);
	cyclic_actions.push_back(func_kill_creature_if_its_time_expired);
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
	delete ptr_sprites_factory;
	delete ptr_behavior;
	delete ptr_time_left_to_live;
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


bool Creature::IsThisCreaturePresentInEnvironment(Creature* ptr_my_creature)
{
	bool result = false;

	if (std::find(current_environment.begin(), current_environment.end(), ptr_my_creature) != current_environment.end())
	{
		result = true;
	}
	return result;
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

void Creature::AddCreatureTypeToCollisionExceptions(CreatureType my_type)
{
	if (std::find(collision_check_exceptions.begin(), 
		collision_check_exceptions.end(), 
		my_type) == collision_check_exceptions.end())
	{
		collision_check_exceptions.push_back(my_type);
	}
}

void Creature::RemoveCreatureTypeFromCollisionExceptions(CreatureType my_type)
{
	collision_check_exceptions.erase(
		std::remove(collision_check_exceptions.begin(), 
		collision_check_exceptions.end(),
		my_type), 
		collision_check_exceptions.end()
	);
}

std::vector<CreatureType> Creature::TellCollisionExceptions()
{
	return collision_check_exceptions;
}

bool Creature::IsThisCreatureTypeAColliderForMe(CreatureType queried_type)
{
	if (std::find(collision_check_exceptions.begin(), 
		collision_check_exceptions.end(), 
		queried_type) != collision_check_exceptions.end())
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool Creature::DoesThisCreatureBelongToWalls()
{
	bool result = false;
	if (std::find(Creature::walls.begin(), Creature::walls.end(), my_type) != Creature::walls.end())
	{
		result = true;
	}
	return result;
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

double Creature::TellTurnVelocity()
{
	return turn_speed;
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

std::vector<Creature*> Creature::FindCreaturesInRadiusInSet(std::vector<Creature*>* ptr_my_creatures_set, Coordinates center_point, int radius)
{
	std::vector<Creature*> result;
	for (Creature* ptr_creature : *ptr_my_creatures_set)
	{
		Coordinates creatures_center = ptr_creature->TellCenterPoint();
		double distance = Distance::CalculateDistanceBetweenPoints(center_point, creatures_center);

		if (static_cast<int>(distance) <= radius)
		{
			result.push_back(ptr_creature);
		}
		//printf("Number of neighbours found: %d.\n", result.size());
	}
	return result;
}

std::vector<Creature*> Creature::FindCreaturesInAreaInSet(std::vector<Creature*>* ptr_my_creatures_set, PreciseRect my_area)
{
	/*This function finds Creatures existing in given set in given area.
	Useful for collissions finding.*/
	{
		//printf("Area to check: x: %f y: %f w: %f h: %f.\n", my_area.x, my_area.y, my_area.w, my_area.h);
		std::vector<Creature*> result;

		//printf("Set to be examined is sized %d.\n", ptr_my_creatures_set->size());
		for (Creature* ptr_creature : *ptr_my_creatures_set)
		{
			Coordinates creatures_center = ptr_creature->TellCenterPoint();

			if (Collisions::IsThisPointInsideRectangle(creatures_center, my_area))
			{
				result.push_back(ptr_creature);
			}
			else
			{
				//printf("Point outside rectangle x: %f y: %f.\n", creatures_center.x, creatures_center.y);
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

	/*if (this == Creature::ptr_current_main_charater)
	{
		printf("Hero movement: velocity: %f x: %f, y: %f, time passed: %f \n", velocity, x_shift_dbl, y_shift_dbl, time_passed);
	}*/

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
		//Moving behavior component for main character.
		MoveBehaviorComponent(-x, -y);
		//printf("Moving all map entities by x: %f, y: %f.\n", -x, -y);
        //printf("Checking main character collision.\n");
        if (DoICollideWithNeighbors())
        {
            //printf("Collision of main character detected\n");
            for (Creature* ptr_creature : current_environment)
            {
                if (ptr_creature != this) /* Prevents moving the main character. */
                {
                    ptr_creature->ShiftPositionAndRevertIfCollisionOccured(x,y,false); /* Reverting changes made in last step.*/
					ptr_creature->MoveBehaviorComponent(x, y);
				}
            }
			//Moving behavior component for main character.
			MoveBehaviorComponent(x, y);
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
	bool did_i_move_successfully = true;
    MoveComponents(x,y);
    if (check_collisions && DoICollideWithNeighbors())
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

void Creature::ThrustTowardsPoint(Coordinates destination, double velocity)
{
	TurnTowardsPoint(destination);
	ThrustForward(velocity);
}

void Creature::SetVelocity(double my_velocity)
{
	velocity = my_velocity;
}

double Creature::TellVelocity()
{
	return velocity;
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

std::vector<Coordinates> Creature::TellHitboxCorners()
{
	Coordinates corner_a = { hitbox.x, hitbox.y };
	Coordinates corner_b = { hitbox.x + hitbox.w, hitbox.y };
	Coordinates corner_c = { hitbox.x, hitbox.y + hitbox.h};
	Coordinates corner_d = { hitbox.x + hitbox.w, hitbox.y + hitbox.h};
	return {corner_a, corner_b, corner_c, corner_d};
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

	//Apart from the flag, creature may have its own exceptions preventing collision checks with certain types.
	//See attribute collision_check_exceptions in Creature definition.

	
	if (ptr_my_creature != this /* Prevents checking collision with itself. */ && 
		((ptr_my_creature->is_obstacle == true && check_only_obstacles==true) || (check_only_obstacles == false)) &&
		IsThisCreatureTypeAColliderForMe(ptr_my_creature->my_type) )
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

bool Creature::DoICollideWithNeighbors()
{
	//Determining environment - what neighbors do I have?
    RemoveNeighbors();
    my_neighbors = FindNeighborsInSet(&current_environment);
    //printf("Number of neighbors found in environment: %d.\n", my_neighbors.size());
    //printf("Environment size: %d.\n", current_environment.size());
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

bool Creature::IsThereLineOfSightBetweenThesePointsInCurrentEnvironment(Coordinates point_a, Coordinates point_b, 
	double max_line_length, 
	std::vector<Creature*> exceptions)
{
	/*Function checks if in current environment two points can be connected without intersecting any
	Creature's hitbox.
	
	max_line_length - what is the maximal distance between points to consider them as "in sight"
	exceptions - which creatures will be excluded from collisions check
	*/
	double distance_between_points = Distance::CalculateDistanceBetweenPoints(point_a, point_b);
	std::vector<Creature*> potential_colliding_creatures = {};

	//Finding potential colliding creatures.

	//Isn't the potential line of sight too long?
	if (max_line_length != 0 && max_line_length < distance_between_points)
	{
		return false;
	}

	PreciseRect area_to_check =
	{
		std::min(point_a.x, point_b.x) - MARGIN_FOR_LINE_OF_SIGHT_CHECKS,
		std::min(point_a.y, point_b.y) - MARGIN_FOR_LINE_OF_SIGHT_CHECKS,
		abs(point_a.x - point_b.x) + MARGIN_FOR_LINE_OF_SIGHT_CHECKS*2,
		abs(point_a.y - point_b.y) + MARGIN_FOR_LINE_OF_SIGHT_CHECKS*2,
	};

	//printf("Current environment holds %d creatures.\n", current_environment.size());
	potential_colliding_creatures = FindCreaturesInAreaInSet(&current_environment, area_to_check);

	//Checking for collisions

	for (Creature* ptr_neighbor : potential_colliding_creatures)
	{
		if (ptr_neighbor->DoesThisCreatureBelongToWalls()
			&& std::find(exceptions.begin(), exceptions.end(), ptr_neighbor) == exceptions.end()) //If creature wasn't listed in exceptions.
		{
			if (Collisions::DoesSegmentIntersectRectangle(point_a, point_b, ptr_neighbor->TellHitbox()))
			{
				return false;
			}
			else
			{
				;
			}
		}
	}
	/*printf("There is a line of sight between x:%f, y:%f, x: %f y: %f.\n",
		point_a.x, point_a.y, point_b.x, point_b.y);*/
	return true;
}

bool Creature::IsThereCorridorBetweenThesePointsInCurrentEnvironment(Coordinates point_a, Coordinates point_b, double corridor_width, double max_corridor_length)
{
	/*

orthogonal line crossing point A          orthogonal line crossing point B
             |                                    |
point a prim | -----------------------------------| point b prim \
			 |	                                  |               \
     point A *------------LINE_OF_SIGHT-----------* point B        } CORRIDOR WIDTH
	         |                                    |               /
point a bis  |------------------------------------| point b bis  /
			 |                                    |
	*/
	if (IsThereLineOfSightBetweenThesePointsInCurrentEnvironment(point_a, point_b, max_corridor_length) == false)
	{
		return false;
	}
	MathLineParams params_for_line_of_sight = MathLine::CalculateLineParams(point_a, point_b);
	//printf("Params for line of sight: a: %f b: %f.\n", params_for_line_of_sight.slope, params_for_line_of_sight.intercept);
	MathLineParams params_for_orthogonal_line_crossing_point_a =
		MathLine::CalculateParamsForOrthogonalLineCrossingGivenPoint(params_for_line_of_sight, point_a);
	//printf("Params for orthogonal line crossing a: a: %f b: %f.\n", params_for_orthogonal_line_crossing_point_a.slope, params_for_orthogonal_line_crossing_point_a.intercept);
	MathLineParams params_for_orthogonal_line_crossing_point_b =
		MathLine::CalculateParamsForOrthogonalLineCrossingGivenPoint(params_for_line_of_sight, point_b);
	//printf("Params for orthogonal line crossing b: a: %f b: %f.\n", params_for_orthogonal_line_crossing_point_b.slope, params_for_orthogonal_line_crossing_point_b.intercept);
	std::pair<double, double> satellite_xes_for_point_a = MathLine::CalculateXesForPointLyingOnGivenLineInGivenDistanceFromGivenX(
		params_for_orthogonal_line_crossing_point_a, corridor_width/2, point_a.x);
	std::pair<double, double> satellite_xes_for_point_b = MathLine::CalculateXesForPointLyingOnGivenLineInGivenDistanceFromGivenX(
		params_for_orthogonal_line_crossing_point_b, corridor_width / 2, point_b.x);
	Coordinates point_a_prim = {satellite_xes_for_point_a.first, MathLine::CalculateYForGivenX(params_for_orthogonal_line_crossing_point_a, satellite_xes_for_point_a.first)};
	Coordinates point_a_bis = { satellite_xes_for_point_a.second, MathLine::CalculateYForGivenX(params_for_orthogonal_line_crossing_point_a, satellite_xes_for_point_a.second)};
	Coordinates point_b_prim = { satellite_xes_for_point_b.first, MathLine::CalculateYForGivenX(params_for_orthogonal_line_crossing_point_b, satellite_xes_for_point_b.first) };
	Coordinates point_b_bis = { satellite_xes_for_point_b.second, MathLine::CalculateYForGivenX(params_for_orthogonal_line_crossing_point_b, satellite_xes_for_point_b.second) };
	
	/*printf("Point a: x: %f y: %f, point b: x: %f y: %f\n", point_a.x, point_a.y, point_b.x, point_b.y);
	printf("Point a prim: x: %f, y: %f\n", point_a_prim.x, point_a_prim.y);
	printf("Point a bis: x: %f, y: %f\n", point_a_bis.x, point_a_bis.y);
	printf("Point b prim: x: %f, y: %f\n", point_b_prim.x, point_b_prim.y);
	printf("Point b bis: x: %f, y: %f\n", point_b_bis.x, point_b_bis.y);*/

	//Checks
	if (IsThereLineOfSightBetweenThesePointsInCurrentEnvironment(point_a_prim, point_b_prim, max_corridor_length) == false)
	{
		return false;
	}
	if (IsThereLineOfSightBetweenThesePointsInCurrentEnvironment(point_a_prim, point_b_bis, max_corridor_length) == false)
	{
		return false;
	}
	if (IsThereLineOfSightBetweenThesePointsInCurrentEnvironment(point_a_bis, point_b_prim, max_corridor_length) == false)
	{
		return false;
	}
	if (IsThereLineOfSightBetweenThesePointsInCurrentEnvironment(point_a_bis, point_b_bis, max_corridor_length) == false)
	{
		return false;
	}
	//printf("There is a corridor!\n");
	return true;
}

bool Creature::IsThisCreatureWithinSightInCurrentEnvironment(Creature* ptr_other_creature, double distance_limit)
{
	/*Checking if there is a line of sight connecting two Creature's center points.*/
	Coordinates other_creature_center = ptr_other_creature->TellCenterPoint();
	Coordinates my_center = TellCenterPoint();

	std::vector<Creature*> exceptions_for_collisions_check = {ptr_other_creature, this};
	bool result = IsThereLineOfSightBetweenThesePointsInCurrentEnvironment(my_center, other_creature_center, distance_limit ,exceptions_for_collisions_check);
	return result;
}

Creature* Creature::FindClosestAccessibleCreatureOfGivenType(CreatureType desired_type, double distance_limit)
{
	Creature* result = nullptr;
	double shortest_distance = 0;
	printf("Distance limit for finding carrier: %f.\n", distance_limit);
	std::vector<Creature*> neighboring_creatures = FindNeighborsInSet(&current_environment, static_cast<int>(distance_limit));
	for (Creature* ptr_neighboring_creature : neighboring_creatures)
	{
		if (ptr_neighboring_creature->my_type == desired_type && ptr_neighboring_creature != this)
		{
			Coordinates my_center = TellCenterPoint();
			Coordinates other_creature_center = ptr_neighboring_creature->TellCenterPoint();
			printf("Found neighboring creature of desired type %p.\n", ptr_neighboring_creature);
			printf("X: %f, Y: %f\n", ptr_neighboring_creature->TellCenterPoint().x, ptr_neighboring_creature->TellCenterPoint().y);
			printf("My center: X: %f, Y:%f\n", TellCenterPoint().x, TellCenterPoint().y);
			if (IsThereCorridorBetweenThesePointsInCurrentEnvironment(my_center, other_creature_center, TellHitbox().w, distance_limit))
			{
				double distance = Distance::CalculateDistanceBetweenPoints(my_center, other_creature_center);
				if (shortest_distance == 0)
				{
					shortest_distance = distance;
					result = ptr_neighboring_creature;
				}
				else if (distance < shortest_distance)
				{
					shortest_distance = distance;
					result = ptr_neighboring_creature;
				}
				else
				{
					;
				}
			}
		}
	}
	return result;
}

//**************
// ANIMATIONS
//**************

void Creature::PlayCurrentAnimationsForVisualComponents()
{
	for (VisualComponent* ptr_my_visual_component : visual_components)
	{
		ptr_my_visual_component->PlayCurrentAnimation();
	}
}

//**************
//PATH REQUESTS
//**************

void Creature::PlaceRandomPathRequest(unsigned int path_length)
{
	RandomPathRequest my_request;
	my_request.requestor_id = this;
	my_request.requested_hops_length = path_length;
	random_path_requests.push_back(my_request);
	printf("%p placed a random path request.\n", this);
}

void Creature::PlacePointToPointPathRequest(Coordinates destination_point)
{
	PointToPointPathRequest my_request;
	my_request.requestor_id = this;
	my_request.my_position = TellCenterPoint();
	my_request.destination = destination_point;
	point_to_point_path_requests.push_back(my_request);
	printf("%p placed a point to point path request.\n", this);
}

void Creature::MakeUseOfPathResponse(RandomPathResponse my_response)
{
	printf("Will make use of received path response!\n");
	for (Coordinates coordinate : my_response.navigation_path)
	{
		CreatureSpawnRequest my_request;
		my_request.initial_center_cooridnates = coordinate;
		my_request.insertion_mode = merge;
		my_request.type = cre_navgrid_node;
		//#TODO - czy potrzebne to pole?
		my_request.mode = center_coordinates;
		//Green
		my_request.color = { 0,255,0,255 };
		PushIntoSpawnRequests(my_request);
	}
	ptr_behavior->MakeUseOfPathResponse(my_response);
}

void Creature::MakeUseOfPathResponse(PointToPointPathResponse my_response)
{
	printf("Will make use of received point to point path response!\n");
	for (Coordinates coordinate : my_response.navigation_path)
	{
		CreatureSpawnRequest my_request;
		my_request.initial_center_cooridnates = coordinate;
		my_request.insertion_mode = merge;
		my_request.type = cre_navgrid_node;
		//#TODO - czy potrzebne to pole?
		my_request.mode = center_coordinates;
		//Green
		my_request.color = { 0,255,0,255 };
		PushIntoSpawnRequests(my_request);
	}
	ptr_behavior->MakeUseOfPathResponse(my_response);
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

bool Creature::HasMyTimePassedOnThisWorld()
{
	if (ptr_time_left_to_live != nullptr)
	{
		if (ptr_time_left_to_live->CheckIfIntervalPassed())
		{
			printf("This guy's time on this world has passed - %p\n", this);
			return true;
		}
	}
	return false;
}

void Creature::SetTimeToLive(unsigned int seconds)
{
	unsigned int miliseconds = seconds * 1000;
	ptr_time_left_to_live = new TimerInterval(miliseconds);
}

void Creature::ResetTimeToLive()
{
	//#TODO - implement!;
}

void Creature::SetTimeToLiveToInfinity()
{
	delete ptr_time_left_to_live;
	ptr_time_left_to_live = nullptr;
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


void Creature::SetBehaviorMode(BehaviorMode behavior_to_be_set, Coordinates* ptr_my_destination)
{
	if (ptr_my_destination == nullptr)
	{
		ptr_behavior->SetMode(behavior_to_be_set);
	}
	else
	{
		ptr_behavior->SetMode(behavior_to_be_set, *ptr_my_destination);
	}
}

void Creature::SetBehaviorPattern(BehaviorPattern pattern_to_be_set, Creature* ptr_my_destiny)
{
	if (ptr_my_destiny == nullptr)
	{
		ptr_behavior->SetPattern(pattern_to_be_set);
	}
	else
	{
		ptr_behavior->SetPattern(pattern_to_be_set, ptr_my_destiny);
	}
}

void Creature::RequestBehaviorMode(BehaviorMode behavior_to_be_set, Coordinates* ptr_my_destination)
{
    if (ptr_my_destination == nullptr)
    {
	    ptr_behavior->RequestMode(behavior_to_be_set);
    }
    else
    {
	    ptr_behavior->RequestMode(behavior_to_be_set, *ptr_my_destination);
    }
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
