#include "../../../Engine/ECS_Components/Components.hpp"
#include "../../../Engine/Headers/Collisions.hpp"
#include "../../../Engine/Headers/game.h"
#include "../../../Engine/Headers/UI/UIFrame.hpp"

#include "unordered_map"

class ItemCollision : public Component{
public:
    UIFrame* uiFrame;
    Window* w;
    std::unordered_map<std::string, bool> prevCollStates;
    std::vector<Entity*>& itemsEntities;
    ColliderComponent& playerCollider;
    Inventory& inventory;

    ItemCollision(UIFrame* uiFrame, std::vector<Entity*>& itemsEntities, ColliderComponent& playerCollider, Inventory& inventory):
        uiFrame(uiFrame), w(uiFrame->getWindowByIndex(2)), itemsEntities(itemsEntities), playerCollider(playerCollider), inventory(inventory) {}

    void onExit() {}

    void onEnter(SDL_Rect itemColliderRect) {
        showTextAre(itemColliderRect);
    }

    void update(){
        bool anyIn = false;
        for(auto& c: itemsEntities){
            auto& itemCollider = c->getComponent<ColliderComponent>();

            bool in = Collisions::AABB(itemCollider.collider, playerCollider.collider);
            bool prev = prevCollStates[itemCollider.tag];

            if(in && !prev){
                onEnter(itemCollider.collider);
            }
            if(!in && prev){
                onExit();
            }

            prevCollStates[itemCollider.tag] = in;

            if(in) anyIn = true;
            if(in) handleInteraction(c);
        }

        uiFrame->getWindowByIndex(2)->isClosed = !anyIn;
        if(!anyIn && w->rect.h != 0) w->changeRect(0,0,0,0);
    }

    void handleInteraction(Entity* item){
        if (Game::event.type == SDL_KEYDOWN){
            if(Game::event.key.keysym.sym == SDLK_e){
                pickUpItem(item->getComponent<ColliderComponent>(),item->getComponent<SpriteComponent>().texturePath);
                item->destroy();
            }
        }
    }

    void pickUpItem(ColliderComponent& itemCollider, const char* texturePath){
        const char* x = itemCollider.tag.c_str();
        Item* i = Game::itemsManager.getItemByName(x);
        int spot = inventory.addItem(i);

        if(spot != -1){
            BoxB* box = uiFrame->getWindowByIndex(0)->getBoxById(spot);
            box->addItem(texturePath);
            int count = inventory.items[spot].count;
            std::string text = std::to_string(count);
            box->updateText(text);
        }else{
            std::cout << "Box nullptr! spot=" << spot << std::endl;
        }
    }

    void showTextAre(SDL_Rect itemColliderRect){
        int xCam = Game::camera.x;
        int yCam = Game::camera.y;
        w->changeRect(itemColliderRect.x - xCam - w->rect.w/2 + itemColliderRect.w/2,itemColliderRect.y - yCam - 32,96,32);
    }
};