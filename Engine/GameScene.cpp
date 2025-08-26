#include "../Headers/GameScene.hpp"
#include <iostream>

GameScene::GameScene() {}

GameScene::~GameScene() {
    UIScene::clear();
}

void GameScene::handleEvents(Game& game) {
    UIScene::handleEvents(game);
}

void GameScene::update(Game& game) {
    UIScene::update(game);
}

void GameScene::render(Game& game) {
    UIScene::render(game);
}

void GameScene::onEnter(Game& game) {
    UIScene::onEnter(game);
}

void GameScene::onExit(Game& game) {
    UIScene::onExit(game);
}
