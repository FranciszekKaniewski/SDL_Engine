#include "Engine/Headers/game.h"
#include "Game/scenes/Headers/Scene1.hpp"
#include "Game/scenes/Headers/Scene2.hpp"
#include "Game/scenes/Headers/Scene3.hpp"

Game *game = nullptr;

int main(int argv, char** args) {

    Uint32 frameStart;
    Uint32 lastTime = SDL_GetTicks();

    game = new Game;
    game->init("Super Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, game->gameSettings.windowSize.wight, game->gameSettings.windowSize.height, false);

    Scene* a = new UIScene1;
    Scene* b = new UIScene2;
    Scene* c = new UIScene3;
    game->scenes.push_back(a);
    game->scenes.push_back(b);
    game->scenes.push_back(c);
    game->changeScene(game->scenes[0]);

    Game::itemsManager = ItemsManager();

    int frames = 0;
    Uint32 fpsTimer = SDL_GetTicks();
    game->currentFPS = 0;

    const int FIXED_FPS = 60;
    const float MS_PER_UPDATE = 1000.0f / FIXED_FPS;
    float lag = 0.0f;

    while (game->running()) {
        frameStart = SDL_GetTicks();
        float elapsed = frameStart - lastTime;
        lastTime = frameStart;
        lag += elapsed;

        game->handleEvents();
        if (game->getCurrentScene()) {
            game->getCurrentScene()->handleEvents(*game);
        }

        while (lag >= MS_PER_UPDATE) {
            Game::deltaTime = MS_PER_UPDATE / 1000.0f;
            if (game->getCurrentScene()) {
                game->getCurrentScene()->update(*game);
            }
            lag -= MS_PER_UPDATE;
        }

        if (game->getCurrentScene()) {
            game->getCurrentScene()->render(*game);
        }

        Uint32 frameTimeMs = SDL_GetTicks() - frameStart;
        if (game->gameSettings.FPSLimit && frameTimeMs < (1000 / game->gameSettings.maxFPS)) {
            SDL_Delay((1000 / game->gameSettings.maxFPS) - frameTimeMs);
        }

        frames++;
        if (SDL_GetTicks() - fpsTimer >= 1000) {
            game->currentFPS = frames;
            frames = 0;
            fpsTimer += 1000;
        }
    }

    game->clean();
    return 0;
}

