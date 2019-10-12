#ifndef CREATURE_HPP
#define CREATURE_HPP
#include <vector>
#include <string>
#include <map>
#include <functional>
#include <algorithm>
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdexcept>
#include <sstream>
#include <algorithm>
#include <Sprite.hpp>
#include <FactorySpawningSprites.hpp>
#include <VectorDrawing.hpp>
#include <TrueTypeWriting.hpp>
#include <Line.hpp>
#include <CustomDefinedTypes.hpp>
#include <Timer.hpp>
#include <TimerInterval.hpp>
#include <TimerCountdown.hpp>
#include <TimerStartStop.hpp>
#include <Navigator.hpp>
#include <FactorySpawningNavigators.hpp>
#include <CommonMath.hpp>
#include <Logger.hpp>

class Creature;

//Response containing random path
struct RandomPathResponse
{
	Creature* requestor_id = nullptr;
	std::vector<Coordinates> navigation_path = {};
};

//Request for generating random path along navigation grid
struct RandomPathRequest
{
	Creature* requestor_id = nullptr;
	unsigned int requested_hops_length = 0;
};

//Response containing path from point to point
struct PointToPointPathResponse
{
	Creature* requestor_id = nullptr;
	std::vector<Coordinates> navigation_path = {};
};

//Request for generating point-to-point path along navigation grid
struct PointToPointPathRequest
{
	Creature* ptr_requestor_id = nullptr;
	Coordinates my_position = { 0,0 };
	Coordinates destination = { 0,0 };
};

//Mesages sent between Creatures
struct ConversationalMessage
{
	Creature* ptr_sender = nullptr;
	std::string sender_friendly_name = "nameless";
	std::string message_text = "";
	std::vector<std::function<void(Creature*)>> routines_to_execute_upon_sending = {};
};

class Behavior;

class Creature
{

	friend class Behavior;
	friend class Magic;
	
    private:

        //###################
        //Variables & const
        //###################


		static const double MARGIN_FOR_LINE_OF_SIGHT_CHECKS;
		static const double DEFAULT_RADIUS_FOR_CREATURE_OF_GIVEN_TYPE_PROXIMITY_CHECKS;
		static std::vector<CreatureType> LIVING_CREATUES;
		static const double DEFAULT_RADIUS_FOR_ALERTING_CREATURES;
		static const double DEFAULT_RADIUS_FOR_CONVERSATION;
		static const Uint32 DEFAULT_CONVERSATION_MESSAGE_TIME_TO_STAY_ON_SCREEN;

		// Pointer to sprites factory
		FactorySpawningSprites* ptr_sprites_factory = nullptr;
        //Is creature an obstacle?
        bool is_obstacle = true;
		//Are any creature types excluded from collision checks?
		std::vector<CreatureType> collision_check_exceptions = {};
        //How many degrees will the creature turn in a single turn function call
        int turn_quant_degree = 20;
        //What direction does the creature face? Angle in degrees, where 0 degrees points upwards and 180 downwards
        int current_angle_degree = 0;
        //Coordinates of next step.
        Coordinates next_step;
		//Is creature unable to move?
		bool is_stuck = false;
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
		//Timer
		TimerCountdown* ptr_time_left_to_live = nullptr;
		//Is the creature living? Dead creatures should be deleted in LevelComponent plane
		bool am_i_alive = true;
		//How much mana do I have?
		int mana = 2;
		//Attack range (in pixels, counting from Creature's center)
		double melee_attack_range = 80;
		//Melee field of attack (e.g. 90 means +45 and -45 degrees counting from line of sight)
		double melee_attack_field_degrees = 180;
		//How long melee attack takes? (miliseconds)
		double melee_attack_duration = 700;
		//How hungry am I?
		int hunger_level = 0;
		//Did creature perform attack?
		bool did_i_perform_attack = false;
		//Requests to spawn other creatures. Requests are collected by LevelComponent
		std::vector<CreatureSpawnRequest> spawn_requests = {};
		//Messages requested
		std::vector<ConversationalMessage> received_conversational_messages = {};

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
		const static double DEFAULT_VELOCITY;
		const static double DEFAULT_TURBO_VELOCITY;

