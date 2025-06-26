#include "./Headers/Scene2.hpp"

void UIScene2::onEnter(Game& game) {
    font = Font("assets/fonts/Copyduck.otf", 128);

    createTextArea(game, game.title, -1, 75);
    font.changeSize(64);
    createTextArea(game, "< Scene 2 >", -1, 200);
    font.changeFontStyle("assets/fonts/Born2bSportyFS.otf");

    createButton(game, "> Back <", -1 , 300, [&game](){game.changeScene(game.scenes[0]);});
    createButton(game, "> Next! <", -1 , 700, [&game](){game.changeScene(game.scenes[2]);});
    createButton(game, "Exit", -1, 500, [](){std::cout << "Exit clicked!\n"; Game::isRunning = false;});
};

void UIScene2::onExit(Game &game) {
    UIScene::clear();
}

void UIScene2::update(Game &game) {}

void UIScene2::render(Game &game) {
    SDL_SetRenderDrawColor(Game::renderer, 200, 200, 200, 255);
    SDL_RenderClear(game.renderer);

    UIScene::render(game);

    SDL_RenderPresent(game.renderer);
}