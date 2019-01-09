#ifndef FACTORY_SPAWNING_SPRITES_HPP
#define FACTORY_SPAWNING_SPRITES_HPP
#include <SDL.h>
#include <Sprite.hpp>
#include <SpriteClawy.hpp>
#include <SpriteBackground.hpp>
#include <SpriteBlackBox.hpp>
#include <SpriteBlackSmoke.hpp>
#include <SpriteVortex.hpp>

class FactorySpawningSprites
{
    private:

    public:
		Sprite* SpawnSprite(SpriteType desired_type, Coordinates* ptr_center = nullptr);
};

#endif //FACTORY_SPAWNING_SPRITESS_HPP