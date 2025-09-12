#include "../../../Engine/Headers/UI/UIScene.hpp"
#include "../../../Engine/Headers/UI/UIFrame.hpp"

class UIScene1 : public UIScene {
public:
    UIScene1() = default;
    ~UIScene1() override = default;

    UIFrame* uiFrame;

    void onEnter(Game& game) override;
    void onExit(Game& game) override;

    void update(Game& game) override;
    void handleEvents(Game& game) override;
    void render(Game& game) override;
};