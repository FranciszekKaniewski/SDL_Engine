#pragma once
#include "../SDL2/include/SDL2/SDL.h"
#include "../ECS_Components/Headers/ColliderComponent.hpp"

class ColliderComponent;

class Collisions{
public:
    static bool AABB(const SDL_Rect& rectA,const SDL_Rect& rectB);
    static bool CircleCircle(const ColliderComponent::Circle& a, const ColliderComponent::Circle& b);
    static bool CircleRect(const ColliderComponent::Circle& c, const SDL_Rect& r);
    static bool CheckCollision(const ColliderComponent& colA, const ColliderComponent& colB);
};