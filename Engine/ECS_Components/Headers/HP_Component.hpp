#pragma once
#include "../Components.hpp"

class HpComponent : public Component {
private:
    int hp;
    bool immunity;
    float immunityTimer;
    float immunityTimeAfterHit;
    std::function<void(Entity&)> onDeathEffect;

public:
    HpComponent(int hp, float time=0): hp(hp), immunity(false), immunityTimeAfterHit(time)
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

    void getImmunity() {
        immunity = true;
        immunityTimer = immunityTimeAfterHit;
    }

    void getDamage(int damage){
        if(immunity) return;

        hp -= damage;
        getImmunity();
        if (entity->hasComponent<SpriteComponent>())
            entity->getComponent<SpriteComponent>().StartBlinking(immunityTimeAfterHit,0.2f);
        if (entity->hasComponent<UIComponent>())
            entity->getComponent<UIComponent>().showDamage("-1");

        if (hp <= 0) {
            hp = 0;
            if (onDeathEffect) {
                onDeathEffect(*entity);
            }
        }
    }

    int getHp(){
        return hp;
    }

    void setOnDeathCallback(std::function<void(Entity&)> callback) {
        onDeathEffect = callback;
    }
};