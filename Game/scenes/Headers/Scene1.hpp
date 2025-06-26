#include "../../../Engine/Headers/UI/UIScene.hpp"

class UIScene1 : public UIScene {
public:
    UIScene1() = default;
    ~UIScene1() override = default;

    void onEnter(Game& game) override;
    void onExit(Game& game) override;

    void update(Game& game) override;
    void render(Game& game) override;
};