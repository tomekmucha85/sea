#include <FactorySpawningSprites.hpp>

//#####################
//Method for spawning sprites
//#####################

Sprite* FactorySpawningSprites::SpawnSprite(SpriteType desired_type, SDL_Rect* ptr_position)
{
	Sprite* result = NULL;
	//printf("Going to create a sprite. Requested type: %d\n", desired_type);
	if (desired_type == clawy)
	{
		//printf("Requested clawy object.\n");
		result = new SpriteClawy(ptr_position);
		return result;
	}
	else if (desired_type == box)
	{
		//printf("Requested box object.\n");
		result = new SpriteBlackBox(ptr_position);
		return result;
	}
	else if (desired_type == background)
	{
		//printf("Reqested blue background object.\n");
		result = new SpriteBackground(ptr_position);
		return result;
	}
	else if (desired_type == black_smoke)
	{
		//printf("Reqested black smoke object.\n");
		result = new SpriteBlackSmoke(ptr_position);
		return result;
	}
	else
	{
		//printf("Requested some other object.\n");
		return result;
	}
}