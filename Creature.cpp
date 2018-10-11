#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <math.h>
#include <Creature.hpp>
#include <Game.hpp>
#include <Sprite.hpp>
#include <CommonHeaderCreatures.hpp>
#include <stdexcept>

//***********************************
//DEFINITIONS OF STATIC CLASS MEMBERS
//***********************************

std::vector <Creature*> Creature::class_instances;
std::vector <CreatureType> Creature::walls = { cre_flying_box };
Creature* Creature::ptr_current_main_charater;

//**************
//STATIC METHODS
//**************

Creature* Creature::SpawnCreature(CreatureType desired_type, SDL_Rect* ptr_position)
{
	Creature* result = NULL;

    if (desired_type == CreatureType::cre_clawy)
    {
        printf("Requested hero creature.\n");
		result = new CreatureClawy(ptr_position);
    }
	else if (desired_type == CreatureType::cre_flying_box)
	{
		printf("Requested flying box creature. \n");
		result = new CreatureFlyingBox(ptr_position);
	}
	else if (desired_type == CreatureType::cre_black_smoke)
	{
		printf("Requested black smoke creature.\n");
		result = new CreatureBlackSmoke(ptr_position);
	}
    else
    {
        printf("Requested some other creature.\n");
    }
	
	return result;
}


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

Creature::Creature(Sprite *ptr_my_sprite, int hitbox_margin)
{
    //Write entry in static vector class_instances
    AddToClassInstancesVector();
    //Give Creature its Sprite
    SetMySprite(ptr_my_sprite);
    //Set the initial value to move upwards by (velocity * pixels)
    next_step.y = velocity * -1;
    //#TODO//Change this, so position will be determined by creature
    SDL_Rect sprite_position = ptr_creature_sprite->TellSpritePosition();
    Creature::InitializeHitbox(sprite_position,hitbox_margin);
}

Creature::Creature(SpriteType my_sprite_type, SDL_Rect* ptr_my_position, int hitbox_margin, int my_render_layer)
{
	//Take care of sprite assignment
	printf("Will assign sprite to newly spawned creature: %d\n", my_sprite_type);
	Creature::SetMySprite(Sprite::CreateSprite(my_sprite_type, ptr_my_position));
	//Set the initial value to move upwards by (velocity * pixels)
	next_step.y = velocity * -1;
	//Initialize hitbox
	//#TODO//Change this, so position will be determined by creature
	SDL_Rect sprite_position = Creature::ptr_creature_sprite->TellSpritePosition();
	Creature::InitializeHitbox(sprite_position, hitbox_margin);
	//Set in which layer should this Creature be rendered
	Creature::SetMyRenderLayer(my_render_layer);
	//Write entry in static vector class_instances
	AddToClassInstancesVector();
}

//************
//DESTRUCTOR
//************

Creature::~Creature()
{
	delete ptr_creature_sprite;
}

//****************************
//MANAGING ALL CLASS INSTANCES
//****************************

void Creature::AddToClassInstancesVector()
{
	int instances_count = Creature::TellInstancesCount();
	int my_render_layer = this->render_layer;
	//Loop to insert Creature pointer in a proper place (order is kept by render_layer attribute)
	if (instances_count > 0)
	{
		for (int i = 0; i <= instances_count; i++)
		{
			int current_item_render_layer = Creature::class_instances[i]->render_layer;
			//If render layer of added item is higher than render layer of currently examined item
			//AND we haven't reached last item in vector.
			if (my_render_layer > current_item_render_layer && i < instances_count - 1)
			{
				printf("%d: did not push into instances vector, cause render layer is higher than current.\n", i);
			}
			//If render layer of added item is higher than render layer of currently examined item
			//AND we have reached last item in vector.
			else if (my_render_layer > current_item_render_layer && i == instances_count - 1)
			{
				Creature::class_instances.push_back(this);
				printf("%d: pushed into instances vector as last item, cause reached last item in vector: %d.\n", i, instances_count  -1);
				break;
			}
			//If render layer of added item is equal or lower than render layer of currently examined item
			else
			{
				Creature::class_instances.insert(Creature::class_instances.begin()+i,this);
				printf("%d: pushed into instances vector as %d , cause render layer is higher than current.\n",i,i);
				break;
			}
		}
	}
	//If vector is empty, push the first item right away
	else
	{
        Creature::class_instances.push_back(this);
		printf("Pushed first item into class instances\n");
	}
	for (Creature* cre_in : Creature::class_instances)
	{
		printf("Creature present in class instances: %p\n", cre_in);
	}
}

