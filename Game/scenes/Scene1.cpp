#include "./Headers/Scene1.hpp"

void UIScene1::onEnter(Game& game) {
    font = Font("assets/fonts/gT.otf", 128);

    uiFrame = new UIFrame(game);
    uiFrame->addWindow({-1,-1,640,940},"assets/UI/windowBG.png");
    Window* mainWindow = uiFrame->getWindowByIndex(0);

    mainWindow->addTextArea(font,"Super Gra!",{20,20,20},{-1,50,350,100});
    mainWindow->addButton(font,"assets/UI/btn.png","Play!",{-1,200,400,125},[&game](){game.changeScene(game.scenes[2]);});
    mainWindow->addButton(font,"assets/UI/btn.png","Next!",{-1,350,400,125},[&game](){game.changeScene(game.scenes[1]);});
};

void UIScene1::onExit(Game &game) {
    UIScene::clear();
    uiFrame->clean();
}

void UIScene1::update(Game &game) {
    uiFrame->update();
}

void UIScene1::handleEvents(Game &game) {
    uiFrame->handleEvents();
}

void UIScene1::render(Game &game) {
    SDL_SetRenderDrawColor(Game::renderer, 200, 200, 200, 255);
    SDL_RenderClear(game.renderer);

    uiFrame->render();

    SDL_RenderPresent(game.renderer);
}