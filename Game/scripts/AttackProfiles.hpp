#pragma once
#include "../../Engine/Headers/AttackProfile.hpp"

// -----------------------------------------------------------------------
//  AttackProfiles – wszystkie ataki w grze.
//
//  Dodając nową broń (AttackProfile):
//    1. Dopisz profil poniżej.
//    2. Przypisz go w Scene: Game::itemsManager.getItemByName("X")->attackProfile = &AttackProfiles::X;
//

namespace AttackProfiles {

    // ----------------------------------------------------------------
    //  Gołe pięści – kombo 3 ciosów
    //    Cios 1: prawy prosty   – mały, przesunięty w prawo
    //    Cios 2: lewy prosty    – mały, przesunięty w lewo
    //    Cios 3: szeroki środek – duży, wyśrodkowany
    // ----------------------------------------------------------------
    inline ComboProfile Unarmed = {
        {   // steps[3]
            PunchStep{ 28.f, 20.f, +20.f, 3, 0.1f },  // prawy prosty
            PunchStep{ 28.f, 20.f, -20.f, 3, 0.1f },  // lewy prosty
            PunchStep{ 28.f, 20.f,   0.f, 5, 0.2f }   // szeroki środkowy
        },
        /*stepCount    */ 3,
        /*reach        */ 32.f,   // tylna krawędź 32px od centrum gracza
        /*chainWindow  */ 0.5f,  // okno na następny cios
        /*comboCooldown*/ 1.0f   // przerwa po pełnym kombo
    };

    // ----------------------------------------------------------------
    //  Topór – kombo 3 ciosów:
    //    Cios 1: zamach od lewej do prawej (+swingAngle)
    //    Cios 2: zamach od prawej do lewej (-swingAngle)
    //    Cios 3: szeroki finisher do przodu (swingAngle=0, OBB rośnie)
    // ----------------------------------------------------------------
    inline WeaponComboProfile AxeCombo = {
        {   // steps[3]
            WeaponStep{ 10.f, 42.f, +1.1f, 10, 0.15f, 72.f },  // L→R zamach
            WeaponStep{ 10.f, 42.f, -1.1f, 10, 0.15f, 72.f },  // R→L zamach
            WeaponStep{ 55.f, 10.f,  0.0f, 18, 0.2f, 60.f }   // finisher
        },
        /*stepCount   */ 3,
        /*chainWindow */ 0.55f,
        /*comboCooldown*/ 0.8f,
        /*cooldown    */ 0.9f
    };

} // namespace AttackProfiles
