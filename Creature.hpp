#ifndef CREATURE_HPP
#define CREATURE_HPP
#include <vector>
#include <string>
#include <functional>
#include <Sprite.hpp>
#include <FactorySpawningSprites.hpp>
#include <VectorDrawing.hpp>
#include <CustomDefinedTypes.hpp>


//cre_none means empty space/no creature present
enum CreatureType    {cre_none, cre_event_trigger, cre_vector_mask, cre_clawy, cre_flying_box, cre_black_smoke};


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
		// Pointer to sprites factory
		FactorySpawningSprites* ptr_sprites_factory = nullptr;
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
        //Hitbox used for calculating collisions.
        SDL_Rect hitbox = {0,0,0,0};
		//In what layer should the creature exist (important while rendering whole scene)
		//Who is on the top, who is below?
		int render_layer = 0;
		//Vector holding creatures present in given radius
		std::vector <Creature*> my_neighbors;
		//How far do we seek for neighbors?
		int neighbor_radius = 75;
		// Distance travelled by the hero creature in x plane
		static long long int main_character_shift_x;
		// Distance travelled by the hero creature in y plane
		static long long int main_character_shift_y;

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
		// #TODO - daæ inny typ?
		CreatureType my_type = cre_none;
        int velocity = 5;
        Sprite *ptr_creature_sprite = nullptr;
		VectorDrawing* ptr_creature_vector = nullptr;
        //Vector holding pointers to all creatures currently present in game except event triggers.
        static std::vector <Creature*> current_environment;
        //Holds address of Creature acting as current main character
        static Creature* ptr_current_main_charater;

		//###################
		//Arrays&vectors
		//###################

		//Vectors grouping creatures by type. Used when random one from some category is needed.
		static std::vector <CreatureType> walls;

        //###################
        //Functions
        //###################
		Creature(SDL_Rect* ptr_area);
		Creature(SpriteType my_sprite_type, SDL_Rect* ptr_my_position, int hitbox_margin = 10, int my_render_layer = 0);
		~Creature();
		void SetMySprite(Sprite* ptr_my_sprite);
		void SetMyVector(SDL_Rect* ptr_my_area);
		void SetMyRenderLayer(int layer_number);
		void MakeMeObstacle();
		void MakeMeNotObstacle();
		SDL_Rect TellHitbox();
        void Turn(int turn_angle_degree);
        void TurnRight();
        void TurnLeft();
        void Move(int x, int y);
		//void MovePixelPerfect(int x, int y);
		void FindNeighborsInSet(std::vector<Creature*>* ptr_my_creatures_set);
		std::vector<Creature*> FindCollisionsInSet(std::vector<Creature*>* ptr_my_creatures_set, bool check_only_obstacles = true);
		void RemoveNeighbors();
        void MovePixelByPixel(int x, int y, bool check_collisions = true);
        void MoveComponents(int x, int y);
        void MoveSprite(int x, int y);
		void MoveVector(int x, int y);
        void MoveHitbox(int x, int y);
        void MoveForward();
        void MoveBackward();
        void Strafe(int sidestep_angle);
        void StrafeLeft();
        void StrafeRight();
		bool DoICollideWithThisCreature(Creature* ptr_my_creature, bool check_only_obstacles=true);
        bool DoICollideWithNeighbors();
		std::vector<Creature*> WhichNeighborsDoICollideWith();
        bool DoICollideXPlane(int my_x, int my_w, int obs_x, int obs_w);
        bool DoICollideYPlane(int my_y, int my_h, int obs_y, int obs_h);
        static void SetMainCharacterToNull();
        void MakeMeMainCharacter();
        bool AmIMainCharacter();
        Creature* WhoIsMainCharacter();
		static long long int TellXMainCharacterShift();
		static long long int TellYMainCharacterShift();
		static void SetXMainCharacterShift(long long int my_shift);
		static void SetYMainCharacterShift(long long int my_shift);
		static void IncrementXMainCharacterShift(long long int my_shift);
		static void IncrementYMainCharacterShift(long long int my_shift);
        void PrintStupidThings(Creature* ptr_to_creature);

		//###################
		//Virtual functions
		//###################
		virtual std::string GiveSignal();
		virtual void ArmTrigger();
		virtual void DisarmTrigger();
		bool AmIArmed();


};


#endif // CREATURE_HPP
