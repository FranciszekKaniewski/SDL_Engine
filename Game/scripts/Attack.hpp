#pragma once
#include <cmath>
#include <algorithm>
#include <vector>
#include "AttackProfiles.hpp"
#include "../../Engine/ECS_Components/Components.hpp"
#include "../../Engine/Headers/Collisions.hpp"
#include "../../Engine/Headers/game.h"
#include "../../Engine/Headers/TextureManager.hpp"

// -----------------------------------------------------------------------
//  AttackComponent
//
//  Tryb dobierany automatycznie na podstawie trzymanego przedmiotu:
//    – brak broni / ITEM       → UNARMED COMBO  (AttackProfiles::Unarmed)
//    – WEAPON z weaponComboProfile → WEAPON COMBO
//    – WEAPON z attackProfile  → WEAPON SINGLE (pchnięcie/zamach)
//
//  Dodaj do entity:
//    entity.addComponent<AttackComponent>(manager.getGroup(groupColliders));
//  Wywołaj przy LPM:
//    entity.getComponent<AttackComponent>().triggerAttack();
// -----------------------------------------------------------------------

class AttackComponent : public Component {
public:
    float                 damageMultiplier = 1.0f;
    std::vector<Entity*>& targets;

private:
    // ----------------------------------------------------------------
    //  Wspólne – zapisywane przy triggerze
    // ----------------------------------------------------------------
    float entityCX = 0.f, entityCY = 0.f;
    float dirX = 1.f, dirY = 0.f;
    float attackAngle = 0.f;          // kąt do kursora w chwili triggera
    std::vector<Entity*> hitEntities;

    // Bieżące wymiary hitboxa (aktualizowane co klatkę)
    float activeHalfW = 0.f;
    float activeHalfH = 0.f;
    float hitX = 0.f, hitY = 0.f;

    // ----------------------------------------------------------------
    //  Tryb WEAPON SINGLE – pojedynczy cios z cooldownem
    // ----------------------------------------------------------------
    AttackProfile currentWeaponProfile{};
    float weaponCooldownTimer = 0.f;
    float weaponAttackTimer   = 0.f;
    bool  weaponAttacking     = false;

    // ----------------------------------------------------------------
    //  Wspólna maszyna stanów dla obu kombo (unarmed i weapon)
    // ----------------------------------------------------------------
    enum class ComboState { IDLE, PUNCHING, CHAIN_WINDOW, FULL_COOLDOWN };
    ComboState comboState      = ComboState::IDLE;
    float      comboStateTimer = 0.f;
    int        comboStep       = 0;
    float      stepDuration    = 0.f;  // duration bieżącego kroku (dla progress)
    float      comboCooldownTimer = 0.f;  // cooldown przed ponownym startem kombo broni

    // ----------------------------------------------------------------
    //  Helpers: kolizje OBB
    // ----------------------------------------------------------------
    static bool OBBvsCircle(float cx, float cy, float ax, float ay,
                             float hw, float hh,
                             float circX, float circY, float circR)
    {
        float dx = circX - cx, dy = circY - cy;
        float lx = dx * ax + dy * ay;
        float ly = dx * (-ay) + dy * ax;
        float px = std::clamp(lx, -hw, hw);
        float py = std::clamp(ly, -hh, hh);
        float d  = (lx - px) * (lx - px) + (ly - py) * (ly - py);
        return d <= circR * circR;
    }

    static bool OBBvsRect(float cx, float cy, float ax, float ay,
                           float hw, float hh, const SDL_Rect& r)
    {
        float rcx = r.x + r.w * 0.5f, rcy = r.y + r.h * 0.5f;
        float rhw = r.w * 0.5f, rhh = r.h * 0.5f;
        float tx = rcx - cx, ty = rcy - cy;
        if (std::abs(tx * ax + ty * ay)        > hw + std::abs(rhw * ax)     + std::abs(rhh * ay))   return false;
        if (std::abs(tx * (-ay) + ty * ax)     > hh + std::abs(rhw * (-ay)) + std::abs(rhh * ax))   return false;
        if (std::abs(tx)                       > std::abs(hw * ax) + std::abs(hh * (-ay)) + rhw)    return false;
        if (std::abs(ty)                       > std::abs(hw * ay) + std::abs(hh * ax)    + rhh)    return false;
        return true;
    }

