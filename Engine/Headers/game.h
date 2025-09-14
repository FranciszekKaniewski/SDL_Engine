#ifndef game_h
#define game_h

#include "iostream"
#include "vector"
#include <memory>

#include "../SDL2/include/SDL2/SDL.h"
#include "../SDL2/include/SDL2/SDL_image.h"
#include "../Headers/Scene.hpp"

class ColliderComponent;

struct WindowSize {
    int wight;
    int height;
};

class Game {
public:
    Game();
    ~Game();

    std::vector<Scene*> scenes;
    static float deltaTime;

    void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

    void handleEvents();
    void update();
    bool running() {return isRunning;};
    void render();
    void clean();
    const char* title;
    WindowSize getWindowSize() {
        int w,h;
        SDL_GetWindowSize(this->window,&w, &h);
        return {w,h};
    };

    static SDL_Renderer *renderer;
    static SDL_Event event;

    static bool isRunning;

    static SDL_Rect camera;

    enum groupLabels : std::size_t {
        groupMap,
        groupPlayers,
        groupEnemies,
        groupColliders
    };

    void changeScene(Scene* newScene);
    Scene* getCurrentScene() const { return currentScene; }
private:

    int cnt = 0;
    SDL_Window *window;
    Scene* currentScene = nullptr;
};

#endif /* game_h */
