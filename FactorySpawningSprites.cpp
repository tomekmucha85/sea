#include <FactorySpawningSprites.hpp>

//#####################
//Method for spawning sprites
//#####################

Sprite* FactorySpawningSprites::SpawnSprite(SpriteType desired_type, Coordinates* ptr_center)
{
	Sprite* result = nullptr;
	//printf("Going to create a sprite. Requested type: %d\n", desired_type);
	if (desired_type == spr_clawy)
	{
		//printf("Requested spr_clawy object.\n");
		result = new SpriteClawy(ptr_center);
		return result;
	}
	else if (desired_type == spr_snailmage)
	{
		//printf("Requested spr_snailmage object.\n");
		result = new SpriteSnailmage(ptr_center);
		return result;
	}
	else if (desired_type == spr_box)
	{
		//printf("Requested spr_box object.\n");
		result = new SpriteBlackBox(ptr_center);
		return result;
	}
	else if (desired_type == spr_background)
	{
		//printf("Reqested blue spr_background object.\n");
		result = new SpriteBackground(ptr_center);
		return result;
	}
	else if (desired_type == spr_black_smoke)
	{
		//printf("Reqested black smoke object.\n");
		result = new SpriteBlackSmoke(ptr_center);
		return result;
	}
	else if (desired_type == spr_vortex)
	{
		//printf("Reqested spr_vortex object.\n");
		result = new SpriteVortex(ptr_center);
		return result;
	}
	else if (desired_type == spr_exit)
	{
		//printf("Reqested spr_exit object.\n");
		result = new SpriteExit(ptr_center);
		return result;
	}
	else if (desired_type == spr_gui_mana_bar)
	{
		//printf("Reqested spr_gui_mana_bar object.\n");
		result = new SpriteGuiManaBar(ptr_center);
		return result;
	}
	else if (desired_type == spr_implosion)
	{
		//printf("Requested spr_implosion object.\n");
		result = new SpriteImplosion(ptr_center);
		return result;
	}
	else
	{
		//printf("Requested some other object.\n");
		return result;
	}
}