#include <LevelComponentTriggers.hpp>

LevelComponentTriggers::LevelComponentTriggers(std::map<LevelComponentType, std::vector<LevelComponent*>>* my_ptr_peer_level_components) :
	LevelComponent(my_ptr_peer_level_components)
{
	printf("Spawned component with triggers. Adddress: %p.\n", this);
}

std::vector<std::string> LevelComponentTriggers::RunTriggersHitByHero()
{
	std::vector<std::string> result = {};
	//Trigger will be deleted after use
	std::vector<Creature*> hit_triggers = FindCollisionsWithMainCharacter(false);
	//If there are any triggers colliding with main character
	// #TODO - zoptymalizowaæ
	while (hit_triggers.size() > 0)
	{
		//Take first trigger among found ones.
		Creature* ptr_trigger = *(hit_triggers.begin());
		//Make trigger send signal.
		std::string signal = ptr_trigger->GiveSignal();
		printf("A trigger %p was hit!\n", ptr_trigger);
		printf("Trigger position is: x:%f y:%f w:%f h:%f.\n", ptr_trigger->TellHitbox().x,
			ptr_trigger->TellHitbox().y,
			ptr_trigger->TellHitbox().w,
			ptr_trigger->TellHitbox().h);
		//If signal is not empty.
		if (signal != "")
		{
			result.push_back(signal);
		}
		RemoveCreature(ptr_trigger);
		hit_triggers = FindCollisionsWithMainCharacter(false);
	}
	return result;
}