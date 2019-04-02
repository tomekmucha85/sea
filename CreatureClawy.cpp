#include <CreatureClawy.hpp>

CreatureClawy::CreatureClawy(Coordinates* ptr_my_center, int hitbox_margin) :
	Creature(CreatureClawy::my_initial_type, ptr_my_center, hitbox_margin)
{
	printf("Spawned CreatureClawy.\n");
	my_type = cre_clawy;
	//Cre_clawy does not collide with another cre_clawy
	//#TODO - czy nie statyczne?
	AddCreatureTypeToCollisionExceptions(cre_clawy);
	VisualComponent* ptr_main_visual_component = visual_components[0];
	ptr_main_visual_component->SetCurrentAnimation(anim_idle);

}

void CreatureClawy::SetVelocity(double my_velocity)
{
	double current_velocity = TellVelocity();
	VisualComponent* ptr_main_visual_component = visual_components[0];
	if (current_velocity == 0 && my_velocity != 0)
	{
		ptr_main_visual_component->SetCurrentAnimation(anim_walk);
	}
	else if (current_velocity != 0 && my_velocity == 0)
	{
		ptr_main_visual_component->SetCurrentAnimation(anim_idle);
	}
	Creature::SetVelocity(my_velocity);
}