int Creature::TellInstancesCount()
{
    return Creature::class_instances.size();
}

//**********************
//SETTING MAIN CHARACTER
//**********************

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
	Creature::render_layer = layer_number;
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
    printf("Hitbox is: x: %d, y: %d, w: %d, h: %d\n", hitbox.x, hitbox.y, hitbox.w, hitbox.h);
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

void Creature::Move(int x, int y)
{
//Moves this object - in case it's a non-player object.
//Moves all the other creatures in case we're dealing with player object. This way screen scroll is achieved.
    //printf("========[Move called by %p.]=============\n", this);
    if (AmIMainCharacter())
    {
        //Now we're C++11 as fuck!
        for (Creature* ptr_creature : Creature::class_instances)
        {
            if (ptr_creature != this) /* Prevents moving the main character. */
            {
                ptr_creature->MovePixelByPixel(-x,-y,false);
            }
        }
        //printf("Checking main character collision.\n");
        if (DoICollide())
        {
            printf("Collision of main character detected\n");
            for (Creature* ptr_creature : Creature::class_instances)
            {
                if (ptr_creature != this) /* Prevents moving the main character. */
                {
                    //#TODO - consider snapshot approach so less calculations will be needed.
                    ptr_creature->MovePixelByPixel(x,y,false); /* Reverting changes made in last step.*/
                }
            }
        }
        /*else
        {
            printf("Collision of main character not detected.\n");
        }*/
    }
    else
    {
        MovePixelByPixel(x,y,true);
    }
}

void Creature::MovePixelByPixel(int x, int y, bool check_collisions)
//Moves creature components (sprite, hitbox etc.) pixel by pixel.
//If going to move creature by (x=2,y=2) pixels, first moves one pixel to the right (x plane)
//then moves one pixel down (y plane), then repeats sequence. Collision check is performed after
//every step.
{
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
        if (DoICollide() && check_collisions)
        {
            //printf("Move points zeroed. (x movement).\n");
            x = 0;
            y = 0;
            MoveComponents(-step_quant_x,0);
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
        if (DoICollide() && check_collisions)
        {
            //printf("Move points zeroed. (y movement).\n");
            y = 0;
            x = 0;
            MoveComponents(0,-step_quant_y);
        }
    }
}

void Creature::MoveComponents(int x, int y)
{
    MoveSprite(x, y);
    MoveHitbox(x, y);
}

void Creature::MoveSprite(int x, int y)
{
    (*ptr_creature_sprite).position.x += x;
    (*ptr_creature_sprite).position.y += y;
}

void Creature::MoveHitbox(int x, int y)
{
    hitbox.x += x;
    hitbox.y += y;
    //printf("New hitbox coordinates: x: %d y: %d\n", hitbox.x, hitbox.y);
}

void Creature::MoveForward()
{
    Creature::Move(next_step.x, next_step.y);
    ptr_creature_sprite->WalkAnimation();
}

void Creature::MoveBackward()
{
    Creature::Move(next_step.x * -1, next_step.y * -1);
}

