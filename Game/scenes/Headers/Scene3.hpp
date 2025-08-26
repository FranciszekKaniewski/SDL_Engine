#include "../../../Engine/Headers/GameScene.hpp"
#include "../../../Engine/Headers/Map.hpp"
#include "../../../Engine/ECS_Components/Components.hpp"

class UIScene3 : public GameScene {
public:
    UIScene3() = default;
    ~UIScene3() override = default;

    Manager manager;
    Map* map;

    void onEnter(Game& game) override;
    void onExit(Game& game) override;

    void update(Game& game) override;
    void render(Game& game) override;
};