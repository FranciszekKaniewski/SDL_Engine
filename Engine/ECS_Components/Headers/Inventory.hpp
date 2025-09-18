#pragma once
#include "map"
#include "../../Headers/ItemsManager.hpp"
#include "../Components.hpp"

struct ItemStack {
    Item* item;
    int count;
};

class Inventory : public Component {
private:
public:
    int selectIndex = 0;
    const int size = 5;
    ItemStack items[5];

    void init() override{
        for (int i = 0; i < size; i++) {
            items[i].item = new Item("none","none",NONE);
            items[i].count = 0;
        }
    }

    void handleEvents(){
        if (Game::event.type == SDL_KEYDOWN) {
            switch (Game::event.key.keysym.sym) {
                case SDLK_1:
                    getItem(0);
                    break;
                case SDLK_2:
                    getItem(1);
                    break;
                case SDLK_3:
                    getItem(2);
                    break;
                case SDLK_4:
                    getItem(3);
                    break;
                case SDLK_5:
                    getItem(4);
                    break;
            }
        }
    }

    int addItem(Item* item){
        for(int i=0;i<size;i++){
            if(strcmp(items[i].item->name, item->name) == 0){
                items[i].count ++;
                return i;
            }
        }
        for(int i=0;i<size;i++){
            if(!items[i].item->name || strcmp(items[i].item->name, "none") == 0) {
                items[i].count = 1;
                items[i].item = item;
                return i;
            }
        }

        return -1;
    }

    void getItem(int i){
        selectIndex = i;
        if(!items[i].item->name || strcmp(items[i].item->name, "none") == 0){
            this->entity->getComponent<HoldingComponent>().deleteItem();
        }
        else{
            this->entity->getComponent<HoldingComponent>().addItem(items[i].item);
        }
    }
};