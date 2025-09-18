#pragma once
#include "../Components.hpp"

class HoldingComponent : public Component{
public:
    Item* item;

    HoldingComponent(): item(nullptr){}
    HoldingComponent(Item* item): item(item) {}

    void update() override{}

    void draw() override{
        if(!item) return;

        Vector2D vec = (this->entity->getComponent<TransformComponent>()).position;
        auto flip = this->entity->getComponent<SpriteComponent>().spriteFlip;
        SDL_Rect rect = {(int)vec.x-Game::camera.x+22,(int)vec.y-Game::camera.y+48,48,48};
        SDL_RenderCopyEx(Game::renderer,item->texture, nullptr,&rect,0, nullptr, flip);
    }

    void addItem(Item* item){
        this->item = item;
    }

    void deleteItem(){
        item = nullptr;
    }
};