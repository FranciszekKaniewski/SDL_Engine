#pragma once

#include "../Components.hpp"

class MovementComponent : public Component{
public:
    TransformComponent *transform;
    SpriteComponent *sprite;

    int baseSpeed = 0;

    void init() override{
        transform = &entity->getComponent<TransformComponent>();
        sprite = &entity->getComponent<SpriteComponent>();
        baseSpeed = transform->speed;
    }

    void update() override{

        Vector2D dir(0, 0);
        transform->velocity.x = 0;
        transform->velocity.y = 0;
        sprite->Play("Idle");

        const Uint8* state = SDL_GetKeyboardState(NULL);
        if(state[SDL_SCANCODE_W]){
            sprite->spriteFlip = SDL_FLIP_NONE;
            dir.y = -1;
            sprite->Play("Walk_Up");
        }
        if(state[SDL_SCANCODE_S]){
            sprite->spriteFlip = SDL_FLIP_NONE;
            dir.y = 1;
            sprite->Play("Walk_Down");
        }
        if(state[SDL_SCANCODE_D]){
            sprite->spriteFlip = SDL_FLIP_NONE;
            dir.x = 1;
            sprite->Play("Walk_Right");
        }
        if(state[SDL_SCANCODE_A]){
            sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
            dir.x = -1;
            sprite->Play("Walk_Right");
        }

        if(state[SDL_SCANCODE_LSHIFT]){
            transform->speed = baseSpeed+2;
        }else{
            transform->speed = baseSpeed;
        }

        dir.Normalize();

        transform->velocity = dir;
    }
};