#pragma once
#include "../SDL2/include/SDL2/SDL.h"

class ColliderComponent;

class Collisions{
public:
    static bool AABB(const SDL_Rect& rectA,const SDL_Rect& rectB);
    static bool AABB(const ColliderComponent& colA, const ColliderComponent& colB);
};