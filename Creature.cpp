#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <math.h>
#include <Creature.hpp>
#include <Sprite.hpp>
#include <Game.hpp>
#include <stdexcept>

//***********************************
//DEFINITIONS OF STATIC CLASS MEMBERS
//***********************************

std::vector <Creature*> Creature::class_instances;
std::vector <SDL_Rect> Creature::obstacles;
Creature* Creature::ptr_current_main_charater;

//**************
//STATIC METHODS
//**************

Creature* Creature::SpawnCreature(CreatureType desired_type, SDL_Rect* ptr_position)
{
    if (desired_type == CreatureType::hero)
    {
        printf("Requested hero creature.\n");
    }
    else
    {
        printf("Requested some other creature.\n");
    }
	//Dummy return #TODO
	return Creature::ptr_current_main_charater;
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
    ptr_creature_sprite = ptr_my_sprite;
    //Set the initial value to move upwards by (velocity * pixels)
    next_step.y = velocity * -1;
    //#TODO//Change this, so position will be determined by creature
    SDL_Rect sprite_position = ptr_creature_sprite->TellSpritePosition();
    Creature::InitializeHitbox(sprite_position,hitbox_margin);
    if (is_obstacle == true)
    {
        printf("Will initialise obstacle\n");
        AddToObstacles(hitbox);
    }
}

//****************************
//MANAGING ALL CLASS INSTANCES
//****************************

void Creature::AddToClassInstancesVector()
{
    Creature::class_instances.push_back(this);
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
//**********
//COLLISIONS
//**********

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

int Creature::TellObstaclesCount()
{
    int obstacles_count = Creature::obstacles.size();
    return obstacles_count;
}

void Creature::AddToObstacles(SDL_Rect my_hitbox)
{
    int current_count = Creature::TellObstaclesCount();
    Creature::obstacles.push_back(hitbox);
    //The trick is that index of the pushed element == number of elements before the push
    //It's crude but effective
    //Anyway, NEVER remove anything from obstacles vector.
    obstacle_index = current_count;
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
            //printf("Collision of main character detected\n");
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
    MoveObstacle(x, y);
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

void Creature::MoveObstacle(int x, int y)
{
    if (obstacle_index != NULL)
    {
        obstacles[obstacle_index].x += x;
        obstacles[obstacle_index].y += y;
        //printf("New obstacle coordinates: x: %d y: %d\n",
        //       obstacles[obstacle_index].x,
        //       obstacles[obstacle_index].y);
    }
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
    int current_count = TellObstaclesCount();
    for (int i = 0; i < current_count ; i++)
    {
        int my_x  = hitbox.x;
        int my_y  = hitbox.y;
        int my_w  = hitbox.w;
        int my_h  = hitbox.h;
        int obs_x = obstacles[i].x;
        int obs_y = obstacles[i].y;
        int obs_w = obstacles[i].w;
        int obs_h = obstacles[i].h;

        //printf("DoICollide obstacle index: %d out of %d.\n", i, current_count);

        if (i == obstacle_index)
        {
        //We don't want to check collision with ourselves!
            //printf("Obstacle index (%d) reached. Skipping check.\n", i);
            continue;
        }
        /*printf("Checking collision of (me, %p) x: %d y: %d w: %d h: %d against (some other object): x: %d, y: %d, w: %d, h: %d\n",
               this, my_x,my_y,my_w,my_h,obs_x,obs_y,obs_w,obs_h);*/

        if (DoICollideXPlane(my_x,my_w,obs_x,obs_w) && DoICollideYPlane(my_y,my_h,obs_y,obs_h))
        {
            //printf("Collision caught by DoICollide!\n");
            result = true;
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
