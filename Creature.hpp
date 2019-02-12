#ifndef CREATURE_HPP
#define CREATURE_HPP
#include <vector>
#include <string>
#include <map>
#include <functional>
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdexcept>
#include <algorithm>
#include <Sprite.hpp>
#include <FactorySpawningSprites.hpp>
#include <VectorDrawing.hpp>
#include <Line.hpp>
#include <CustomDefinedTypes.hpp>
#include <Timer.hpp>
#include <Navigator.hpp>
#include <FactorySpawningNavigators.hpp>
#include <CommonMath.hpp>


class Behavior;

class Creature
{
	friend class Behavior;
	friend class Magic;
	
    private:

        //###################
        //Variables & const
        //###################
		// Pointer to sprites factory
		FactorySpawningSprites* ptr_sprites_factory = nullptr;
        //Is creature an obstacle?
        bool is_obstacle = true;
        //How many degrees will the creature turn in a single turn function call
        int turn_quant_degree = 10;
        //What direction does the creature face? Angle in degrees, where 0 degrees points upwards and 180 downwards
        int current_angle_degree = 0;
        //Coordinates of next step.
        Coordinates next_step;
        //Hitbox used for calculating collisions.
		PreciseRect hitbox = {0,0,0,0};
		//In what layer should the creature exist (important while rendering whole scene)
		//Who is on the top, who is below?
		int render_layer = 0;
		//Vector holding creatures present in given radius
		std::vector <Creature*> my_neighbors;
		//How far do we seek for neighbors?
		int default_neighbor_radius = 75;
		//Should be rendered on screen?
		bool is_visible = true;
		//Object determining creatures behavior (AI)
		Behavior* ptr_behavior = nullptr;
		//Contains actions associated to specific creature which will be performed during every game loop / every n game loops.
		std::vector<std::function<void(Creature*)>> cyclic_actions = {};
		//Is the creature living? Dead creatures should be deleted in LevelComponent plane
		bool am_i_alive = true;
		//How much mana do I have?
		int mana = 2;
		//Requests to spawn other creatures. Requests are collected by LevelComponent
		std::vector<CreatureSpawnRequest> spawn_requests = {};

        //###################
        //Functions
        //###################


        void InitializeHitbox(PreciseRect sprite_position, int margin_percent = 0);

    public:

        //###################
        //Variables
        //###################
		CreatureType my_type = cre_none;
        double velocity = 0;
		double default_velocity = 100;

		double turn_speed = 0.4; // How fast creature turns around
		int turn_direction = 0; // -1 = left, 1 = right, 0 = no turning.

		std::vector<VisualComponent*> visual_components = {};
		//VisualComponent* ptr_creature_visual_component = nullptr;

        //Sprite* ptr_creature_sprite = nullptr;
		//VectorDrawing* ptr_creature_vector = nullptr;
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
		Creature(PreciseRect* ptr_area);
		Creature(Coordinates* ptr_my_center);
		Creature(SpriteType my_sprite_type, Coordinates* ptr_my_center, int hitbox_margin = 10);
		~Creature();
		void SetMyVisualComponent(VisualComponent* ptr_my_visual_component);
		void DeleteAllVisualComponents();
		void AddVisualComponent(VisualComponent* ptr_my_visual_component);
		void SetMyRenderLayer(int layer_number);
		int TellRenderLayer();
		void MakeMeObstacle();
		void MakeMeNotObstacle();
		PreciseRect TellHitbox();
        void Turn(int turn_angle_degree);
        void TurnRight();
        void TurnLeft();
		void TurnStop();
		void DetermineNextStep(double time_passed);
        virtual bool Move(double x, double y);
		//void MovePixelPerfect(int x, int y);
		void AddToNeighbors(std::vector<Creature*> new_neighbors);
		std::vector<Creature*> FindNeighborsInSet(std::vector<Creature*>* ptr_my_creatures_set, int radius = NULL);
		std::vector<Creature*> FindCollisionsInSet(std::vector<Creature*>* ptr_my_creatures_set, bool check_only_obstacles = true);
		void RemoveNeighbors();
		void SetVelocity(double my_velocity);
        bool ShiftPositionAndRevertIfCollisionOccured(double x, double y, bool check_collisions = true);
        void MoveComponents(double x, double y);
		void MoveVisualComponent(double x, double y);
		void MoveBehaviorComponent(double x, double y);
        void MoveHitbox(double x, double y);
        void ThrustForward(double velocity=NULL);
        void ThrustBackward(double velocity=NULL);
		void TurnTowardsPoint(Coordinates point);
		void ThrustTowardsPoint(Coordinates destination);
        bool Strafe(int sidestep_angle);
        bool StrafeLeft();
        bool StrafeRight();
		void SetPosition(Coordinates new_center_position);
		int TellCurrentAngleDegree();
		Coordinates TellCenterPoint();
		Coordinates TellNextStep();
		void SetAngleDegree(int my_degree);
		Coordinates CalculatePointInGivenDistanceFromCreatureCenter(unsigned int distance);
		bool DoICollideWithThisCreature(Creature* ptr_my_creature, bool check_only_obstacles=true);
        bool DoICollideWithNeighbors(int margin = 0);
		std::vector<Creature*> WhichNeighborsDoICollideWith();
		static void RemoveAllEntriesFromEnvironmentExceptMainHero();
        static void SetMainCharacterToNull();
        void MakeMeMainCharacter();
        bool AmIMainCharacter();
		bool AmIVisible();
		void SetVisibility(bool should_be_visible);
		static Creature* WhoIsMainCharacter();
		void PerformCyclicActions();
		void AddCyclicAction(std::function<void(Creature*)> my_cyclic_action);
		std::vector<CreatureSpawnRequest>* TellSpawnRequests();
		//#TODO - napisaæ funkcjê do usuwania akcji cyklicznych
		void PushIntoSpawnRequests(CreatureSpawnRequest my_request);
		void FollowPhysics();
		void FollowBehavior();
		void SetBehaviorMode(BehaviorMode behavior_to_be_set);
		void Kill();
		void Resurrect();
		bool AmIAlive();
		void CastSpell(SpellName my_spell_name);
		int TellManaLevel();
		void SetManaLevel(int new_level);
		void ChangeManaLevel(int change_amount);
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

		std::function<void(Creature*)> func_follow_physics = [](Creature* ptr_creature)
		{
			ptr_creature->FollowPhysics();
		};

};

class Behavior
{
	friend class Creature;

    private:
		BehaviorMode mode = beh_idle;
		//Object generating navigators
		FactorySpawningNavigators* ptr_navigators_factory = nullptr;
		//Object generating waypoints
		Navigator* ptr_navigator = nullptr;

    public:
		Behavior();
		~Behavior();
		void WhatToDo(Creature* my_creature);
		void SetMode(BehaviorMode mode_to_be_set);
		void Move(Coordinates movement);

};

class Magic
{

	friend class Creature;

    private:
		//Spell characteristics: 
        static std::map<SpellName, int> spell_name_vs_mana_cost;

    public:
		static bool CastSpell(SpellName desired_name, Creature* ptr_wizard);
		static bool DoIHaveEnoughMana(SpellName desired_name, Creature* ptr_wizard);
		static void PaySpellCost(SpellName desired_name, Creature* ptr_wizard);
};


#endif // CREATURE_HPP
