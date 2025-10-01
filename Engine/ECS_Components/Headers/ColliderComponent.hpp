#pragma once
#include "string"
#include "../../SDL2/include/SDL2/SDL.h"
#include "../Components.hpp"
#include "../../Headers/TextureManager.hpp"

enum ColliderType {
    RECT,
    CIRCLE
};

class ColliderComponent : public Component{
public:
    SDL_Rect collider;
    struct Circle {
        int x, y;
        int r;
    } circleCollider;
    std::string tag;
    ColliderType type;

    SDL_Texture* tex;
    SDL_Rect srcR,destR;
    int x,y;

    TransformComponent* transform;

    ColliderComponent(std::string t){
        tag = t;
    }

    ~ColliderComponent(){
        SDL_DestroyTexture(tex);
        collider = {0,0};
        srcR = destR = {0,0,0,0};
    }

    ColliderComponent(std::string t,int xpos,int ypos,int size, ColliderType type = RECT): type(type){
        tag = t;
        if(type == RECT) {
            collider.x = x = xpos;
            collider.y = y = ypos;
            collider.w = collider.h = size;
        }else if(type == CIRCLE){
            circleCollider.x = x = xpos;
            circleCollider.y = y = ypos;
            circleCollider.r = size;
        }
    }

    ColliderComponent(std::string t,int xpos,int ypos,int w, int h, ColliderType type = RECT): type(type){
        tag = t;
        collider.x = x = xpos;
        collider.y = y = ypos;
        collider.w = w;
        collider.h = h;
    }

    void init() override{
        bool con = !entity->hasComponent<TransformComponent>();
        if(con){
            entity->addComponent<TransformComponent>();
        }
        transform = &entity->getComponent<TransformComponent>();

        tex = TextureManager::LoadTexture("assets/block.png");
        srcR = {0,0,32,32};

        if (type == ColliderType::RECT) {
            destR = {collider.x, collider.y, collider.w, collider.h};
        } else {
            destR = {circleCollider.x - circleCollider.r, circleCollider.y - circleCollider.r, circleCollider.r * 2, circleCollider.r * 2};
        }
    }

    void onEnter(){
        std::cout<<"Entering!\n";
    }

    void onExit(){
        std::cout<<"Exiting!\n";
    }

    void update() override{
        if(tag != "terrain") {
            if (type == ColliderType::RECT) {
                collider.x = static_cast<int>(transform->position.x) + x;
                collider.y = static_cast<int>(transform->position.y) + y;
            } else if (type == ColliderType::CIRCLE) {
                circleCollider.x = static_cast<int>(transform->position.x) + x;
                circleCollider.y = static_cast<int>(transform->position.y) + y;
            }
        }

        if (type == ColliderType::RECT) {
            destR.x = collider.x - Game::camera.x;
            destR.y = collider.y - Game::camera.y;
        } else {
            destR.x = circleCollider.x - circleCollider.r - Game::camera.x;
            destR.y = circleCollider.y - circleCollider.r - Game::camera.y;
        }
    }

    void draw() override{
        if (type == ColliderType::RECT) {
            TextureManager::Draw(tex, srcR, destR, SDL_FLIP_NONE);
        } else if(type == CIRCLE) {
            TextureManager::DrawCircle(circleCollider.x - Game::camera.x,
                                       circleCollider.y - Game::camera.y,
                                       circleCollider.r,
                                       {255,0,0,255});
        }
    }
};