    void drawOBB(float cx, float cy, float hw, float hh, SDL_Color col) const {
        float ax = dirX, ay = dirY, px = -ay, py = ax;
        float c[4][2] = {
            { cx + ax*hw + px*hh, cy + ay*hw + py*hh },
            { cx + ax*hw - px*hh, cy + ay*hw - py*hh },
            { cx - ax*hw - px*hh, cy - ay*hw - py*hh },
            { cx - ax*hw + px*hh, cy - ay*hw + py*hh }
        };
        SDL_SetRenderDrawColor(Game::renderer, col.r, col.g, col.b, col.a);
        for (int i = 0; i < 4; ++i) {
            int j = (i + 1) % 4;
            SDL_RenderDrawLine(Game::renderer,
                (int)(c[i][0] - Game::camera.x), (int)(c[i][1] - Game::camera.y),
                (int)(c[j][0] - Game::camera.x), (int)(c[j][1] - Game::camera.y));
        }
    }

    // ----------------------------------------------------------------
    //  Helpers: wykrywanie trybu broni
    // ----------------------------------------------------------------
    bool hasWeaponCombo() const {
        if (!entity->hasComponent<HoldingComponent>()) return false;
        auto& h = entity->getComponent<HoldingComponent>();
        return h.item && h.item->type == WEAPON && h.item->weaponComboProfile;
    }

    bool hasWeaponSingle() const {
        if (!entity->hasComponent<HoldingComponent>()) return false;
        auto& h = entity->getComponent<HoldingComponent>();
        return h.item && h.item->type == WEAPON && h.item->attackProfile && !h.item->weaponComboProfile;
    }

    const WeaponComboProfile& getWeaponCombo() const {
        return *entity->getComponent<HoldingComponent>().item->weaponComboProfile;
    }

    // ----------------------------------------------------------------
    //  Wspólne: zapisz pozycję entity i kierunek do kursora
    // ----------------------------------------------------------------
    void captureOriginAndDir() {
        auto& tf = entity->getComponent<TransformComponent>();
        entityCX = tf.position.x + (tf.width  * tf.scale) / 2.0f;
        entityCY = tf.position.y + (tf.height * tf.scale) / 2.0f;
        int mx, my;
        SDL_GetMouseState(&mx, &my);
        float dx = (float)mx + Game::camera.x - entityCX;
        float dy = (float)my + Game::camera.y - entityCY;
        float len = std::sqrt(dx * dx + dy * dy);
        if (len > 0.0001f) { dirX = dx / len; dirY = dy / len; }
        else               { dirX = 1.f;      dirY = 0.f; }
        attackAngle = std::atan2(dirY, dirX);
    }

    // ----------------------------------------------------------------
    //  Sprawdza kolizje aktualnego hitboxa OBB ze wszystkimi celami
    // ----------------------------------------------------------------
    void checkHits(int damage) {
        for (auto* t : targets) {
            if (!t->isActive()) continue;
            if (!t->hasComponent<HpComponent>()) continue;
            if (!t->hasComponent<ColliderComponent>()) continue;
            if (std::find(hitEntities.begin(), hitEntities.end(), t) != hitEntities.end()) continue;

            auto& col = t->getComponent<ColliderComponent>();
            bool hit = (col.type == CIRCLE)
                ? OBBvsCircle(hitX, hitY, dirX, dirY, activeHalfW, activeHalfH,
                              (float)col.circleCollider.x, (float)col.circleCollider.y, (float)col.circleCollider.r)
                : OBBvsRect  (hitX, hitY, dirX, dirY, activeHalfW, activeHalfH, col.collider);
            if (hit) {
                t->getComponent<HpComponent>().getDamage((int)(damage * damageMultiplier));
                hitEntities.push_back(t);
            }
        }
    }

