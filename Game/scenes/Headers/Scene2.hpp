#include "../../../Engine/Headers/UI/UIScene.hpp"

class UIScene2 : public UIScene {
public:
    UIScene2() = default;
    ~UIScene2() override = default;

    void onEnter(Game& game) override;
    void onExit(Game& game) override;

    void update(Game& game) override;
    void render(Game& game) override;
};