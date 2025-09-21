#pragma once
#include "string"
#include "../../SDL2/include/SDL2/SDL.h"
#include "../Components.hpp"
#include "../../Headers/TextureManager.hpp"

class ColliderComponent : public Component{
public:
    SDL_Rect collider;
    std::string tag;

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

    ColliderComponent(std::string t,int xpos,int ypos,int size){
        tag = t;
        collider.x = x = xpos;
        collider.y = y = ypos;
        collider.w = collider.h = size;
    }

    ColliderComponent(std::string t,int xpos,int ypos,int w, int h){
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
        destR = {collider.x,collider.y,collider.w,collider.h};
    }

    void onEnter(){
        std::cout<<"Entering!\n";
    }

    void onExit(){
        std::cout<<"Exiting!\n";
    }

    void update() override{
        if(tag != "terrain") {
            collider.x = static_cast<int>(transform->position.x)+x;
            collider.y = static_cast<int>(transform->position.y)+y;
        }

        destR.x = collider.x - Game::camera.x;
        destR.y = collider.y - Game::camera.y;
    }

    void draw() override{
        TextureManager::Draw(tex,srcR,destR,SDL_FLIP_NONE);
    }
};