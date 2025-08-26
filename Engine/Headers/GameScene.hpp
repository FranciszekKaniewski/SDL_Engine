#ifndef GAME_SCENE_HPP
#define GAME_SCENE_HPP

#pragma once

#include "./UI/UIScene.hpp"

class GameScene : public UIScene {
public:
    GameScene();
    ~GameScene();

    void handleEvents(Game& game) override;
    void update(Game& game) override;
    void render(Game& game) override;

    void onEnter(Game& game) override;
    void onExit(Game& game) override;
};

#endif
