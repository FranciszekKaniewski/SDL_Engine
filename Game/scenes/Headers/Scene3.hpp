#include "../../../Engine/Headers/GameScene.hpp"
#include "../../../Engine/Headers/Map.hpp"
#include "../../../Engine/ECS_Components/Components.hpp"
#include "../../../Engine/Headers/UI/UIFrame.hpp"
#include "../../../Engine/Headers/Collisions.hpp"
#include "../../scripts/ItemCollisions.hpp"
#include "../../scripts/Player_WallsCollision.hpp"
#include "../../../Engine/ECS_Components/Headers/HP_Component.hpp"

class UIScene3 : public GameScene {
public:
    UIScene3() = default;
    ~UIScene3() override = default;

    Manager manager;
    Map* map;

    UIFrame* uiFrame;
    Window* inventoryWindow;
    Window* devWindow;
    Window* escWindow;
    Window* hpWindow;

    void onEnter(Game& game) override;
    void onExit(Game& game) override;

    void update(Game& game) override;
    void handleEvents(Game& game) override;
    void render(Game& game) override;

    //helpers
    void createItem(const char* path, const std::string& tag, int x, int y);
};