#include "../../Engine/ECS_Components/Components.hpp"
#include "../../Engine/Headers/Collisions.hpp"
#include "../../Engine/Headers/game.h"
#include "../../Engine/Headers/UI/UIFrame.hpp"

class Player_WallsCollisions : public Component{
public:
    ColliderComponent& playerCollider;
    TransformComponent& playerTransform;
    std::vector<Entity*>& wallsColliders;
    int speed;

    Player_WallsCollisions(ColliderComponent& playerCollider, TransformComponent& playerTransform,int speed, std::vector<Entity*>& wallsColliders):
        playerCollider(playerCollider), playerTransform(playerTransform), wallsColliders(wallsColliders), speed(speed)
    {}

    void update(){
        float n = 0.0f;
        float m = 0.0f;
        for(auto& c: wallsColliders){

            SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
            SDL_Rect playerColliderRect = playerCollider.collider;

            if(Collisions::AABB(cCol, playerColliderRect)) {

                if(c->getComponent<ColliderComponent>().tag == "terrain") {
                    float xOverlap = 0;
                    if (playerColliderRect.x < cCol.x)
                        xOverlap = (playerColliderRect.x + playerColliderRect.w) - cCol.x;
                    else xOverlap = (cCol.x + cCol.w) - playerColliderRect.x;

                    float yOverlap = 0;
                    if (playerColliderRect.y < cCol.y)
                        yOverlap = (playerColliderRect.y + playerColliderRect.h) - cCol.y;
                    else yOverlap = (cCol.y + cCol.h) - playerColliderRect.y;

                    if (std::abs(xOverlap) < std::abs(yOverlap)) {
                        m = (playerColliderRect.x + playerColliderRect.w / 2.0f < cCol.x + cCol.w / 2.0f) ? -speed
                                                                                                          : speed;
                        n += 0;
                    } else {
                        m += 0;
                        n = (playerColliderRect.y + playerColliderRect.h / 2.0f < cCol.y + cCol.h / 2.0f) ? -speed
                                                                                                          : speed;
                    }
                }
                else if(c->getComponent<ColliderComponent>().tag == "fire-camp"){
                    this->entity->getComponent<HpComponent>().getDamage(1);
                }
            }
        }
        if(n!= 0.0f || m != 0.0f){
            Vector2D move(m,n);

            playerTransform.position = playerTransform.position.Add(move);
            playerTransform.position.Bounce(move,0.025f,0.05f);
        }
        playerTransform.position = playerTransform.position.Update();
    }
};