		double turn_speed = 0.4; // How fast creature turns around
		int turn_direction = 0; // -1 = left, 1 = right, 0 = no turning.

		std::map<std::string, VisualComponent*> visual_components = {};

        //Vector holding pointers to all creatures currently present in game
        static std::vector <Creature*> current_environment;
        //Holds address of Creature acting as current main character
        static Creature* ptr_current_main_charater;

		//NAVIGATION
		//#TODO - spokojnie mo¿na zast¹piæ zwyk³¹ tablic¹
		std::vector<RandomPathRequest> random_path_requests = {};
		std::vector<PointToPointPathRequest> point_to_point_path_requests = {};

		//Vectors grouping creatures by type. Used when random one from some category is needed.
		static std::vector <CreatureType> walls;

		//Vector storing requests to print something onscreen
		std::vector<PrintRequest> print_requests = {};

        //###################
        //Functions
        //###################
		Creature(PreciseRect* ptr_area);
		Creature(Coordinates* ptr_my_center);
		Creature(SpriteType my_sprite_type, Coordinates* ptr_my_center, int hitbox_margin = 10);
		Creature(std::string my_text, Coordinates* ptr_my_upper_left_corner, SDL_Color my_color, TTF_Font* ptr_my_font);
		~Creature();
		void SetMainVisualComponent(VisualComponent* ptr_my_visual_component);
		void ResetMainVisualComponent(VisualComponent* ptr_my_visual_component);
		VisualComponent* TellMainVisualComponent();
		void DeleteAllVisualComponents();
		void AddVisualComponent(VisualComponent* ptr_my_visual_component, std::string component_name);
		void RemoveVisualComponent(std::string component_name);
		Sprite* SpawnSpriteUsingFactory(SpriteType desired_type);
		void SetMyRenderLayer(int layer_number);
		int TellRenderLayer();
		void AddCreatureTypeToCollisionExceptions(CreatureType my_type);
		void RemoveCreatureTypeFromCollisionExceptions(CreatureType my_type);
		std::vector<CreatureType> TellCollisionExceptions();
		bool IsThisCreatureTypeAColliderForMe(CreatureType queried_type);
		PreciseRect TellHitbox();
        void Turn(int turn_angle_degree);
        void TurnRight();
        void TurnLeft();
		void TurnStop();
		double TellTurnVelocity();
		void DetermineNextStep(double time_passed);
        virtual bool Move(double x, double y);
		bool TellIfStuck();
		void AddToNeighbors(std::vector<Creature*> new_neighbors);
		std::vector<Creature*> FindNeighborsInSet(std::vector<Creature*>* ptr_my_creatures_set, int radius = NULL);
		std::vector<Creature*> FindCollisionsInSet(std::vector<Creature*>* ptr_my_creatures_set, bool check_only_obstacles = true);
		void RemoveNeighbors();
		void SetVelocity(double my_velocity);
		double TellVelocity();
        bool ShiftPositionAndRevertIfCollisionOccured(double x, double y, bool check_collisions = true);
        void MoveComponents(double x, double y);
		void MoveVisualComponent(double x, double y);
		void MoveBehaviorComponent(double x, double y);
        void MoveHitbox(double x, double y);
		virtual double TellDefaultVelocity();
        void ThrustForward(double velocity=0);
        void ThrustBackward(double velocity=0);
		void ThrustTowardsPoint(Coordinates destination, double velocity=0);
		void RunAwayFromPoint(Coordinates point, double velocity=0);
		void TurnTowardsPoint(Coordinates point);
		void TurnAwayFromPoint(Coordinates point);
		void SetPosition(Coordinates new_center_position);
		int TellCurrentAngleDegree();
		Coordinates TellCenterPoint();
		std::vector<Coordinates> TellHitboxCorners();
		Coordinates TellNextStep();
		void SetAngleDegree(int my_degree);
		bool DoesThisCreatureBelongToWalls();
        void MakeMeMainCharacter();
        bool AmIMainCharacter();
		bool AmIVisible();
		void SetVisibility(bool should_be_visible);
		void PerformCyclicActions();
		virtual void PerformCyclicActionsClassSpecific();
		void AddCyclicAction(std::function<void(Creature*)> my_cyclic_action);
		void AddCommonCyclicActions();
		//#TODO - napisaæ funkcjê do usuwania akcji cyklicznych


