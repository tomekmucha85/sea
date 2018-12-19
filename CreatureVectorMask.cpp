#include <CreatureVectorMask.hpp>

CreatureVectorMask::CreatureVectorMask(SDL_Rect* ptr_area) :
	Creature(ptr_area)
{
	printf("Spawned a vector mask.\n");
	my_type = cre_vector_mask;
	printf("Assigned type.\n");
	MakeMeNotObstacle();
	printf("No obstacle set.\n");
	ptr_creature_vector = new VectorDrawing(ptr_area);
	printf("Vector %p assigned to mask %p.\n", ptr_creature_vector, this);
	//By default this creature should not be rendered on screen.
	SetVisibility(false);
}
