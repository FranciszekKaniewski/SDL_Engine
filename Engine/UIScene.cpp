#include "Headers/UI/UIScene.hpp"

UIScene::UIScene() {}

UIScene::~UIScene() {
    clear();
}

void UIScene::clear() {}

void UIScene::onEnter(Game& game) {
    std::cout << "Entering Scene" << std::endl;
}

void UIScene::onExit(Game& game) {}

void UIScene::handleEvents(Game& game) {}

void UIScene::update(Game& game) {}

void UIScene::render(Game& game) {}