    // ----------------------------------------------------------------
    //  Wspólna animacja kroku: zamach (swingAngle≠0) lub pchnięcie (=0)
    //  Ustawia dirX/dirY, hitX/hitY, activeHalfW/activeHalfH.
    // ----------------------------------------------------------------
    void animateStep(float halfW, float halfH, float swingAngle, float reach, float progress) {
        if (swingAngle != 0.f) {
            // Zamach – OBB obraca się łukiem. Formuła działa dla obu kierunków:
            //   +swingAngle → od lewej do prawej
            //   -swingAngle → od prawej do lewej
            float angle = attackAngle - swingAngle * 0.5f + progress * swingAngle;
            dirX = std::cos(angle);
            dirY = std::sin(angle);
            hitX = entityCX + dirX * reach;
            hitY = entityCY + dirY * reach;
            activeHalfW = halfW;
            activeHalfH = halfH;
        } else {
            // Pchnięcie rosnące – tylna krawędź stała, przednia rośnie
            activeHalfW       = halfW * progress;
            activeHalfH       = halfH;
            float centerReach = (reach - halfW) + activeHalfW;
            hitX = entityCX + dirX * centerReach;
            hitY = entityCY + dirY * centerReach;
        }
    }

    // ================================================================
    //  WEAPON SINGLE – jeden cios (zamach lub pchnięcie)
    // ================================================================
    void triggerWeaponSingle() {
        if (weaponCooldownTimer > 0.f) return;
        currentWeaponProfile = *entity->getComponent<HoldingComponent>().item->attackProfile;
        captureOriginAndDir();
        weaponAttacking     = true;
        weaponAttackTimer   = currentWeaponProfile.duration;
        weaponCooldownTimer = currentWeaponProfile.cooldown;
        hitEntities.clear();
    }

    void updateWeaponSingle() {
        if (weaponCooldownTimer > 0.f) weaponCooldownTimer -= Game::deltaTime;
        if (!weaponAttacking) return;
        weaponAttackTimer -= Game::deltaTime;
        float elapsed  = currentWeaponProfile.duration - weaponAttackTimer;
        float progress = std::clamp(elapsed / currentWeaponProfile.duration, 0.f, 1.f);
        animateStep(currentWeaponProfile.halfW, currentWeaponProfile.halfH,
                    currentWeaponProfile.swingAngle, currentWeaponProfile.reach, progress);
        checkHits(currentWeaponProfile.damage);
        if (weaponAttackTimer <= 0.f) {
            weaponAttacking = false;
            hitEntities.clear();
        }
    }

    // ================================================================
    //  WEAPON COMBO – kombo broni (np. topór)
    // ================================================================
    void startWeaponComboStep(int step) {
        const WeaponComboProfile& p = getWeaponCombo();
        captureOriginAndDir();
        stepDuration    = p.steps[step].duration;
        comboState      = ComboState::PUNCHING;
        comboStateTimer = stepDuration;
        hitEntities.clear();
    }

    void triggerWeaponCombo() {
        if (comboCooldownTimer > 0.f) return;
        switch (comboState) {
            case ComboState::IDLE:
                comboStep = 0;
                startWeaponComboStep(0);
                break;
            case ComboState::CHAIN_WINDOW:
                comboStep++;
                startWeaponComboStep(comboStep);
                break;
            default: break;
        }
    }

    void updateWeaponCombo() {
        if (comboCooldownTimer > 0.f) comboCooldownTimer -= Game::deltaTime;
        if (comboState == ComboState::IDLE) return;

        comboStateTimer -= Game::deltaTime;

        if (comboState == ComboState::PUNCHING) {
            const WeaponComboProfile& p = getWeaponCombo();
            const WeaponStep& step = p.steps[comboStep];
            float elapsed  = stepDuration - comboStateTimer;
            float progress = std::clamp(elapsed / stepDuration, 0.f, 1.f);
            animateStep(step.halfW, step.halfH, step.swingAngle, step.reach, progress);
            checkHits(step.damage);

            if (comboStateTimer <= 0.f) {
                bool isLast = (comboStep >= p.stepCount - 1);
                if (isLast) {
                    comboState        = ComboState::FULL_COOLDOWN;
                    comboStateTimer   = p.comboCooldown;
                    comboCooldownTimer = p.cooldown;
                    comboStep         = 0;
                } else {
                    comboState      = ComboState::CHAIN_WINDOW;
                    comboStateTimer = p.chainWindow;
                }
                hitEntities.clear();
            }
        }
        else if (comboState == ComboState::CHAIN_WINDOW) {
            if (comboStateTimer <= 0.f) { comboState = ComboState::IDLE; comboStep = 0; }
        }
        else if (comboState == ComboState::FULL_COOLDOWN) {
            if (comboStateTimer <= 0.f) comboState = ComboState::IDLE;
        }
    }

