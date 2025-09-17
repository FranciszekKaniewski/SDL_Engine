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
    std::vector<Entity*>& itemsColliders;
    ColliderComponent& playerCollider;
    Inventory& inventory;

    ItemCollision(UIFrame* uiFrame, std::vector<Entity*>& itemsColliders, ColliderComponent& playerCollider, Inventory& inventory):
        uiFrame(uiFrame), w(uiFrame->getWindowByIndex(2)), itemsColliders(itemsColliders), playerCollider(playerCollider), inventory(inventory) {}

    void onExit() {}

    void onEnter(ColliderComponent& itemCollider, const char* texturePath){
        SDL_Rect itemColliderRect = itemCollider.collider;
        w->changeRect(itemColliderRect.x - Game::camera.x - w->rect.w/2 + itemColliderRect.w/2,itemColliderRect.y - Game::camera.y - 32,96,32);
        const char* x = itemCollider.tag.c_str();
        Item i = Game::itemsManager.getItemByName(x);
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

    void update(){
        bool anyIn = false;
        for(auto& c: itemsColliders){
            auto& itemCollider = c->getComponent<ColliderComponent>();
            const char* texturePath = c->getComponent<SpriteComponent>().texturePath;

            bool in = Collisions::AABB(itemCollider.collider, playerCollider.collider);
            bool prev = prevCollStates[itemCollider.tag];

            if(in && !prev){
                onEnter(itemCollider,texturePath);
            }
            if(!in && prev){
                onExit();
            }

            prevCollStates[itemCollider.tag] = in;

            if(in) anyIn = true;
        }

        uiFrame->getWindowByIndex(2)->isClosed = !anyIn;
        if(!anyIn && w->rect.h != 0) w->changeRect(0,0,0,0);
    }
};