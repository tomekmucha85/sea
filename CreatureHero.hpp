#include <Creature.hpp>
#ifndef CREATURE_HERO_HPP
#define CREATURE_HERO_HPP
class CreatureHero : public Creature
{
    private:
        Sprite* ptr_my_sprite = NULL;

    public:
        CreatureHero(Sprite *ptr_my_sprite, int hitbox_margin = 10);
};
#endif //CREATURE_HERO_HPP
//1.Pointer to sprite initialised at NULL
//3.In constructor an instance of sprite is created and pointer passed to object.
//4.Sprite object contains all the animation data? So sprite needs inheriting classes!
