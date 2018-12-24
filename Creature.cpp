#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdexcept>
#include <algorithm>
#include <Creature.hpp>

//***********************************
//DEFINITIONS OF STATIC CLASS MEMBERS
//***********************************

std::vector <Creature*> Creature::current_environment;
std::vector <CreatureType> Creature::walls = { cre_flying_box };
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
Creature::Creature(SDL_Rect* ptr_area)
{
	printf("Invisible creature constructed.\n");
	//Hitbox == ptr_area. No margin is set.
	InitializeHitbox(*ptr_area, 0);
	printf("Hitbox is: x: %d y: %d w: %d h: %d.\n", hitbox.x, hitbox.y, hitbox.w, hitbox.h);

}

Creature::Creature(SpriteType my_sprite_type, SDL_Rect* ptr_my_position, int hitbox_margin, int my_render_layer)
{
	ptr_sprites_factory = new FactorySpawningSprites();
	ptr_behavior = new Behavior();
	cyclic_actions.push_back(func_follow_behavior);
	//Take care of sprite assignment
	//printf("Will assign sprite to newly spawned creature: %d\n", my_sprite_type);
	SetMySprite(ptr_sprites_factory->SpawnSprite(my_sprite_type, ptr_my_position));
	//Set the initial value to move upwards by (velocity * pixels)
	next_step.y = velocity * -1;
	//Initialize hitbox
	//#TODO//Change this, so position will be determined by creature
	SDL_Rect sprite_position = ptr_creature_sprite->TellSpritePosition();
	InitializeHitbox(sprite_position, hitbox_margin);
	//Set in which layer should this Creature be rendered
	SetMyRenderLayer(my_render_layer);
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

SDL_Rect Creature::TellHitbox()
{
	return hitbox;
}

//**************
//SETTING PARAMS
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

void Creature::SetMyVector(SDL_Rect* ptr_my_area)
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

void Creature::InitializeHitbox(SDL_Rect sprite_position, int margin_percent)
{
    //Protection against margin covering whole hitbox
    if (margin_percent >= 50)
    {
        throw std::invalid_argument("Incorrect argument passed to Creature::InitializeHitbox. Hitbox margin would consume whole hitbox.");
    }
    float x_margin = sprite_position.w*((float) margin_percent/100);
    float y_margin = sprite_position.h*((float) margin_percent/100);
    x_margin = (int) x_margin;
    y_margin = (int) y_margin;
    hitbox.x = sprite_position.x + x_margin;
    hitbox.w = sprite_position.w - x_margin;
    hitbox.y = sprite_position.y + y_margin;
    hitbox.h = sprite_position.h - y_margin;
    //printf("Hitbox is: x: %d, y: %d, w: %d, h: %d\n", hitbox.x, hitbox.y, hitbox.w, hitbox.h);
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
    current_angle_degree += turn_angle_degree;
    current_angle_degree = NormalizeAngle(current_angle_degree);
    double current_angle_radian = DegreeToRadian(current_angle_degree);
    double x_shift_dbl = sin(current_angle_radian) * velocity;
    double y_shift_dbl = cos(current_angle_radian) * velocity;
    next_step.x = (int) x_shift_dbl;
    //Multiplied by -1 because negative value means moving upward the screen
    next_step.y = (int) y_shift_dbl * -1;
//    printf ("Turn degree is %d which equals %f radians\n", current_angle_degree, current_angle_radian);
//    printf("Next step coordinates:\n");
//    printf("X shift is %d\n", next_step.x);
//    printf("Y shift is %d\n", next_step.y);
}

//Preventing angle from exceeding 360 degrees
int Creature::NormalizeAngle(int angle)
{
    if (angle > 360 || angle < -360)
    {
        angle = angle%360;
        //printf("Angle was normalized to %d degrees\n", angle);
    }
    return angle;
}

void Creature::TurnRight()
{
    Creature::Turn(turn_quant_degree);
    (*ptr_creature_sprite).angle += turn_quant_degree;
}


void Creature::TurnLeft()
{
    Creature::Turn(turn_quant_degree * -1);
    (*ptr_creature_sprite).angle -= turn_quant_degree;
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
	int my_middle_x = this->hitbox.x + (this->hitbox.w / 2);
	int my_middle_y = this->hitbox.y + (this->hitbox.h / 2);
	for (Creature* ptr_creature : *ptr_my_creatures_set)
	{
		int creature_middle_x = ptr_creature->hitbox.x + (ptr_creature->hitbox.w / 2);
		int creature_middle_y = ptr_creature->hitbox.y + (ptr_creature->hitbox.h / 2);
		int distance_x = std::abs(creature_middle_x - my_middle_x);
		int distance_y = std::abs(creature_middle_y - my_middle_y);
		int distance = sqrt((distance_x * distance_x) + (distance_y*distance_y));
		//printf("Distance is: x: %d y: %d, overall: %d, neighbor radius is: %d\n", distance_x, distance_y, distance, default_neighbor_radius);

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

bool Creature::Move(int x, int y)
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
                ptr_creature->MovePixelByPixel(-x,-y,false);
            }
        }
        //printf("Checking main character collision.\n");
        if (DoICollideWithNeighbors())
        {
            //printf("Collision of main character detected\n");
            for (Creature* ptr_creature : current_environment)
            {
                if (ptr_creature != this) /* Prevents moving the main character. */
                {
                    //#TODO - consider snapshot approach so less calculations will be needed.
                    ptr_creature->MovePixelByPixel(x,y,false); /* Reverting changes made in last step.*/
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
        did_i_move_successfully = MovePixelByPixel(x,y,true);
    }
	return did_i_move_successfully;
}

bool Creature::MovePixelByPixel(int x, int y, bool check_collisions)
//Moves creature components (sprite, hitbox etc.) pixel by pixel.
//If going to move creature by (x=2,y=2) pixels, first moves one pixel to the right (x plane)
//then moves one pixel down (y plane), then repeats sequence. Collision check is performed after
//every step.
//Returns true is no collision occurred, or false in case of a collision.
{
	bool did_i_move_successfully_x_plane = true;
	bool did_i_move_successfully_y_plane = true;
	bool did_i_move_successfully = false;
    //printf("Move pixel by pixel called for %p.\n", this);
    while(x!=0 || y!=0)
    {
        //printf("Moving pixel by pixel. Mov points left: x: %d, y: %d.\n", x,y);
        int step_quant_x = 0;
        int step_quant_y = 0;
//X plane part
        if (x>0)
        {
            step_quant_x = 1;
            x--;
        }
        else if (x<0)
        {
            step_quant_x = -1;
            x++;
        }
        MoveComponents(step_quant_x,0);
        if (DoICollideWithNeighbors() && check_collisions)
        {
            //printf("Move points zeroed. (x movement).\n");
            x = 0;
            y = 0;
            MoveComponents(-step_quant_x,0);
			did_i_move_successfully_x_plane = false;
        }
//Y plane part
        if (y>0)
        {
            step_quant_y = 1;
            y--;
        }
        else if (y<0)
        {
            step_quant_y = -1;
            y++;
        }
        MoveComponents(0,step_quant_y);
        if (DoICollideWithNeighbors() && check_collisions)
        {
            //printf("Move points zeroed. (y movement).\n");
            y = 0;
            x = 0;
            MoveComponents(0,-step_quant_y);
			did_i_move_successfully_y_plane = false;
        }
    }

	did_i_move_successfully = (did_i_move_successfully_x_plane && did_i_move_successfully_y_plane);
	return(did_i_move_successfully);
}

void Creature::MoveComponents(int x, int y)
{
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

void Creature::MoveVector(int x, int y)
{
	//printf("Moved vector!\n");
	ptr_creature_vector->position.x += x;
	ptr_creature_vector->position.y += y;
}

void Creature::MoveSprite(int x, int y)
{
    ptr_creature_sprite->position.x += x;
    ptr_creature_sprite->position.y += y;
}

void Creature::MoveHitbox(int x, int y)
{
    hitbox.x += x;
    hitbox.y += y;
    //printf("New hitbox coordinates: x: %d y: %d\n", hitbox.x, hitbox.y);
}

bool Creature::MoveForward()
{
	bool did_i_move_successfully = true;
    did_i_move_successfully = Move(next_step.x, next_step.y);
    ptr_creature_sprite->WalkAnimation();
	return did_i_move_successfully;
}

bool Creature::MoveBackward()
{
	bool did_i_move_successfully = true;
    did_i_move_successfully = Move(next_step.x * -1, next_step.y * -1);
	return did_i_move_successfully;
}

bool Creature::Strafe(int sidestep_angle)
{
	bool did_i_move_successfully = true;
    Coordinates next_step_cache = next_step;
    double strafing_angle = Creature::DegreeToRadian(current_angle_degree+sidestep_angle);
    double x_shift_dbl = sin(strafing_angle) * velocity;
    double y_shift_dbl = cos(strafing_angle) * velocity;
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

//**********
//COLLISIONS
//**********


bool Creature::DoICollideWithThisCreature(Creature* ptr_my_creature, bool check_only_obstacles)
{
	bool result = false;
	//printf("DoICollideWithNeighbors called for %p.\n", this);
	int my_x = hitbox.x;
	int my_y = hitbox.y;
	int my_w = hitbox.w;
	int my_h = hitbox.h;

	//Checking collisions with creatures.
	//Depending on flag "check_only_obstacles":
	//    - collisions are checked only with Creature entities with "is_obstacle" flag set to true 
	//    - or with all Creature beings.

	if (ptr_my_creature != this /* Prevents checking collision with itself. */ && 
		((ptr_my_creature->is_obstacle == true && check_only_obstacles==true)) || (check_only_obstacles == false))
	{
		int obs_x = ptr_my_creature->hitbox.x;
		int obs_y = ptr_my_creature->hitbox.y;
		int obs_w = ptr_my_creature->hitbox.w;
		int obs_h = ptr_my_creature->hitbox.h;

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
	int my_x = hitbox.x;
	int my_y = hitbox.y;
	int my_w = hitbox.w;
	int my_h = hitbox.h;
	//for (Creature* ptr_creature : Creature::class_instances)
	for (Creature* ptr_creature : this->my_neighbors)
	{
		if (ptr_creature != this /* Prevents checking collision with itself. */ && ptr_creature->is_obstacle == true)
		{
			int obs_x = ptr_creature->hitbox.x;
			int obs_y = ptr_creature->hitbox.y;
			int obs_w = ptr_creature->hitbox.w;
			int obs_h = ptr_creature->hitbox.h;
		
			if (DoICollideXPlane(my_x,my_w,obs_x,obs_w, margin) && DoICollideYPlane(my_y,my_h,obs_y,obs_h, margin))
			{
			    //printf("Collision caught by DoICollideWithNeighbors!\n");
			    result = true;
			}
		}
	}
	return result;
}

bool Creature::DoICollideXPlane(int my_x, int my_w, int obs_x, int obs_w, int margin)
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

bool Creature::DoICollideYPlane(int my_y, int my_h, int obs_y, int obs_h, int margin)
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