#include "./Headers/Collisions.hpp"
#include "./ECS_Components/Headers/ColliderComponent.hpp"

bool Collisions::AABB(const SDL_Rect& recA,const SDL_Rect& recB){
    if(
            recA.x + recA.w >= recB.x &&
            recB.x + recB.w >= recA.x &&
            recA.y + recA.h >= recB.y &&
            recB.y + recB.h >= recA.y
            ){
        return true;
    }

    return false;
}

bool Collisions::CircleCircle(const ColliderComponent::Circle& a, const ColliderComponent::Circle& b) {
    int dx = a.x - b.x;
    int dy = a.y - b.y;
    int rSum = a.r + b.r;
    return dx*dx + dy*dy <= rSum*rSum;
}

bool Collisions::CircleRect(const ColliderComponent::Circle& c, const SDL_Rect& r) {
    int closestX = std::max(r.x, std::min(c.x, r.x + r.w));
    int closestY = std::max(r.y, std::min(c.y, r.y + r.h));
    int dx = c.x - closestX;
    int dy = c.y - closestY;
    return dx*dx + dy*dy <= c.r * c.r;
}

bool Collisions::CheckCollision(const ColliderComponent &colA, const ColliderComponent &colB) {
    if (colA.type == ColliderType::RECT && colB.type == ColliderType::RECT) {
        return AABB(colA.collider, colB.collider);
    }
    else if (colA.type == ColliderType::CIRCLE && colB.type == ColliderType::CIRCLE) {
        return CircleCircle(colA.circleCollider, colB.circleCollider);
    }
    else if (colA.type == ColliderType::CIRCLE && colB.type == ColliderType::RECT) {
        return CircleRect(colA.circleCollider, colB.collider);
    }
    else if (colA.type == ColliderType::RECT && colB.type == ColliderType::CIRCLE) {
        return CircleRect(colB.circleCollider, colA.collider);
    }
    return false;
}