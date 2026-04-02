#pragma once

enum class HitboxType { CIRCLE, OBB };

// -----------------------------------------------------------------------
//  AttackProfile – pojedynczy cios broni (np. topór, miecz)
// -----------------------------------------------------------------------
struct AttackProfile {
    int        damage;
    float      cooldown;    // przerwa między atakami (s)
    float      duration;    // jak długo hitbox jest aktywny (s)
    float      reach;       // odległość od centrum entity do centrum hitboxa (px)
    HitboxType hitboxType;
    float      radius;      // CIRCLE: promień (px)
    float      halfW;       // OBB: półwymiar wzdłuż osi obrotu (grubość ostrza)
    float      halfH;       // OBB: półwymiar prostopadle (długość ostrza)
    float      swingAngle;  // OBB: kąt zamachu w radianach (0 = pchnięcie do przodu)
};

// -----------------------------------------------------------------------
//  PunchStep – jeden cios w kombo
//    halfW      – głębokość OBB wzdłuż kierunku uderzenia
//    halfH      – szerokość OBB
//    sideOffset – boczne przesunięcie (+ prawo, - lewo, 0 środek)
//    damage     – obrażenia
//    duration   – czas aktywności hitboxa (s)
// -----------------------------------------------------------------------
struct PunchStep {
    float halfW;
    float halfH;
    float sideOffset;
    int   damage;
    float duration;
};

// -----------------------------------------------------------------------
//  ComboProfile – seria ciosów z oknem łączenia i przerwą po pełnym kombo
//    reach        – odległość od entity do TYLNEJ krawędzi hitboxa (px)
//                   (hitbox "wyrasta" od tej krawędzi do przodu)
//    chainWindow  – ile sekund gracz ma na kliknięcie następnego ciosu
//    comboCooldown– przymusowa przerwa po wykonaniu wszystkich kroków (s)
// -----------------------------------------------------------------------
struct ComboProfile {
    static constexpr int MAX_STEPS = 3;
    PunchStep steps[MAX_STEPS];
    int   stepCount;
    float reach;
    float chainWindow;
    float comboCooldown;
};

// -----------------------------------------------------------------------
//  WeaponStep – jeden krok kombo broni
//    halfW      – grubość ostrza (wzdłuż promienia)
//    halfH      – długość ostrza (prostopadle)
//    swingAngle – kąt zamachu: + = od lewej do prawej,
//                              - = od prawej do lewej,
//                              0 = pchnięcie do przodu (OBB rośnie)
//    damage     – obrażenia
//    duration   – czas trwania kroku (s)
//    reach      – odległość od entity do centrum hitboxa (px)
// -----------------------------------------------------------------------
struct WeaponStep {
    float halfW;
    float halfH;
    float swingAngle;
    int   damage;
    float duration;
    float reach;
};

// -----------------------------------------------------------------------
//  WeaponComboProfile – kombo broni (np. dwa zamachy + finisher)
//    chainWindow   – okno czasowe na kolejny cios (s)
//    comboCooldown – przerwa po pełnym kombo (s)
//    cooldown      – minimalny czas przed ponownym rozpoczęciem kombo (s)
// -----------------------------------------------------------------------
struct WeaponComboProfile {
    static constexpr int MAX_STEPS = 3;
    WeaponStep steps[MAX_STEPS];
    int   stepCount;
    float chainWindow;
    float comboCooldown;
    float cooldown;
};
