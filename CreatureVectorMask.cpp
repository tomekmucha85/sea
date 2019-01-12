#include <CreatureVectorMask.hpp>

CreatureVectorMask::CreatureVectorMask(PreciseRect* ptr_area) :
	Creature(ptr_area)
{
	printf("Spawned a vector mask.\n");
	my_type = cre_vector_mask;
	printf("Assigned type.\n");
	MakeMeNotObstacle();
	printf("No obstacle set.\n");
	//By default this creature should not be rendered on screen.
	SetVisibility(false);
}
