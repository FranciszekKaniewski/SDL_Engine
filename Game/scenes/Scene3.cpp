#include "./Headers/Scene3.hpp"

void UIScene3::onEnter(Game& game) {
    font = Font("assets/fonts/Copyduck.otf", 32);

    uiFrame = new UIFrame(game);
    uiFrame->addWindow({-1,game.getWindowSize().height-100,55*5+10,60},"assets/UI/windowBG.png");
    Window* mainWindow = uiFrame->getWindowByIndex(0);

    for(int i =0;i<5;i++){
        mainWindow->addBox(font,"assets/UI/item-box.png","0",{i*55+5,-1,50,50},{0,0,0});
    }

    Window* escWindow = uiFrame->addWindow({-1,-1,600,900},"assets/UI/windowBG.png",true);
    escWindow->addButton(font,"assets/UI/btn.png","Exit",{-1,100,400,150},[](){std::cout << "Exit clicked!\n"; Game::isRunning = false;});

    map = new Map("assets/til.png",2,32,manager);
    map->LoadMap("assets/map50x50.map",50,50);

    auto& player(manager.addEntity());
    player.addComponent<TransformComponent>(0,0,64,64,2);
    player.addComponent<SpriteComponent>("assets/Imgs/character-sprite.png",true);
    player.addComponent<MovementComponent>();
    player.addComponent<ColliderComponent>("enemy");
    player.addComponent<Inventory>();
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

void UIScene3::handleEvents(Game &game) {
    uiFrame->handleEvents();

    //ESC
    if (Game::event.type == SDL_KEYDOWN){
        if(Game::event.key.keysym.sym == SDLK_ESCAPE){
            uiFrame->getWindowByIndex(1)->toggle();
        }
    }
    //

    //INVENTORY
    auto& players(manager.getGroup(Game::groupPlayers));
    auto& player = players[0];

    player->getComponent<Inventory>().handleEvents();
    for(int i =0;i<5;i++){
        uiFrame->getWindowByIndex(0)->getBoxById(i)->setSelected(false);
    }
    uiFrame->getWindowByIndex(0)->getBoxById(player->getComponent<Inventory>().selectIndex)->setSelected(true);
    //
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

    uiFrame->render();

    SDL_RenderPresent(game.renderer);
}