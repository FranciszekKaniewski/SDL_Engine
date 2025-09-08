#include "./Headers/Scene3.hpp"

void UIScene3::onEnter(Game& game) {
    map = new Map("assets/til.png",2,32,manager);
    map->LoadMap("assets/map50x50.map",50,50);

    font = Font("assets/fonts/Copyduck.otf", 32);
    createButton(game, "Exit", -1, 900, [](){std::cout << "Exit clicked!\n"; Game::isRunning = false;});

    auto& player(manager.addEntity());
    player.addComponent<TransformComponent>(0,0,64,64,2);
    player.addComponent<SpriteComponent>("assets/Imgs/Sprite-0001.png",true);
    player.addComponent<MovementComponent>();
    player.addComponent<ColliderComponent>("enemy");
    player.addGroup(Game::groupPlayers);
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

    auto& players(manager.getGroup(Game::groupPlayers));
    auto& player = players[0];
    Game::camera.x = player->getComponent<TransformComponent>().position.x - game.getWindowSize().wight/2 + 64;
    Game::camera.y = player->getComponent<TransformComponent>().position.y - game.getWindowSize().height/2 + 64;
}

void UIScene3::render(Game &game) {
    SDL_RenderClear(game.renderer);

    auto& tiles = manager.getGroup(Game::groupMap);
    for(auto& t : tiles){
        t->draw();
    }
    auto& players(manager.getGroup(Game::groupPlayers));
    for(auto& p : players){
        p->draw();
    }

    UIScene::render(game);

    SDL_RenderPresent(game.renderer);
}