#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdexcept>
#include <algorithm>
#include <Creature.hpp>
//#TODO - oczyœciæ importy w pliku cpp


//***********************************
//DEFINITIONS OF STATIC CLASS MEMBERS
//***********************************

std::vector <Creature*> Creature::current_environment;
std::vector <CreatureType> Creature::walls = { cre_flying_box, cre_spell_open_doors };
Creature* Creature::ptr_current_main_charater;
long long int Creature::main_character_shift_x = 0;
long long int Creature::main_character_shift_y = 0;
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


// #TODO - czy potrzebne?
Creature::Creature(PreciseRect* ptr_area)
{
	printf("Invisible creature constructed.\n");
	//Hitbox == ptr_area. No margin is set.
	InitializeHitbox(*ptr_area, 0);
	printf("Hitbox is: x: %f y: %f w: %f h: %f.\n", hitbox.x, hitbox.y, hitbox.w, hitbox.h);

}

//Constructor spawning a creature around CENTER coordinates given
Creature::Creature(SpriteType my_sprite_type, Coordinates* ptr_my_center, int hitbox_margin)
{
	ptr_sprites_factory = new FactorySpawningSprites();
	ptr_behavior = new Behavior();
	cyclic_actions.push_back(func_follow_behavior);
	cyclic_actions.push_back(func_follow_physics);
	SetMySprite(ptr_sprites_factory->SpawnSprite(my_sprite_type, ptr_my_center));
	//Set the initial value to move upwards by (velocity * pixels)
	next_step.y = velocity * -1;
	//Initialize hitbox
	//#TODO//Change this, so position will be determined by creature
	PreciseRect sprite_position = ptr_creature_sprite->TellSpritePosition();
	InitializeHitbox(sprite_position, hitbox_margin);
	//Write entry in static vector class_instances
}

//************
//DESTRUCTOR
//************

Creature::~Creature()
{
	//printf("Destructor called for Creature %p.\n", this);
	//printf("Attempting to remove sprite %p.\n", ptr_creature_sprite);
	if (ptr_creature_sprite != nullptr)
	{
		delete ptr_creature_sprite;
	}
	if (ptr_creature_vector != nullptr)
	{
		delete ptr_creature_vector;
	}
	if (ptr_sprites_factory != nullptr)
	{
		delete ptr_sprites_factory;
	}
}

//****************************
//MANAGING ALL CLASS INSTANCES
//****************************

//**********************
//SETTING ENVIRONMENT
//**********************

//******************************************
//SETTING MAIN CHARACTER AND RECORDING SHIFT
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

long long int Creature::TellXMainCharacterShift()
{
	return main_character_shift_x;
}

long long int Creature::TellYMainCharacterShift()
{
	return main_character_shift_y;
}

void Creature::SetXMainCharacterShift(long long int my_shift)
{
	main_character_shift_x = my_shift;
}

void Creature::SetYMainCharacterShift(long long int my_shift)
{
	main_character_shift_y = my_shift;
}

void Creature::IncrementXMainCharacterShift(long long int my_shift)
{
	main_character_shift_x += my_shift;
}

void Creature::IncrementYMainCharacterShift(long long int my_shift)
{
	main_character_shift_y += my_shift;
}

PreciseRect Creature::TellHitbox()
{
	return hitbox;
}

//**************
//SETTING AND TELLING PARAMS
//**************

void Creature::SetMySprite(Sprite* ptr_my_sprite)
{
	if (ptr_creature_sprite != NULL)
	{
		delete ptr_creature_sprite;
		printf("SetMySprite deleted current sprite object.");
	}
	ptr_creature_sprite = ptr_my_sprite;
}

void Creature::SetMyRenderLayer(int layer_number)
{
	render_layer = layer_number;
}

void Creature::SetMyVector(PreciseRect* ptr_my_area)
{
	ptr_creature_vector = new VectorDrawing(ptr_my_area);
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
    //printf("Hitbox is: x: %f, y: %f, w: %f, h: %f\n", hitbox.x, hitbox.y, hitbox.w, hitbox.h);
}

//********
//MOVEMENT
//********

double Creature::DegreeToRadian (int angle_degree)
{
    double angle_radian = (angle_degree * PI) / 180;
    return angle_radian;
}

void Creature::Turn(int turn_angle_degree)
{
	//printf("Current angle degree: %d.\n", current_angle_degree);
	//printf("Turning by %d degrees.\n", turn_angle_degree);
    current_angle_degree += turn_angle_degree;
    current_angle_degree = NormalizeAngle(current_angle_degree);
	//printf("Current angle after normalization: %d.\n", current_angle_degree);
}


Coordinates Creature::TellNextStep()
{
	return next_step;
}

//Preventing angle from exceeding 360 degrees

