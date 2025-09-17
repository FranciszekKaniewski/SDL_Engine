#pragma once
#include "map"
#include "../../Headers/ItemsManager.hpp"

struct ItemStack {
    Item item;
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
            items[i].item.name = nullptr;
            items[i].item.imgPath = nullptr;
            items[i].item.type = ITEM;
            items[i].count = 0;
        }
    }

    void handleEvents(){
        if (Game::event.type == SDL_KEYDOWN) {
            switch (Game::event.key.keysym.sym) {
                case SDLK_1:
                    selectIndex = 0;
                    break;
                case SDLK_2:
                    selectIndex = 1;
                    break;
                case SDLK_3:
                    selectIndex = 2;
                    break;
                case SDLK_4:
                    selectIndex = 3;
                    break;
                case SDLK_5:
                    selectIndex = 4;
                    break;
            }
        }
    }

    int addItem(Item item){
        for(int i=0;i<size;i++){
            if(items[i].item.name == item.name){
                items[i].count ++;
                return i;
            }
        }
        for(int i=0;i<size;i++){
            if(!items[i].item.name || strcmp(items[i].item.name, "null") == 0) {
                items[i].count = 1;
                items[i].item = item;
                return i;
            }
        }

        return -1;
    }
};