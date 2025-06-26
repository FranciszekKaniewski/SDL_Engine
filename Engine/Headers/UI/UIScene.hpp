#ifndef MENU_SCENE_HPP
#define MENU_SCENE_HPP

#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <functional>

#include "../Font.hpp"
#include "../game.h"
#include "../Scene.hpp"

struct Button {
    SDL_Rect rect;
    SDL_Texture* texture = nullptr;
    std::string label;
    std::function<void()> onClick;
    SDL_Color textColor;
    SDL_Color bgColor;
    bool hover;
};

struct TextArea {
    SDL_Rect rect;
    SDL_Texture* texture = nullptr;
    std::string label;
};

class UIScene : public Scene {
public:
    UIScene();
    ~UIScene();

    void handleEvents(Game& game) override;
    void update(Game& game) override;
    void render(Game& game) override;

    void onEnter(Game& game) override;
    void onExit(Game& game) override;

protected:
    std::vector<Button> buttons;
    std::vector<TextArea> textAreas;
    Font font;

    void createButton(Game& game, const std::string& label, int x, int y, std::function<void()> onClick,SDL_Color textColor = { 20, 20, 20, 255 }, SDL_Color bgColor = { 230, 130, 140, 255 }, int padding = 10);
    void createTextArea(Game& game, const std::string& label, int x, int y);
    void handleClick(Game& game, int x, int y);
    void clear();
};


#endif