int Creature::NormalizeAngle(int angle)
{
	if (angle < 0)
	{
		angle = 360 + angle;
	}
	else if (angle >= 360)
	{
		angle = 360 - angle;
	}
	else if (angle >= 2 * 360 || angle <= -2 * 360)
	{
		printf("Angle exceeds 360 twice! Something went wrong.\n");
		throw std::invalid_argument("Angle outside acceptable range.\n");
	}
	else
	{
		;
	}
    return angle;
}

void Creature::TurnRight()
{
    Turn(turn_quant_degree);
    ptr_creature_sprite->angle += turn_quant_degree;
}


void Creature::TurnLeft()
{
    Turn(turn_quant_degree * -1);
    ptr_creature_sprite->angle -= turn_quant_degree;
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
	double my_middle_x = this->hitbox.x + (this->hitbox.w / 2);
	double my_middle_y = this->hitbox.y + (this->hitbox.h / 2);
	for (Creature* ptr_creature : *ptr_my_creatures_set)
	{
		double creature_middle_x = ptr_creature->hitbox.x + (ptr_creature->hitbox.w / 2);
		double creature_middle_y = ptr_creature->hitbox.y + (ptr_creature->hitbox.h / 2);
		double distance_x = std::abs(creature_middle_x - my_middle_x);
		double distance_y = std::abs(creature_middle_y - my_middle_y);
		double distance = sqrt((distance_x * distance_x) + (distance_y*distance_y));
		//printf("Distance is: x: %f y: %f, overall: %f, neighbor radius is: %d\n", distance_x, distance_y, distance, default_neighbor_radius);

		if (distance <= radius)
		{
			result.push_back(ptr_creature);
		}
		//printf("Number of neighbours found: %d.\n", result.size());
	}
	return result;
}

std::vector<Creature*> Creature::FindCollisionsInSet(std::vector<Creature*>* ptr_my_creatures_set, bool check_only_obstacles)
{
	std::vector<Creature*> result = {};
	//printf("I am %p. I will find collisions with me in set %p consisting of %d items.\n", this, ptr_my_creatures_set, ptr_my_creatures_set->size());
	for (Creature* ptr_creature : *ptr_my_creatures_set)
	{
		if (DoICollideWithThisCreature(ptr_creature, check_only_obstacles))
		{
			printf("Found collision in set!\n");
			result.push_back(ptr_creature);
		}
	}
	//printf("Found %d collisions of %p in set %p.\n", result.size(), this, ptr_my_creatures_set);
	return result;
}

void Creature::DetermineNextStep(double time_passed)
{
	//printf("Time passed: %f.\n", time_passed);
	double current_angle_radian = DegreeToRadian(current_angle_degree);
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
			//#TODO - main character shift jest nieu¿ywany. Usun¹æ?
			//IncrementXMainCharacterShift(-x);
			//IncrementYMainCharacterShift(-y);
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
	//#TODO - przerobiæ na VisualComponent
	if (ptr_creature_sprite != nullptr) //Not every creature has its sprite - e.g. event trigger has only vector!
	{
		MoveSprite(x, y);
	}
	if (ptr_creature_vector != nullptr)
	{
		MoveVector(x, y);
	}
    MoveHitbox(x, y);
}

void Creature::MoveVector(double x, double y)
{
	//printf("Moved vector!\n");
	ptr_creature_vector->Move(x, y);
	//ptr_creature_vector->position.x += x;
	//ptr_creature_vector->position.y += y;
}

void Creature::MoveSprite(double x, double y)
{
	ptr_creature_sprite->Move(x, y);
    //ptr_creature_sprite->position.x += x;
    //ptr_creature_sprite->position.y += y;
}

void Creature::MoveHitbox(double x, double y)
{
    hitbox.x += x;
    hitbox.y += y;
    //printf("New hitbox coordinates: x: %d y: %d\n", hitbox.x, hitbox.y);
}

void Creature::ThrustForward()
{
	SetVelocity(default_velocity);
}

void Creature::ThrustBackward()
{

	SetVelocity(-default_velocity);
}

void Creature::SetVelocity(float my_velocity)
{
	velocity = my_velocity;
}