		//#################################################
		// Collisions and interaction with other Creatures
		//#################################################
		void MakeMeObstacle();
		void MakeMeNotObstacle();
		Coordinates CalculatePointInGivenDistanceFromCreatureCenter(unsigned int distance);
		bool DoICollideWithThisCreature(Creature* ptr_my_creature, bool check_only_obstacles = true);
		bool DoICollideWithNeighbors();
		std::vector<Creature*> WhichNeighborsDoICollideWith();
		bool IsThisCreatureWithinSightInCurrentEnvironment(Creature* ptr_other_creature, double distance_limit = 0);
		Creature* FindClosestAccessibleCreatureOfGivenType(CreatureType desired_type, double distance_limit = 0);
		std::vector<Creature*> FindAllAccessibleCreatureOfGivenType(CreatureType desired_type, double distance_limit = 0);
		bool AmIWithinProximityRadiusOfCertainTypeCreature(CreatureType queried_type, 
			double radius=DEFAULT_RADIUS_FOR_CREATURE_OF_GIVEN_TYPE_PROXIMITY_CHECKS);


        //#########################
		//# Combat
		//#########################
		virtual void Attack(AttackTypes my_type);
		bool TellIfCreaturePerformedAttack();
		void ResetAttackFlag();
		void DealDamageInRadius(int radius, 
			std::vector<CreatureType> vulnerable_types=LIVING_CREATUES);

		//################################
		//Animation for visual components
		//################################

		void PlayCurrentAnimationsForVisualComponents();
		//Used within SetVelocity
        //#TODO - rozwi¹zaæ to ³adniej, zamiast ukrywania wywo³añ animacji w ró¿nych funkcjach
		virtual void SetVelocityDependentAnimation(double previous_velocity=0, double new_velocity=0);

	    //###################
		//Static helpers
		//###################

		static Creature* WhoIsMainCharacter();
		static void SetMainCharacterToNull();
		static bool IsThisCreaturePresentInEnvironment(Creature* ptr_my_creature);
		static bool IsThereLineOfSightBetweenThesePointsInCurrentEnvironment(Coordinates point_a, Coordinates point_b, double max_line_length = 0,
			std::vector<Creature*> exceptions = {});
		static bool IsThereCorridorBetweenThesePointsInCurrentEnvironment(Coordinates point_a, Coordinates point_b, double corridor_width = MARGIN_FOR_LINE_OF_SIGHT_CHECKS, double max_corridor_length = 0);
		static std::vector<Creature*> FindCreaturesInAreaInSet(std::vector<Creature*>* ptr_my_creatures_set, PreciseRect my_area);
		static std::vector<Creature*> FindCreaturesInRadiusInSet(std::vector<Creature*>* ptr_my_creatures_set, Coordinates center_point, int radius);
		//Useful for logging.
		static std::string ConvertCreaturePointerToString(Creature* ptr_my_creature);

	    //###################
		//Requests processing
		//###################

		void PushIntoSpawnRequests(CreatureSpawnRequest my_request);
		void PlaceRandomPathRequest(unsigned int path_length);
		void PlacePointToPointPathRequest(Coordinates destination_point);
		std::vector<CreatureSpawnRequest>* TellSpawnRequests();
		void MakeUseOfPathResponse(RandomPathResponse my_response);
		void MakeUseOfPathResponse(PointToPointPathResponse my_response);

		//###################
        //Conversations
		//###################

