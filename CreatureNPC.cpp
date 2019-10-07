#include <CreatureNPC.hpp>

CreatureNPC::CreatureNPC(SpriteType my_sprite_type, Coordinates* ptr_my_center, int hitbox_margin) :
	Creature(my_sprite_type, ptr_my_center, hitbox_margin)
{
	//Abstract class - no instance of NPC should exist, only its children are meant to be spawned.
}
