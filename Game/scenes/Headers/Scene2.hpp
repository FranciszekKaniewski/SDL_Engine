#include "../../../Engine/Headers/UI/UIScene.hpp"
#include "../../../Engine/Headers/UI/UIFrame.hpp"

class UIScene2 : public UIScene {
public:
    UIScene2() = default;
    ~UIScene2() override = default;

    UIFrame* uiFrame;

    void onEnter(Game& game) override;
    void onExit(Game& game) override;

    void update(Game& game) override;
    void handleEvents(Game& game) override;
    void render(Game& game) override;
};