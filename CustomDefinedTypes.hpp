#ifndef CUSTOM_DEFINED_TYPES_HPP
#define CUSTOM_DEFINED_TYPES_HPP

//Functions to be passed, executing triggered event
typedef void(*TriggeredEvent)();

struct Coordinates
{
	double x = 0;
	double y = 0;
};

struct Color
{
	int red = 255;
	int green = 255;
	int blue = 255;
	int alpha = 255;
};

struct PreciseRect
{
	double x = 0;
	double y = 0;
	double w = 0;
	double h = 0;
};


//Level names
enum LevelType { level_ninemazes, level_base, level_test };

//Behavior patterns
enum BehaviorMode {beh_none, beh_chase_hero, beh_idle, beh_projectile };

//Sprite names
enum SpriteType { clawy, background, box, black_smoke, vortex };

//cre_none means empty space/no creature present
enum CreatureType { cre_none, cre_event_trigger, cre_vector_mask, cre_clawy, cre_flying_box, cre_black_smoke, cre_npc, cre_spell, cre_spell_ball, cre_blue_bground };

//Names of spells to be cast - spell is a Creature, offspring of CreatureSpell
enum SpellName {spell_vortex, spell_open_gate};


//Types of possible level components
enum LevelComponentType { levco_maze, levco_powerups, levco_test, levco_core, levco_triggers };
//List of possible behaviours while inserting a Creature:
//    force - add creature and destroy colliding ones
//    merge - add creature and do not care if it collides with any other
//    safe - check if collision occurs. If yes - do not insert

enum InsertionMode { force, merge, safe };

enum Directions { north, east, south, west };

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
};

#endif