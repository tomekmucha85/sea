#include <Sprite.hpp>
#ifndef CREATURE_HPP
#define CREATURE_HPP

//cre_none means empty space/no creature present
enum CreatureType    {cre_none, cre_clawy, cre_flying_box, cre_black_smoke};

class Creature
{
    private:
        //###################
        //Types
        //###################

        struct Coordinates
        {
            int x = 0;
            int y = 0;
        };
        //###################
        //Variables & const
        //###################

        //Guess what
        const double PI = 3.14159265;
        //Is creature an obstacle?
        bool is_obstacle = true;
        //How many degrees will the creature turn in a single turn function call
        int turn_quant_degree = 10;
        //What direction does the creature face? Angle in degrees, where 0 degrees points upwards and 180 downwards
        int current_angle_degree = 0;
        //Coordinates of next step.
        Coordinates next_step;
		//Coordinates used when creature has to back up.
		Coordinates last_position;
        //Hitbox used for calculating collisions.
        SDL_Rect hitbox = {0,0,0,0};
		//In what layer should the creature exist (important while rendering whole scene)
		//Who is on the top, who is below?
		int render_layer = 0;

        //###################
        //Functions
        //###################

        double DegreeToRadian (int angle_degree);
        int NormalizeAngle(int angle);
        void InitializeHitbox(SDL_Rect sprite_position, int margin_percent = 0);

    public:

        //###################
        //Variables
        //###################
        int velocity = 5;
        Sprite *ptr_creature_sprite = NULL;
        //Vector holding pointers to all instances of a class
        static std::vector <Creature*> class_instances;
        //Holds address of Creature acting as current main character
        static Creature* ptr_current_main_charater;
		//Backup of object in case changes need to be reverted
		Creature* my_backup = NULL;

		//###################
		//Arrays&vectors
		//###################

		//Vectors grouping creatures by type. Used when random one from some category is needed.
		static std::vector <CreatureType> walls;

        //###################
        //Functions
        //###################
		//#TODO - czy pierwszy konstruktor (ponizej) jest w ogóle potrzebny?
        Creature(Sprite *ptr_my_sprite, int hitbox_margin = 10);
		Creature(SpriteType my_sprite_type, SDL_Rect* ptr_my_position, int hitbox_margin = 10, int my_render_layer = 0);
		~Creature();
        static Creature* SpawnCreature(CreatureType desired_type, SDL_Rect* ptr_position);
		void SetMySprite(Sprite* ptr_my_sprite);
		void SetMyRenderLayer(int layer_number);
		void MakeMeObstacle();
		void MakeMeNotObstacle();
        void Turn(int turn_angle_degree);
        void TurnRight();
        void TurnLeft();
        void Move(int x, int y);
		//void MovePixelPerfect(int x, int y);
        void MovePixelByPixel(int x, int y, bool check_collisions = true);
        void MoveComponents(int x, int y);
        void MoveSprite(int x, int y);
        void MoveHitbox(int x, int y);
        void MoveForward();
        void MoveBackward();
        void Strafe(int sidestep_angle);
        void StrafeLeft();
        void StrafeRight();
        bool DoICollide();
        bool DoICollideXPlane(int my_x, int my_w, int obs_x, int obs_w);
        bool DoICollideYPlane(int my_y, int my_h, int obs_y, int obs_h);
        static void SetMainCharacterToNull();
        void MakeMeMainCharacter();
        bool AmIMainCharacter();
        Creature* WhoIsMainCharacter();
        void AddToClassInstancesVector();
        static int TellInstancesCount();
        void PrintStupidThings(Creature* ptr_to_creature);
};
#endif // CREATURE_HPP
