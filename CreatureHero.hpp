#include <Creature.hpp>
#include <Game.hpp>
#ifndef CREATURE_HERO_HPP
#define CREATURE_HERO_HPP
class CreatureHero : public Creature
{
    private:
		Sprite * ptr_my_sprite = NULL;

    public:
        CreatureHero(Sprite *ptr_my_sprite, int hitbox_margin = 10);
};
#endif //CREATURE_HERO_HPP
//3.In constructor an instance of sprite is created and pointer passed to object.
//4.Sprite object contains all the animation data? So sprite needs inheriting classes!
