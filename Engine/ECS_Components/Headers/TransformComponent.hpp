#pragma once
#include "../Components.hpp"
#include "../../Headers/Vector.hpp"

class TransformComponent : public Component{

public:
    Vector2D position;
    Vector2D velocity;

    int height = 16;
    int width = 9;
    int scale = 6;

    int speed = 3;

    TransformComponent(){
        position.Zero();
    }
    ~TransformComponent(){
        position.Zero();
        velocity.Zero();
        height = width = scale = speed = 0;
    }
    TransformComponent(int s){
        position.x = 400;
        position.y = 320;
        scale = s;
    }
    TransformComponent(float x, float y,int s){
        position.x = x;
        position.y = y;
        scale = s;
    }
    TransformComponent(float x, float y,int h,int w, int s){
        position.x = x;
        position.y = y;
        height = h;
        width = w;
        scale = s;
    }

    void init() override{
        velocity.Zero();
    }
    void update() override{
        position.x += velocity.x * speed;
        position.y += velocity.y * speed;
    }
};