void Creature::Strafe(int sidestep_angle)
{
    Coordinates next_step_cache = next_step;
    double strafing_angle = Creature::DegreeToRadian(current_angle_degree+sidestep_angle);
    double x_shift_dbl = sin(strafing_angle) * velocity;
    double y_shift_dbl = cos(strafing_angle) * velocity;
    next_step.x = (int) x_shift_dbl;
    //Multiplied by -1 because negative value means moving upward the screen
    next_step.y = (int) y_shift_dbl * -1;
    Creature::Move(next_step.x, next_step.y);
    next_step = next_step_cache;
}

void Creature::StrafeLeft()
{
    Creature::Strafe(-90);
}

void Creature::StrafeRight()
{
    Creature::Strafe(90);
}

//**********
//COLLISIONS
//**********

bool Creature::DoICollide()
{
    bool result = false;
    //printf("DoICollide called for %p.\n", this);
	int my_x = hitbox.x;
	int my_y = hitbox.y;
	int my_w = hitbox.w;
	int my_h = hitbox.h;
	for (Creature* ptr_creature : Creature::class_instances)
	{
		if (ptr_creature != this /* Prevents checking collision with itself. */ && ptr_creature->is_obstacle == true)
		{
			int obs_x = ptr_creature->hitbox.x;
			int obs_y = ptr_creature->hitbox.y;
			int obs_w = ptr_creature->hitbox.w;
			int obs_h = ptr_creature->hitbox.h;
		
			if (DoICollideXPlane(my_x,my_w,obs_x,obs_w) && DoICollideYPlane(my_y,my_h,obs_y,obs_h))
			{
			    //printf("Collision caught by DoICollide!\n");
			    result = true;
			}
		}
	}
	return result;
}

bool Creature::DoICollideXPlane(int my_x, int my_w, int obs_x, int obs_w)
{
    if ((my_x >= obs_x && my_x<= obs_x + obs_w) ||
        /* FOLLOWING SCENARIO:
                [MY  OBJECT]
        [THE OTHER OBJECT]
        */
        (my_x + my_w >= obs_x && my_x + my_w <= obs_x + obs_w) ||
        /* FOLLOWING SCENARIO:
        [MY  OBJECT]
           [THE OTHER OBJECT]
        */
        (my_x >= obs_x && my_x + my_w  <= obs_x + obs_w) ||
        /* FOLLOWING SCENARIO:
           [MY  OBJECT]
        [THE OTHER OBJECT]
        */
        (my_x <= obs_x && my_x + my_w >= obs_x + obs_w))
        /* FOLLOWING SCENARIO:
           [   MY      OBJECT      ]
               [THE OTHER OBJECT]
        */
    {
        //printf("Hit in X plane.\n");
        return true;
    }
    else
    {
        return false;
    }

}

bool Creature::DoICollideYPlane(int my_y, int my_h, int obs_y, int obs_h)
{
    if ((my_y >= obs_y && my_y<= obs_y + obs_h) ||
        /* FOLLOWING SCENARIO:

                         [                ]
           [          ]  [THE OTHER OBJECT]
           [MY  OBJECT]  [                ]
           [          ]
        */
        (my_y + my_h >= obs_y && my_y + my_h <= obs_y + obs_h) ||
        /* FOLLOWING SCENARIO:

           [          ]
           [MY  OBJECT]  [                ]
           [          ]  [THE OTHER OBJECT]
                         [                ]
        */
        (my_y >= obs_y && my_y + my_h <= obs_y + obs_h) ||
        /* FOLLOWING SCENARIO:

                         [                ]
           [          ]  [                ]
           [MY  OBJECT]  [                ]
           [          ]  [THE OTHER OBJECT]
                         [                ]
                         [                ]
        */
        (my_y <= obs_y && my_y + my_h >= obs_y + obs_h))
        /* FOLLOWING SCENARIO:

           [          ]
           [          ]  [                ]
           [MY  OBJECT]  [THE OTHER OBJECT]
           [          ]  [                ]
           [          ]

        */
    {
        //printf("Hit in Y plane.\n");
        return true;
    }
    else
    {
        return false;
    }

}
