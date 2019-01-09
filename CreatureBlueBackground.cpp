#include <CreatureBlueBackground.hpp>

CreatureBlueBackground::CreatureBlueBackground(Coordinates* ptr_my_center, int hitbox_margin, int initial_render_layer) :
	Creature(CreatureBlueBackground::my_initial_type, ptr_my_center, hitbox_margin, initial_render_layer)
{
	printf("Spawned CreatureBlueBackground.\n");
	my_type = cre_blue_bground;
}