#include <CreatureNPC.hpp>

CreatureNPC::CreatureNPC(SDL_Rect* ptr_my_position, int hitbox_margin) :
	Creature(CreatureNPC::my_initial_type, ptr_my_position, hitbox_margin)
{
	//printf("Spawned CreatureNPC.\n");
	my_type = cre_npc;
}