		ConversationalMessage ConstructConversationalMessage(std::string message_text, 
			std::string my_friendly_name="nameless", 
			std::vector<std::function<void(Creature*)>> my_routines_to_execute_upon_sending = {});
		void ReceiveConversationalMessage(ConversationalMessage my_message);
		void SendConversationalMessage(Creature* ptr_addressee, ConversationalMessage my_message);
		void ReadReceivedConversationalMessages();
		virtual void ReactForReceivedConversationalMessage(ConversationalMessage my_message);
		PrintRequest ConstructRequestToPrintMessageOnscreen(std::string my_text, Uint32 my_duration_miliseconds=DEFAULT_CONVERSATION_MESSAGE_TIME_TO_STAY_ON_SCREEN);
		void PlaceRequestToPrintMessageOnscreen(PrintRequest my_request);
		void ClearPrintRequestsBuffer();

		//###################
		//Events
		//###################

		void FollowPhysics();
		void Kill();
		void Resurrect();
		bool AmIAlive();
		void CastSpell(SpellName my_spell_name);
		int TellManaLevel();
		void SetManaLevel(int new_level);
		void ChangeManaLevel(int change_amount);
		int TellHungerLevel();
		void SetHungerLevel(int new_level);
		void ChangeHungerLevel(int change_amount);
		void AlertLivingCreaturesInRadius(double radius=DEFAULT_RADIUS_FOR_ALERTING_CREATURES);

		//###################
		//Behavior
		//###################
		void FollowBehavior();
		void SetBehaviorMode(BehaviorMode behavior_to_be_set, Coordinates* ptr_my_destination = nullptr);
		void SetBehaviorPattern(BehaviorPattern pattern_to_be_set, Creature* ptr_my_destiny = nullptr);
		//#TODO - zrobiæ cztery metody, bez domyœlnego argumentu
		void RequestBehaviorMode(BehaviorMode behavior_to_be_set, Coordinates* ptr_my_destination);
		void RequestBehaviorMode(BehaviorMode behavior_to_be_set, Uint32 time_limit);
		void RequestBehaviorMode(BehaviorMode behavior_to_be_set, Creature* ptr_related_creature);
		Creature* TellFollowedCreature();


		//###################
        //Timing
        //###################

		void SetTimeToLive(unsigned int seconds);
		void ResetTimeToLive();
		void SetTimeToLiveToInfinity();
		bool HasMyTimePassedOnThisWorld();
		Uint32 HowMuchTimeLeftForMe();

		//###################
		//Virtual functions
		//###################
		virtual std::string GiveSignal();
		virtual void ArmTrigger();
		virtual void DisarmTrigger();
		virtual bool AmIArmed();

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

		std::function<void(Creature*)> func_play_current_animation_for_visual_components = [](Creature* ptr_creature)
		{
			ptr_creature->PlayCurrentAnimationsForVisualComponents();
		};

		std::function<void(Creature*)> func_kill_creature_if_its_time_expired = [](Creature* ptr_creature)
		{
			if (ptr_creature->HasMyTimePassedOnThisWorld())
			{
				printf("Killed 'cause time expired!\n");
				ptr_creature->Kill();
			}
		};

		std::function<void(Creature*)> func_read_received_messages = [](Creature* ptr_creature)
		{
			ptr_creature->ReadReceivedConversationalMessages();
		};

		std::function<void(Creature*)> func_calm_main_character_down = [](Creature* ptr_creature)
		{
			Creature::ptr_current_main_charater->SetHungerLevel(0);
			Creature::ptr_current_main_charater->RequestBehaviorMode(beh_sleep, 1500);
		};

		std::function<void(Creature*)> func_aggravate_main_character = [](Creature* ptr_creature)
		{
			Creature::ptr_current_main_charater->SetHungerLevel(3);
		};
};

class Behavior
{
	friend class Creature;

    private:
		static const double MAX_RADIUS_FOR_FINDING_CLOSEST_AVAILABLE_CREATURE;
		static const double DISTANCE_TO_KEEP_BETWEEN_HERO_AND_FOLLOWED_CREATURE;
		static const double DISTANCE_MAKING_ESCAPE_SUCCESSFULL;
		static BehaviorMode MODES_NOT_REQUIRING_ARGUMENTS_UPON_START[];
		BehaviorMode mode = beh_idle;
		BehaviorPattern pattern = beh_pat_none;
		//Object generating navigators
		FactorySpawningNavigators* ptr_navigators_factory = nullptr;
		//Object generating waypoints
		Navigator* ptr_navigator = nullptr;