//#TODO - naprawiæ strafe
bool Creature::Strafe(int sidestep_angle)
{
	bool did_i_move_successfully = true;
    Coordinates next_step_cache = next_step;
    double strafing_angle = DegreeToRadian(current_angle_degree+sidestep_angle);
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

int Creature::TellCurrentAngleDegree()
{
	return current_angle_degree;
}

void Creature::SetAngleDegree(int my_degree)
{
	current_angle_degree = my_degree;
}

Coordinates Creature::CalculatePointInGivenDistanceFromCreatureCenter(unsigned int distance)
{
	PreciseRect current_coordinates = TellHitbox();
	Coordinates result = {0,0};
	double creature_center_x = current_coordinates.x + (current_coordinates.w / 2);
	double creature_center_y = current_coordinates.y + (current_coordinates.h / 2);
	int current_angle_degree = TellCurrentAngleDegree();
	double current_angle_radian = DegreeToRadian(current_angle_degree);
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
	double my_x = hitbox.x;
	double my_y = hitbox.y;
	double my_w = hitbox.w;
	double my_h = hitbox.h;

	//Checking collisions with creatures.
	//Depending on flag "check_only_obstacles":
	//    - collisions are checked only with Creature entities with "is_obstacle" flag set to true 
	//    - or with all Creature beings.

	if (ptr_my_creature != this /* Prevents checking collision with itself. */ && 
		((ptr_my_creature->is_obstacle == true && check_only_obstacles==true)) || (check_only_obstacles == false))
	{
		double obs_x = ptr_my_creature->hitbox.x;
		double obs_y = ptr_my_creature->hitbox.y;
		double obs_w = ptr_my_creature->hitbox.w;
		double obs_h = ptr_my_creature->hitbox.h;

		if (DoICollideXPlane(my_x, my_w, obs_x, obs_w) && DoICollideYPlane(my_y, my_h, obs_y, obs_h))
		{
			//printf("Collision caught by DoICollideWithThisCreature!\n");
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

// #TODO - funkcja do rozbicia na iteracjê po s¹siadach i sprawdzanie kolizji
bool Creature::DoICollideWithNeighbors(int margin)
{
    bool result = false;
    //printf("DoICollideWithNeighbors called for %p.\n", this);
	double my_x = hitbox.x;
	double my_y = hitbox.y;
	double my_w = hitbox.w;
	double my_h = hitbox.h;
	//for (Creature* ptr_creature : Creature::class_instances)
	for (Creature* ptr_creature : this->my_neighbors)
	{
		if (ptr_creature != this /* Prevents checking collision with itself. */ && ptr_creature->is_obstacle == true)
		{
			double obs_x = ptr_creature->hitbox.x;
			double obs_y = ptr_creature->hitbox.y;
			double obs_w = ptr_creature->hitbox.w;
			double obs_h = ptr_creature->hitbox.h;
		
			if (DoICollideXPlane(my_x,my_w,obs_x,obs_w, margin) && DoICollideYPlane(my_y,my_h,obs_y,obs_h, margin))
			{
			    //printf("Collision caught by DoICollideWithNeighbors!\n");
			    result = true;
			}
		}
	}
	return result;
}

bool Creature::DoICollideXPlane(double my_x, double my_w, double obs_x, double obs_w, int margin)
{
	//Margin enables us to detect collision some pixels earlier/later
	//Default margin is 0.
	if ((my_x - margin) >= obs_x && (my_x - margin) <= obs_x + obs_w)
	{
		return true;
	}
	else if ((my_x + my_w + margin) >= obs_x && (my_x + my_w + margin) <= obs_x + obs_w)
	{
		return true;
	}
	else if ((my_x - margin) <= obs_x && (my_x + my_w + margin) >= obs_x + obs_w)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Creature::DoICollideYPlane(double my_y, double my_h, double obs_y, double obs_h, int margin)
{
	//Margin enables us to detect collision some pixels earlier/later
	if ((my_y - margin) >= obs_y && (my_y - margin) <= obs_y + obs_h)
	{
		return true;
	}
	else if ((my_y + my_h + margin) >= obs_y && (my_y + my_h + margin) <= obs_y + obs_h)
	{
		return true;
	}
	else if ((my_y - margin) <= obs_y && (my_y + my_h + margin) >= obs_y + obs_h)
	{
		return true;
	}
	else
	{
		return false;
	}
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
	// #TODO - dorobiæ funkcjê do tworzenia requestów
	CreatureSpawnRequest spell_request;
	//#TODO - ucywilizowaæ to mapowanie
	if (my_spell_name == spell_vortex)
	{
		spell_request.type = cre_spell_ball;
		spell_request.initial_behavior_mode = beh_projectile;
	}
	else if (my_spell_name == spell_open_gate)
	{
		spell_request.type = cre_spell_open_doors;
	}

	int desired_distance = 100;
	spell_request.mode = center_coordinates;
	Coordinates center_coordinates = CalculatePointInGivenDistanceFromCreatureCenter(desired_distance);
	spell_request.initial_center_cooridnates = { center_coordinates.x, center_coordinates.y };

	printf("Current hero hitbox: x %f y %f w %f h %f.\n ", TellHitbox().x, TellHitbox().y, TellHitbox().w,
		TellHitbox().h);

	spell_request.initial_angle_degree = TellCurrentAngleDegree();
	spell_request.insertion_mode = merge;

	spawn_requests.push_back(spell_request);
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