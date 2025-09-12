#pragma once

class Inventory : public Component {
private:
public:
    int selectIndex = 0;
    int items[5];

    void init() override{
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
};