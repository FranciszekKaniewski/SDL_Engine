#include "./Headers/Scene1.hpp"

void UIScene1::onEnter(Game& game) {
    font = Font("assets/fonts/Copyduck.otf", 128);

    createTextArea(game, game.title, -1, 100);
    font.changeSize(64);
    font.changeFontStyle("assets/fonts/Born2bSportyFS.otf");

    createButton(game, "> Start <", -1 , 300, [&game](){game.changeScene(game.scenes[1]);});
    createButton(game, "Options", -1 , 400, [](){std::cout << "Options clicked!\n";});
    createButton(game, "Exit", -1, 500, [](){std::cout << "Exit clicked!\n"; Game::isRunning = false;});
};

void UIScene1::onExit(Game &game) {
    UIScene::clear();
}

void UIScene1::update(Game &game) {}
void UIScene1::render(Game &game) {
    SDL_SetRenderDrawColor(Game::renderer, 200, 200, 200, 255);
    SDL_RenderClear(game.renderer);

    UIScene::render(game);

    SDL_RenderPresent(game.renderer);
}