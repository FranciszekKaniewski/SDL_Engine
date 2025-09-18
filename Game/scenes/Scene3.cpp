#include "./Headers/Scene3.hpp"

void UIScene3::onEnter(Game& game) {
    font = Font("assets/fonts/gT.otf", 32);

    uiFrame = new UIFrame(game);
    uiFrame->addWindow({-1,game.getWindowSize().height-100,55*5+10,60},"assets/UI/windowBG.png");
    Window* mainWindow = uiFrame->getWindowByIndex(0);


    Window* escWindow = uiFrame->addWindow({-1,-1,600,900},"assets/UI/windowBG.png",true);
    escWindow->addButton(font,"assets/UI/btn.png","Exit",{-1,100,400,150},[](){std::cout << "Exit clicked!\n"; Game::isRunning = false;});

    Window* itemTextAreaWindow = uiFrame->addWindow({0,0,0,0},"assets/UI/btn.png",true);
    itemTextAreaWindow->addTextArea(font,"Click E!",{255,255,255},{0,0,96,32});

    map = new Map("assets/til.png",2,32,manager);
    map->LoadMap("assets/map50x50.map",50,50);

    auto& player(manager.addEntity());
    player.addComponent<TransformComponent>(100,100,64,64,2);
    player.addComponent<SpriteComponent>("assets/Imgs/character-sprite.png",true);
    player.addComponent<MovementComponent>();
    player.addComponent<ColliderComponent>("player",15*2,11*2+42,34*2,22*2);
    player.addComponent<Inventory>();
    player.addComponent<ItemCollision>(uiFrame,manager.getGroup(Game::groupItems),player.getComponent<ColliderComponent>(),player.getComponent<Inventory>());
    player.addComponent<Player_WallsCollisions>(player.getComponent<ColliderComponent>(), player.getComponent<TransformComponent>(),player.getComponent<TransformComponent>().speed , manager.getGroup(Game::groupColliders));
    player.addComponent<HoldingComponent>();
    player.addGroup(Game::groupPlayers);

    for(int i =0;i<5;i++){
        int count = player.getComponent<Inventory>().items[i].count;
        std::string text = std::to_string(count);
        mainWindow->addBox(font,"assets/UI/item-box.png",text,{i*55+5,-1,50,50},{0,0,0});
    }

    auto& item(manager.addEntity());
    item.addComponent<TransformComponent>(200,200,64,64,1);
    item.addComponent<SpriteComponent>("assets/items/Carrot.png",64);
    item.addComponent<ColliderComponent>("Carrot",0,0,64);
    item.addGroup(Game::groupItems);

    auto& item2(manager.addEntity());
    item2.addComponent<TransformComponent>(300,200,64,64,1);
    item2.addComponent<SpriteComponent>("assets/items/Carrot2.png",64);
    item2.addComponent<ColliderComponent>("Carrot2",0,0,64);
    item2.addGroup(Game::groupItems);

    auto& item3(manager.addEntity());
    item3.addComponent<TransformComponent>(400,200,64,64,1);
    item3.addComponent<SpriteComponent>("assets/items/Carrot.png",64);
    item3.addComponent<ColliderComponent>("Carrot",0,0,64);
    item3.addGroup(Game::groupItems);

    auto& axe(manager.addEntity());
    axe.addComponent<TransformComponent>(200,300,64,64,1);
    axe.addComponent<SpriteComponent>("assets/items/axe.png",64);
    axe.addComponent<ColliderComponent>("Axe",0,0,64);
    axe.addGroup(Game::groupItems);
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

    // Wall COLLISONS
    player->getComponent<Player_WallsCollisions>().update();

    // Items COLLISONS
    player->getComponent<ItemCollision>().update();
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
    auto& items(manager.getGroup(Game::groupItems));
    for(auto& i : items){
        i->draw();
    }
    auto& players(manager.getGroup(Game::groupPlayers));
    for(auto& p : players){
        p->draw();
    }

    uiFrame->render();

    SDL_RenderPresent(game.renderer);
}