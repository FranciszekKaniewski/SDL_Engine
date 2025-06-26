#include "Engine/Headers/game.h"
#include "Game/scenes/Headers/Scene1.hpp"
#include "Game/scenes/Headers/Scene2.hpp"
#include "Game/scenes/Headers/Scene3.hpp"

Game *game = nullptr;

int main(int argv, char** args) {

    const int FPS = 60;
    const int frameDelay = 1000 / FPS;

    Uint32 frameStart;
    int frameTime;

    game = new Game;

    game->init("Super Game", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,1920,1080,false);

    Scene* a = new UIScene1;
    Scene* b = new UIScene2;
    Scene* c = new UIScene3;
    game->scenes.push_back(a);
    game->scenes.push_back(b);
    game->scenes.push_back(c);
    game->changeScene(game->scenes[0]);

    while(game->running()) {

        frameStart = SDL_GetTicks();

        if (game->getCurrentScene()) {
            game->getCurrentScene()->handleEvents(*game);
            game->getCurrentScene()->update(*game);
            game->getCurrentScene()->render(*game);
        }
        game->handleEvents();

        frameTime = SDL_GetTicks() - frameStart;

        if(frameDelay > frameTime){
            SDL_Delay(frameDelay - frameTime);
        }
    }

    game->clean();

    return 0;
}
