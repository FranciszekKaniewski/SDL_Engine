#ifndef game_h
#define game_h

#include "iostream"
#include "vector"
#include <memory>

#include "../SDL2/include/SDL2/SDL.h"
#include "../SDL2/include/SDL2/SDL_image.h"
#include "../Headers/Scene.hpp"
#include "./ItemsManager.hpp"
#include "./Audio.hpp"

class ColliderComponent;

struct WindowSize {
    int wight;
    int height;
};

struct GameSettings{
    bool FPSLimit = true;
    int maxFPS = 144;
    WindowSize windowSize = {1920,1080};
};

class Game {
public:
    Game();
    ~Game();

    std::vector<Scene*> scenes;
    static float deltaTime;

    static ItemsManager itemsManager;

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
    static Audio audio;

    static bool isRunning;

    static SDL_Rect camera;

    enum groupLabels : std::size_t {
        groupMap,
        groupPlayers,
        groupEnemies,
        groupColliders,
        groupItems
    };

    void changeScene(Scene* newScene);
    Scene* getCurrentScene() const { return currentScene; }

    int currentFPS = 0;
    GameSettings gameSettings;
    static bool devMode;
private:

    int cnt = 0;
    SDL_Window *window;
    Scene* currentScene = nullptr;
};

#endif /* game_h */
