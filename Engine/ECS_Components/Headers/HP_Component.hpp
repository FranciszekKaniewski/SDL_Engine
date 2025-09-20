#pragma once
#include "../Components.hpp"

class HpComponent : public Component {
private:
    int hp;
    bool immunity;
    float immunityTimer;
public:
    HpComponent(int hp): hp(hp), immunity(false)
    {}

    void update() override {
        if (immunity) {
            immunityTimer -= Game::deltaTime;
            if (immunityTimer <= 0.0f) {
                immunity = false;
                immunityTimer = 0.0f;
            }
        }
    }

    void getImmunity(float time) {
        immunity = true;
        immunityTimer = time;
    }

    void getDamage(int damage){
        if(immunity) return;

        hp -= damage;
        getImmunity(2.0f);
        entity->getComponent<SpriteComponent>().StartBlinking(2.0f,0.2f);
        entity->getComponent<UIComponent>().showDamage("-1");
    }

    int getHp(){
        return hp;
    }
};