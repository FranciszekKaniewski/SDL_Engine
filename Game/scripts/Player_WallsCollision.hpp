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
            ColliderComponent& cCol = c->getComponent<ColliderComponent>();

            if(Collisions::CheckCollision(cCol, playerCollider)) {
                if(cCol.tag == "terrain") {
                    float cx = playerCollider.circleCollider.x;
                    float cy = playerCollider.circleCollider.y;
                    float r  = playerCollider.circleCollider.r;

                    float rx = cCol.collider.x;
                    float ry = cCol.collider.y;
                    float rw = cCol.collider.w;
                    float rh = cCol.collider.h;

                    float closestX = std::max(rx, std::min(cx, rx + rw));
                    float closestY = std::max(ry, std::min(cy, ry + rh));

                    float dx = cx - closestX;
                    float dy = cy - closestY;
                    float dist2 = dx*dx + dy*dy;

                    if(dist2 < r*r) {
                        float dist = std::sqrt(dist2);
                        float nx = 0.0f, ny = 0.0f;
                        if(dist > 0.0001f){
                            nx = dx / dist;
                            ny = dy / dist;
                        } else {
                            nx = 1.0f;
                            ny = 0.0f;
                        }

                        float penetration = r - dist;

                        m += nx * penetration;
                        n += ny * penetration;
                    }
                }
                else if(cCol.tag == "fire-camp"){
                    this->entity->getComponent<HpComponent>().getDamage(1);
                }
            }
        }

        if(n != 0.0f || m != 0.0f){
            Vector2D move(m, n);
            playerTransform.position = playerTransform.position.Add(move);
            playerTransform.position.Bounce(move, 0.025f, 0.05f);
        }

        playerTransform.position = playerTransform.position.Update();
    }
};