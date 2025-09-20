#pragma once
#include "../Components.hpp"
#include "../../Headers/UI/UIFrame.hpp"

#include <random>

class UIComponent : public Component{
private:
    UIFrame* uiFrame;
    Window* tmpWindow;
    float windowTimer;
public:
    UIComponent(UIFrame* uiFrame): uiFrame(uiFrame),tmpWindow(nullptr), windowTimer(0.0f) {}

    void update() override{
        if(tmpWindow) {
            windowTimer -= Game::deltaTime;
            if (windowTimer <= 0.0f) {
                uiFrame->removeWindow(tmpWindow);
                windowTimer = 0.0f;
            }
        }
    }

    void showDamage(std::string text){
        int width = entity->getComponent<TransformComponent>().width;
        int height = entity->getComponent<TransformComponent>().height;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> disW(0, width-32);
        std::uniform_real_distribution<> disH(0, height-32);

        int randomValueW = disW(gen);
        int randomValueH = disH(gen);

        Vector2D objPos = entity->getComponent<TransformComponent>().position;
        SDL_Rect rect = {(int)objPos.x - Game::camera.x + 32 + randomValueW,(int)objPos.y - Game::camera.y + randomValueH,32,32};

        Font font = Font("assets/fonts/Born2bSportyFS.otf", 32);
        tmpWindow = uiFrame->addWindow(rect,"assets/UI/windowBG.png");
        tmpWindow->addTextArea(font,text,{150,90,90},{0,0,32,32});
        windowTimer = 2.0f;
    }
};