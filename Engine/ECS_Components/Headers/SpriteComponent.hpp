#pragma once

#include "../Components.hpp"
#include "../../SDL2/include/SDL2/SDL.h"
#include "../../Headers/TextureManager.hpp"
#include "Animation.hpp"
#include "map"

using namespace std;
class  SpriteComponent : public Component{
private:
    TransformComponent* transform;
    SDL_Texture *texture;
    SDL_Rect srcRect, destRect;

    bool animated = false;
    int frames = 0;
    int speed = 100;
    int size = 16;

public:

    int animIndex = 0;

    std::map<const char*,Animation> animations;

    SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

    SpriteComponent() = default;
    SpriteComponent(const char* path){
        setTex(path);
    }

    SpriteComponent(const char* path, bool isAnimated){
        animated = isAnimated;

        Animation idle = Animation(0,2,200);
        Animation walk = Animation(2,2,200);

        animations.emplace("Idle", idle);
        animations.emplace("Walk", walk);

        Play("Idle");

        setTex(path);
    }

    SpriteComponent(const char* path, int size){
        setTex(path);
        this->size = size;
    }

    ~SpriteComponent(){
//        std::cout << "Destroying sprite component: " << this << std::endl;
        SDL_DestroyTexture(texture);
    }

    void setTex(const char* path){
        texture = TextureManager::LoadTexture(path);
    }

    void init() override{
        transform = &entity->getComponent<TransformComponent>();

        srcRect.x = srcRect.y = 0;
        srcRect.w  = transform->width;
        srcRect.h = transform->height;
    }
    void update() override{
        srcRect.h = size;

        if(animated){
            srcRect.x = 9;
            srcRect.w = 9;
            srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks()/speed) % frames);
        }

        srcRect.y = animIndex * size;

        destRect.x = static_cast<int>(transform->position.x) - Game::camera.x;
        destRect.y = static_cast<int>(transform->position.y) - Game::camera.y;
        destRect.w = transform->width * transform->scale;
        destRect.h = transform->height * transform->scale;
    }
    void draw() override{
        TextureManager::Draw(texture,srcRect,destRect,spriteFlip);
    }

    void Play(const char* animName){
        frames = animations[animName].frames;
        animIndex = animations[animName].index;
        speed = animations[animName].speed;
    }
};