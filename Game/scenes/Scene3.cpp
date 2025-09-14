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
    player.addComponent<TransformComponent>(100,100,64,64,2);
    player.addComponent<SpriteComponent>("assets/Imgs/character-sprite.png",true);
    player.addComponent<MovementComponent>();
    player.addComponent<ColliderComponent>("player",15*2,11*2+42,34*2,22*2);
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

    //COLLISONS
    SDL_Rect playerCol = player->getComponent<ColliderComponent>().collider;
    Vector2D playerPos = player->getComponent<TransformComponent>().position;

    auto& colliders = manager.getGroup(Game::groupColliders);

    float n = 0.0f;
    float m = 0.0f;
    for(auto& c: colliders){

        SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;

        if(Collisions::AABB(cCol, playerCol)) {

            float xOverlap = 0;
            if(playerCol.x < cCol.x) xOverlap = (playerCol.x + playerCol.w) - cCol.x;
            else xOverlap = (cCol.x + cCol.w) - playerCol.x;

            float yOverlap = 0;
            if(playerCol.y < cCol.y) yOverlap = (playerCol.y + playerCol.h) - cCol.y;
            else yOverlap = (cCol.y + cCol.h) - playerCol.y;

            float speed = player->getComponent<TransformComponent>().speed;

            if(std::abs(xOverlap) < std::abs(yOverlap)) {
                m = (playerCol.x + playerCol.w/2.0f < cCol.x + cCol.w/2.0f) ? -speed : speed;
                n += 0;
            } else {
                m += 0;
                n = (playerCol.y + playerCol.h/2.0f < cCol.y + cCol.h/2.0f) ? -speed : speed;
            }
        }
    }
    if(n!= 0.0f || m != 0.0f){
        Vector2D move(m,n);

        player->getComponent<TransformComponent>().position = playerPos.Add(move);
        playerPos.Bounce(move,0.025f,0.05f);
    }
    player->getComponent<TransformComponent>().position = playerPos.Update();
    //
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
    auto& colliders = manager.getGroup(Game::groupColliders);
    for(auto& c : colliders){
        c->draw();
    }
    auto& players(manager.getGroup(Game::groupPlayers));
    for(auto& p : players){
        p->draw();
    }

    uiFrame->render();

    SDL_RenderPresent(game.renderer);
}