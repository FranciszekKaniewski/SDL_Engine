#include "./Headers/Scene2.hpp"

void UIScene2::onEnter(Game& game) {
    font = Font("assets/fonts/Copyduck.otf", 128);

    uiFrame = new UIFrame(game);
    uiFrame->addWindow({-1,-1,640,940},"assets/UI/windowBG.png");
    Window* mainWindow = uiFrame->getWindowByIndex(0);

    mainWindow->addTextArea(font,game.title,{20,20,20},{-1,50,350,100});

    mainWindow->addTextArea(font,"< Scene 2 >",{20,20,20},{-1,200,350,100});

    font.changeFontStyle("assets/fonts/Born2bSportyFS.otf");
    mainWindow->addButton(font,"assets/UI/btn.png","> Play! <",{-1,350,350,100},[&game](){game.changeScene(game.scenes[2]);});
    mainWindow->addButton(font,"assets/UI/btn.png","> Back  <",{-1,500,350,100},[&game](){game.changeScene(game.scenes[0]);});
    mainWindow->addButton(font,"assets/UI/btn.png","> Exit  <",{-1,650,350,100},[](){std::cout << "Exit clicked!\n"; Game::isRunning = false;});
};

void UIScene2::onExit(Game &game) {
    UIScene::clear();
    uiFrame->clean();
}

void UIScene2::update(Game &game) {}

void UIScene2::handleEvents(Game &game) {
    uiFrame->handleEvents();
}

void UIScene2::render(Game &game) {
    SDL_SetRenderDrawColor(Game::renderer, 200, 200, 200, 255);
    SDL_RenderClear(game.renderer);

    UIScene::render(game);
    uiFrame->render();

    SDL_RenderPresent(game.renderer);
}