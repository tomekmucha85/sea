#ifndef CREATURE_HPP
#define CREATURE_HPP
#include <vector>
#include <string>
#include <functional>
#include <Sprite.hpp>
#include <FactorySpawningSprites.hpp>
#include <VectorDrawing.hpp>
#include <CustomDefinedTypes.hpp>


class Behavior;

class Creature
{
	friend class Behavior;
	
    private:

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
		int default_neighbor_radius = 75;
		// Distance travelled by the hero creature in x plane
		static long long int main_character_shift_x;
		// Distance travelled by the hero creature in y plane
		static long long int main_character_shift_y;
		//Should be rendered on screen?
		bool is_visible = true;
		//Object determining creatures behavior (AI)
		Behavior* ptr_behavior = nullptr;
		//Contains actions associated to specific creature which will be performed during every game loop / every n game loops.
		std::vector<std::function<void(Creature*)>> cyclic_actions = {};
		//Is the creature living? Dead creatures should be deleted in LevelComponent plane
		bool am_i_alive = true;
		//Requests to spawn other creatures. Requests are collected by LevelComponent
		std::vector<CreatureSpawnRequest> spawn_requests = {};

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
        int velocity = 3;
		//#TODO - przerobiæ na VisualComponent
        Sprite *ptr_creature_sprite = nullptr;
		VectorDrawing* ptr_creature_vector = nullptr;
        //Vector holding pointers to all creatures currently present in game
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
		Creature(SpriteType my_sprite_type, Coordinates* ptr_my_center, int hitbox_margin = 10, int my_render_layer = 0);
		~Creature();
		void SetMySprite(Sprite* ptr_my_sprite);
		void SetMyVector(SDL_Rect* ptr_my_area);
		void SetMyRenderLayer(int layer_number);
		void SetVelocity(int new_velocity);
		void MakeMeObstacle();
		void MakeMeNotObstacle();
		SDL_Rect TellHitbox();
        void Turn(int turn_angle_degree);
        void TurnRight();
        void TurnLeft();
		void DetermineNextStep();
        virtual bool Move(int x, int y);
		//void MovePixelPerfect(int x, int y);
		void AddToNeighbors(std::vector<Creature*> new_neighbors);
		std::vector<Creature*> FindNeighborsInSet(std::vector<Creature*>* ptr_my_creatures_set, int radius = NULL);
		std::vector<Creature*> FindCollisionsInSet(std::vector<Creature*>* ptr_my_creatures_set, bool check_only_obstacles = true);
		void RemoveNeighbors();
        bool MovePixelByPixel(int x, int y, bool check_collisions = true);
        void MoveComponents(int x, int y);
        void MoveSprite(int x, int y);
		void MoveVector(int x, int y);
        void MoveHitbox(int x, int y);
        bool MoveForward();
        bool MoveBackward();
        bool Strafe(int sidestep_angle);
        bool StrafeLeft();
        bool StrafeRight();
		int TellCurrentAngleDegree();
		Coordinates TellNextStep();
		void SetAngleDegree(int my_degree);
		SDL_Rect CalculatePointInGivenDistanceFromCreatureCenter(unsigned int distance);
		bool DoICollideWithThisCreature(Creature* ptr_my_creature, bool check_only_obstacles=true);
        bool DoICollideWithNeighbors(int margin = 0);
		std::vector<Creature*> WhichNeighborsDoICollideWith();
        bool DoICollideXPlane(int my_x, int my_w, int obs_x, int obs_w, int margin = 0);
        bool DoICollideYPlane(int my_y, int my_h, int obs_y, int obs_h, int margin = 0);
        static void SetMainCharacterToNull();
        void MakeMeMainCharacter();
        bool AmIMainCharacter();
		bool AmIVisible();
		void SetVisibility(bool should_be_visible);
		static Creature* WhoIsMainCharacter();
		static long long int TellXMainCharacterShift();
		static long long int TellYMainCharacterShift();
		static void SetXMainCharacterShift(long long int my_shift);
		static void SetYMainCharacterShift(long long int my_shift);
		static void IncrementXMainCharacterShift(long long int my_shift);
		static void IncrementYMainCharacterShift(long long int my_shift);
		void PerformCyclicActions();
		void AddCyclicAction(std::function<void(Creature*)> my_cyclic_action);
		std::vector<CreatureSpawnRequest>* TellSpawnRequests();
		//#TODO - napisaæ funkcjê do usuwania akcji cyklicznych
		void FollowBehavior();
		void SetBehaviorMode(BehaviorMode behavior_to_be_set);
		void Kill();
		void Resurrect();
		bool AmIAlive();
		void CastSpell(SpellName my_spell_name);
        void PrintStupidThings(Creature* ptr_to_creature);

		//###################
		//Virtual functions
		//###################
		virtual std::string GiveSignal();
		virtual void ArmTrigger();
		virtual void DisarmTrigger();
		virtual bool AmIArmed();
		/*virtual void MakeDisposable();
		virtual void MakePermanent();
		virtual bool AmIDisposable();*/

		//###################
		//COMMON LAMBDAS
		//###################

		std::function<void(Creature*)> func_follow_behavior = [](Creature* ptr_creature)
		{
			ptr_creature->FollowBehavior();
		};

};

class Behavior
{
	friend class Creature;

    private:
		BehaviorMode mode = beh_idle;

    public:
		void WhatToDo(Creature* my_creature);
		void SetMode(BehaviorMode mode_to_be_set);

};

#endif // CREATURE_HPP
