#include "./Headers/Scene3.hpp"

void UIScene3::onEnter(Game& game) {
    map = new Map("assets/til.png",2,32,manager);
    map->LoadMap("assets/map50x50.map",50,50);

    font = Font("assets/fonts/Copyduck.otf", 32);
    createButton(game, "Exit", -1, 900, [](){std::cout << "Exit clicked!\n"; Game::isRunning = false;});

    auto& enemy(manager.addEntity());
    enemy.addComponent<TransformComponent>(0,0,6);
    enemy.addComponent<SpriteComponent>("assets/Imgs/player2.png");
    enemy.addComponent<ColliderComponent>("enemy");
    enemy.addGroup(Game::groupEnemies);
}

void UIScene3::onExit(Game &game) {

    delete map;
    map = nullptr;
    manager.clearEntities();
    UIScene::clear();
}

void UIScene3::update(Game &game) {
    manager.refresh();
    manager.update();
}

void UIScene3::render(Game &game) {
    SDL_RenderClear(game.renderer);

    auto& tiles = manager.getGroup(Game::groupMap);
    for(auto& t : tiles){
        t->draw();
    }
    auto& enemies(manager.getGroup(Game::groupEnemies));
    for(auto& e : enemies){
        e->draw();
    }

    UIScene::render(game);

    SDL_RenderPresent(game.renderer);
}