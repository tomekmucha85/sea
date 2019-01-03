#include <CreatureNPC.hpp>

CreatureNPC::CreatureNPC(Coordinates* ptr_my_center, int hitbox_margin) :
	Creature(CreatureNPC::my_initial_type, ptr_my_center, hitbox_margin)
{
	//printf("Spawned CreatureNPC.\n");
	my_type = cre_npc;
}
