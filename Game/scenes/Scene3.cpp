#include "./Headers/Scene3.hpp"

void UIScene3::onEnter(Game& game) {
    map = new Map("assets/til.png",2,32,manager);
    map->LoadMap("assets/map50x50.map",50,50);

    font = Font("assets/fonts/Copyduck.otf", 32);
    createButton(game, "Exit", -1, 900, [](){std::cout << "Exit clicked!\n"; Game::isRunning = false;});
}

void UIScene3::onExit(Game &game) {

    delete map;
    map = nullptr;
    manager.clearEntities();
    UIScene::clear();
}

void UIScene3::update(Game &game) {}

void UIScene3::render(Game &game) {
    SDL_RenderClear(game.renderer);

    auto& tiles = manager.getGroup(Game::groupMap);
    for(auto& t : tiles){
        t->draw();
    }

    UIScene::render(game);

    SDL_RenderPresent(game.renderer);
}