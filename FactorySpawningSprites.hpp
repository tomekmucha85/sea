#ifndef FACTORY_SPAWNING_SPRITES_HPP
#define FACTORY_SPAWNING_SPRITES_HPP
#include <SDL.h>
#include <Sprite.hpp>
#include <SpriteClawy.hpp>
#include <SpriteBackground.hpp>
#include <SpriteBlackBox.hpp>
#include <SpriteBlackSmoke.hpp>

enum SpriteType { clawy, background, box, black_smoke };

class FactorySpawningSprites
{
    private:

    public:
		Sprite* SpawnSprite(SpriteType desired_type, SDL_Rect* ptr_position = NULL);
};

#endif //FACTORY_SPAWNING_SPRITESS_HPP