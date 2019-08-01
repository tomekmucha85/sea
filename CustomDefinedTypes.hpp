#ifndef CUSTOM_DEFINED_TYPES_HPP
#define CUSTOM_DEFINED_TYPES_HPP
#include <SDL.h>

//Functions to be passed, executing triggered event
//typedef void(*TriggeredEvent)();

enum DebugLevel {debug_none, debug_info, debug_full};

struct Coordinates
{
	double x = 0;
	double y = 0;
};

struct MathLineParams 
{ 
	double slope = 0; 
	double intercept = 0; 
};

struct PreciseRect
{
	double x = 0;
	double y = 0;
	double w = 0;
	double h = 0;
};


//Types of Visual Components
enum VisualComponentType {visco_sprite,
	visco_vector, 
	visco_line,
	visco_true_type_writing,
	visco_undetermined};

//Level names
enum LevelType { level_ninemazes, level_base, level_test, level_menu };

//Behavior
enum BehaviorMode {
	beh_none,
	beh_chase_hero,
	beh_idle,
	beh_projectile,
	beh_run_in_circles,
	beh_run_along_predefined_path,
	beh_wander_on_navmesh,
	beh_go_towards_fixed_point,
	beh_follow_closest_carrier,
	beh_follow_certain_creature,
};

//Outcome of actions derived from behavior

enum BehaviorActionResult {
	beh_result_action_succedeed,
	beh_result_action_failed,
	beh_result_objective_complete,
	beh_result_objective_failed
};

//Behavior patterns (rules of switching between modes)
enum BehaviorPattern {
	beh_pat_none,
	beh_pat_death_magnetic,
	beh_pat_stalker,
};


//Navigator modes of opearation
enum NavigatorMode {navig_undetermined, navig_circle, navig_coordinates_list, navig_wander_on_navmesh};

//Navigation states
enum NavigationState {active, inactive};

//Sprite names
enum SpriteType { 
	spr_clawy, 
	spr_snailmage, 
	spr_background, 
	spr_box, 
	spr_black_smoke, 
	spr_vortex, 
	spr_exit, 
	spr_gui_mana_bar,
	spr_implosion,
};

//Animation types
enum AnimationType {anim_none, 
	anim_idle, 
	anim_walk, 
	anim_vortex,
    anim_attack_melee};

//Wall blocks - related to SpriteBlackBox.hpp
enum WallType { 
	wall_bottom ,
	wall_corner_bottom_left_inward,
	wall_corner_bottom_left_outward,
	wall_corner_bottom_right_inward,
	wall_corner_bottom_right_outward,
	wall_left,
	wall_right,
	wall_top,
	wall_top_ending_left,
	wall_top_ending_right,
	wall_corner_upper_left_inward,
	wall_corner_upper_left_outward,
	wall_corner_upper_right_inward,
	wall_corner_upper_right_outward,
};

//Possible door states - related to SpriteExit.hpp
enum DoorState 
{
	door_closed, door_open, door_open_lit
};

//cre_none means empty space/no creature present
enum CreatureType { cre_none, 
	cre_event_trigger, 
	cre_vector_mask, 
	cre_clawy,
	cre_carrier_a,
	cre_flying_box, 
	cre_black_smoke,
	cre_npc,
	cre_spell,
	cre_spell_ball,
	cre_spell_open_doors,
	cre_blue_bground,
	cre_navgrid_node,
    cre_writing};

//Names of spells to be cast - spell is a Creature, offspring of CreatureSpell
enum SpellName {spell_none, spell_vortex, spell_open_gate};

//Types of possible level components
enum LevelComponentType { levco_maze, levco_powerups, levco_test, levco_core, levco_triggers, levco_navgrid };
//List of possible behaviours while inserting a Creature:
//    force - add creature and destroy colliding ones
//    merge - add creature and do not care if it collides with any other
//    safe - check if collision occurs. If yes - do not insert

enum InsertionMode { force, merge, safe };

enum Directions { north, north_east, east, south_east, south, south_west, west, north_west };

//Decide if Creature should be spawned with coordinates describing its center point or area occupied by creature.
enum PositioningMode {center_coordinates, area_coordinates, undefined};

//A request to spawn a Creature on behalf of other creature. Request is processed by LevelComponent.
struct CreatureSpawnRequest
{
	CreatureType type = cre_none;
	PositioningMode mode = undefined;
	PreciseRect initial_area = {0,0,0,0};
	Coordinates initial_center_cooridnates = {0,0};
	int initial_angle_degree = 0;
	BehaviorMode initial_behavior_mode = beh_none;
	InsertionMode insertion_mode = safe;
	//RED
	SDL_Color color = { 255,0,0,255 };
};

struct CreatureDestructionInGivenAreaRequest
{
	CreatureType kind_to_be_destroyed = cre_none; //None means that ALL kinds of creatures will be destroyed
	PreciseRect nukage_area = { 0,0,0,0 };
};

//Possible attack types for Creatures
enum AttackTypes {attack_none, attack_melee, attack_projectile};

//Possible level endings - victory, defeat or some other state - e.g. going to another level without finishing the previous one
enum LevelEnding {victory, defeat, other_ending_1, other_ending_2, other_ending_3};

//Interface mode - how events are interpreted.
enum InterfaceMode {interf_none, interf_menu, interf_guided, interf_game};

//Kind of Brain Computer Interface in use
enum BCIMode { bci_none, bci_physical, bci_virtual };

#endif