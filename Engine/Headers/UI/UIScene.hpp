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
    Font font;

    void clear();
};


#endif