    // ================================================================
    //  UNARMED COMBO – gołe pięści
    // ================================================================
    void startUnarmedStep(int step) {
        captureOriginAndDir();
        stepDuration    = AttackProfiles::Unarmed.steps[step].duration;
        comboState      = ComboState::PUNCHING;
        comboStateTimer = stepDuration;
        hitEntities.clear();
    }

    void triggerUnarmed() {
        switch (comboState) {
            case ComboState::IDLE:
                comboStep = 0;
                startUnarmedStep(0);
                break;
            case ComboState::CHAIN_WINDOW:
                comboStep++;
                startUnarmedStep(comboStep);
                break;
            default: break;
        }
    }

    void updateUnarmed() {
        if (comboState == ComboState::IDLE) return;
        comboStateTimer -= Game::deltaTime;

        if (comboState == ComboState::PUNCHING) {
            const PunchStep& step = AttackProfiles::Unarmed.steps[comboStep];
            float elapsed  = stepDuration - comboStateTimer;
            float progress = std::clamp(elapsed / stepDuration, 0.f, 1.f);

            activeHalfW = step.halfW * progress;
            activeHalfH = step.halfH;
            float centerReach = AttackProfiles::Unarmed.reach + activeHalfW;
            float perpX = -dirY, perpY = dirX;
            hitX = entityCX + dirX * centerReach + perpX * step.sideOffset;
            hitY = entityCY + dirY * centerReach + perpY * step.sideOffset;

            checkHits(step.damage);

            if (comboStateTimer <= 0.f) {
                bool isLast = (comboStep >= AttackProfiles::Unarmed.stepCount - 1);
                if (isLast) {
                    comboState      = ComboState::FULL_COOLDOWN;
                    comboStateTimer = AttackProfiles::Unarmed.comboCooldown;
                    comboStep       = 0;
                } else {
                    comboState      = ComboState::CHAIN_WINDOW;
                    comboStateTimer = AttackProfiles::Unarmed.chainWindow;
                }
                hitEntities.clear();
            }
        }
        else if (comboState == ComboState::CHAIN_WINDOW) {
            if (comboStateTimer <= 0.f) { comboState = ComboState::IDLE; comboStep = 0; }
        }
        else if (comboState == ComboState::FULL_COOLDOWN) {
            if (comboStateTimer <= 0.f) comboState = ComboState::IDLE;
        }
    }

public:
    explicit AttackComponent(std::vector<Entity*>& targets) : targets(targets) {}

    void triggerAttack() {
        if      (hasWeaponCombo())   triggerWeaponCombo();
        else if (hasWeaponSingle())  triggerWeaponSingle();
        else                         triggerUnarmed();
    }

    void update() override {
        if      (hasWeaponCombo())   updateWeaponCombo();
        else if (hasWeaponSingle())  updateWeaponSingle();
        else                         updateUnarmed();
    }

    void draw() override {
        if (!Game::devMode) return;
        bool active = weaponAttacking
            || (!hasWeaponSingle() && comboState == ComboState::PUNCHING);
        if (!active) return;
        drawOBB(hitX, hitY, activeHalfW, activeHalfH, {255, 165, 0, 255});
    }

    bool getIsAttacking() const {
        if (hasWeaponSingle()) return weaponAttacking;
        return comboState == ComboState::PUNCHING;
    }

    float getCooldownProgress() const {
        if (hasWeaponSingle()) {
            float cd = currentWeaponProfile.cooldown;
            return (cd > 0.f) ? 1.f - (weaponCooldownTimer / cd) : 1.f;
        }
        return (comboState == ComboState::FULL_COOLDOWN) ? 0.f : 1.f;
    }
};