		bool was_mode_changed = false;
		bool was_pattern_changed = false;

		//###################################################################
        //# Mode that was requested and will be processed by behavior pattern
        //###################################################################
		BehaviorMode current_requested_mode = beh_none;
		Coordinates current_requested_mode_destination = {0,0};
		Creature* ptr_current_requested_mode_destination_creature = nullptr;
		Uint32 current_requested_mode_time_limit = 0;

		//#TODO - czy na pewno tak powinno byæ? Mo¿e lepiej zrobiæ kilka klas dziedzicz¹cych po Behavior.
		
		//###################################
		//# VARIABLES FOR SPECIFIC PATTERNS
		//###################################
		//pattern death magnetic
		Creature* beh_pat_death_magnetic_destination = nullptr;

		//###################################
        //# VARIABLES FOR SPECIFIC MODES
        //###################################
		//mode navigate towards fixed point
		Coordinates destination_point = { 0,0 };
		//mode follow certain creature && mode follow closest creature
		Creature* ptr_followed_creature = nullptr;
		//mode escape from creature
		Creature* ptr_dreaded_creature = nullptr;
		//mode wander on navmesh
		bool was_wander_path_request_placed = false;
		//mode sleep
		TimerCountdown* ptr_timer_for_sleep = nullptr;

    public:
		//###################
		//# CONSTS
		//###################
		static const Uint32 BEH_PAT_STALKER_SLEEP_TIME;

		//###################
		//# FUNCTIONS
		//###################

		Behavior();
		~Behavior();
		void WhatToDo(Creature* ptr_my_creature);
		BehaviorActionResult PerformActionDefinedByMode(Creature* ptr_my_creature);
		bool InitializeModeFollowCertainCreature(Creature* ptr_my_creature);
		void SetFollowedCreature(Creature* ptr_my_creature);
		Creature* TellFollowedCreature();
		void SetDreadedCreature(Creature* ptr_my_creature);
		Creature* TellDreadedCreature(Creature* ptr_my_creature);
		bool FollowCertainCreature(Creature* ptr_my_creature, Creature* ptr_followed_creature);
		bool InitializeModeFollowClosestCreature(Creature* ptr_my_creature);
		bool RequestMode(BehaviorMode mode_to_be_requested);
		bool RequestMode(BehaviorMode mode_to_be_requested, Coordinates my_destination_point);
		bool RequestMode(BehaviorMode mode_to_be_requested, Creature* ptr_destination_creature);
		bool RequestMode(BehaviorMode mode_to_be_requested, Uint32 time_limit);
		bool SetMode(BehaviorMode mode_to_be_set);
		bool SetMode(BehaviorMode mode_to_be_set, Coordinates my_destination_point);
		bool SetMode(BehaviorMode mode_to_be_set, Creature* ptr_my_destiny);
		bool SetMode(BehaviorMode mode_to_be_set, Uint32 time_limit);
		int CalculateBestMovementAngleToAvoidMeetingGivenCreature(Creature* ptr_my_creature, Creature* ptr_creature_to_avoid);
		void SetPattern(BehaviorPattern pattern_to_be_set);
		void SetPattern(BehaviorPattern pattern_to_be_set, Creature* ptr_my_destiny);
		void ServeModeChangeRequestForBehaviorPatternDeathMagnetic(BehaviorMode requested_mode);
		void ServeModeChangeRequestForBehaviorPatternStalker(BehaviorMode requested_mode);
		void ServeModeChangeRequestForBehaviorPatternCarefulWanderer(BehaviorMode requested_mode);
		BehaviorMode PopCurrentRequestedMode();
		void Move(Coordinates movement);
		void MakeUseOfPathResponse(RandomPathResponse my_response);
		void MakeUseOfPathResponse(PointToPointPathResponse my_response);
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
