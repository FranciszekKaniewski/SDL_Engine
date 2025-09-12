#pragma once
#include "vector"
#include "./Window.hpp"

class UIFrame{
private:
    std::vector<Window*> windows;
    Game& game;

public:
    UIFrame(Game& game): game(game){};

    void update() {}

    void handleEvents(){
        SDL_PollEvent(&Game::event);

        int x, y;
        SDL_GetMouseState(&x, &y);

        for(Window* window : windows){
            window->updateMousePosClick(x,y);
        }

        if (game.event.type == SDL_MOUSEBUTTONDOWN) {
            for(Window* window : windows){
                window->handleClick();
            }
        }
    }

    void render(){
        for(Window* window : windows){
            window->draw();
        }
    }

    Window* addWindow(SDL_Rect rect,const char* texturePath, bool isClosed=false){
        if(rect.x == -1)
            rect.x = game.getWindowSize().wight/2 - rect.w/2;
        if(rect.y == -1)
            rect.y = game.getWindowSize().height/2 - rect.h/2;

        Window* window = new Window(rect,texturePath,isClosed);
        windows.push_back(window);
        return window;
    }

    Window* getWindowByIndex(int index){
        return windows[index];
    }

    void clean(){
        for(Window* window : windows){
            window->clean();
        }
        windows.clear();
    }
};