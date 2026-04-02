#include "./Headers/Scene3.hpp"

void UIScene3::onEnter(Game& game) {
    font = Font("assets/fonts/gT.otf", 32);

    uiFrame = new UIFrame(game);
    inventoryWindow = uiFrame->addWindow({-1,game.getWindowSize().height-100,55*5+10,60},"assets/UI/windowBG.png");
    escWindow = uiFrame->addWindow({-1,-1,600,900},"assets/UI/windowBG.png",true);
    escWindow->addButton(font,"assets/UI/btn.png","Exit",{-1,100,400,150},[](){std::cout << "Exit clicked!\n"; Game::isRunning = false;});

    Window* itemTextAreaWindow = uiFrame->addWindow({0,0,0,0},"assets/UI/btn.png",true);
    itemTextAreaWindow->addTextArea(font,"Click E!",{255,255,255},{0,0,96,32});

    hpWindow = uiFrame->addWindow({-1,5,400,60},"assets/UI/windowBG.png",false);
    hpWindow->addTextArea(font,"HP: 0",{0,255,0},{-1,-1,190,50});

    devWindow = uiFrame->addWindow({5,5,200,60},"assets/UI/windowBG.png",true);
    devWindow->addTextArea(font,"FPS: 0",{0,255,0},{5,5,190,50});

    map = new Map("assets/tilemap.png",1,64,manager);
    map->LoadMap("assets/map50x50.map",50,50);

    auto& player(manager.addEntity());
    player.addComponent<TransformComponent>(100,100,64,64,2);
    player.addComponent<SpriteComponent>("assets/Imgs/character-sprite.png",true);
    player.addComponent<MovementComponent>();
    player.addComponent<ColliderComponent>("player",32*2,32*2+20,20,CIRCLE);
    player.addComponent<Inventory>();
    player.addComponent<ItemCollision>(uiFrame,manager.getGroup(Game::groupItems),player.getComponent<ColliderComponent>(),player.getComponent<Inventory>());
    player.addComponent<Player_WallsCollisions>(player.getComponent<ColliderComponent>(), player.getComponent<TransformComponent>(),player.getComponent<TransformComponent>().speed , manager.getGroup(Game::groupColliders));
    player.addComponent<HoldingComponent>();
    player.addComponent<HpComponent>(20);
    player.addComponent<UIComponent>(uiFrame);
    player.addGroup(Game::groupPlayers);

    for(int i =0;i<5;i++){
        int count = player.getComponent<Inventory>().items[i].count;
        std::string text = std::to_string(count);
        inventoryWindow->addBox(font,"assets/UI/item-box.png",text,{i*55+5,-1,50,50},{0,0,0});
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

    auto& fireCamp(manager.addEntity());
    fireCamp.addComponent<TransformComponent>(300,300,64,64,1);
    fireCamp.addComponent<SpriteComponent>("assets/objects/fire-camp.png",true);
    fireCamp.addComponent<ColliderComponent>("fire-camp",32,32,20,CIRCLE);
    fireCamp.addGroup(Game::groupColliders);

    int x = Game::audio.addSong("assets/sounds/soundtrack.mp3");
    Game::audio.playMusic(x);
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
            escWindow->toggle();
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

    player->getComponent<ItemCollision>().handleInteraction();
}

void UIScene3::render(Game &game) {
    SDL_RenderClear(game.renderer);

    auto& tiles = manager.getGroup(Game::groupMap);
    for(auto& t : tiles){
        t->draw();
    }
    auto& items(manager.getGroup(Game::groupItems));
    for(auto& i : items){
        i->draw();
    }

    //Rendering base on y
    auto& players = manager.getGroup(Game::groupPlayers);
    auto& colliders = manager.getGroup(Game::groupColliders);

    std::vector<Entity*> renderables;

    renderables.insert(renderables.end(), players.begin(), players.end());
    renderables.insert(renderables.end(), colliders.begin(), colliders.end());

    std::sort(renderables.begin(), renderables.end(), [](Entity* a, Entity* b) {
        bool hasA = a->hasComponent<ColliderComponent>();
        bool hasB = b->hasComponent<ColliderComponent>();

        float bottomA = 0.0f;
        float bottomB = 0.0f;

        if (hasA) {
            auto& ca = a->getComponent<ColliderComponent>();
            if (ca.type == ColliderType::RECT) {
                bottomA = ca.collider.y + ca.collider.h;
            } else {
                bottomA = ca.circleCollider.y + ca.circleCollider.r;
            }
        } else {
            bottomA = a->getComponent<TransformComponent>().position.y;
        }

        if (hasB) {
            auto& cb = b->getComponent<ColliderComponent>();
            if (cb.type == ColliderType::RECT) {
                bottomB = cb.collider.y + cb.collider.h;
            } else {
                bottomB = cb.circleCollider.y + cb.circleCollider.r;
            }
        } else {
            bottomB = b->getComponent<TransformComponent>().position.y;
        }

        return bottomA < bottomB;
    });

    for (auto& e : renderables) {
        e->draw();
    }

    devWindow->isClosed = !Game::devMode;
    if (Game::devMode) {
        devWindow->getTextAreaById(0)->updateText("FPS: " + std::to_string(game.currentFPS));
    }

    hpWindow->getTextAreaById(0)->updateText("HP: " + std::to_string(players[0]->getComponent<HpComponent>().getHp()));

    uiFrame->render();

    SDL_RenderPresent(game.renderer);
}