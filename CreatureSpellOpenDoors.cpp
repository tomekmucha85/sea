#include <CreatureSpellOpenDoors.hpp>

CreatureSpellOpenDoors::CreatureSpellOpenDoors(Coordinates* ptr_my_center, int hitbox_margin) :
	CreatureSpell(CreatureSpellOpenDoors::my_initial_type, ptr_my_center, hitbox_margin)
{
	printf("Spawned CreatureSpellOpenDoors.\n");
	my_type = cre_spell_open_doors;
}

