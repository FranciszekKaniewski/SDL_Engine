#include "../SDL2/include/SDL2/SDL.h"
#include "../SDL2/include/SDL2/SDL_image.h"
#include "../SDL2/include/SDL2/SDL_ttf.h"
#include "../Game/scenes/Headers/Scene1.hpp"
#include "../Game/scenes/Headers/Scene2.hpp"

#include "Headers/game.h"
#include "./ECS_Components/Components.hpp"
#include "./Headers/Map.hpp"

bool Game::isRunning = false;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;
SDL_Rect Game::camera = {0,0,1024,1024};
float Game::deltaTime = 0.0f;
ItemsManager Game::itemsManager;
Audio Game::audio;

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen){
    this->title = title;

    int flags = 0;
    if(fullscreen){
        flags = SDL_WINDOW_FULLSCREEN;
    }

    // Validation
    if(SDL_Init(SDL_INIT_EVERYTHING) == 0){
        std::cout << "SDL works!" << std::endl;

        window = SDL_CreateWindow(title,xpos,ypos,width,height, flags);
        if(window){
            std::cout << "Window Created!" << std::endl;
        }
        renderer = SDL_CreateRenderer(window,-1,0);
        if(renderer){
            SDL_SetRenderDrawColor(renderer,255,255,255,255);
            std::cout << "Renderer Created!" << std::endl;
        }
        if (TTF_Init() == -1) {
            std::cerr << "TTF_Init error: " << TTF_GetError() << std::endl;
            return;
        }
        if (Mix_Init(MIX_INIT_MP3) == 0) {
            std::cerr << "Mix_Init Error: " << Mix_GetError() << std::endl;
            return;
        }
        if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
            std::cerr << "Mix_OpenAudio Error: " << Mix_GetError() << std::endl;
            return;
        }
        isRunning = true;
    }else{
        isRunning = false;
    }
}

void Game::handleEvents() {
    while(SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                isRunning = false;
                break;
        }

        if (currentScene) {
            currentScene->handleEvents(*this);
        }
    }
}

void Game::update() {

}

void Game::render() {
    SDL_RenderClear(renderer);

    SDL_RenderPresent(renderer);
}

void Game::changeScene(Scene* newScene) {
    if (currentScene) {
        currentScene->onExit(*this);
    }
    currentScene = newScene;
    currentScene->onEnter(*this);
}

void Game::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game closed!" << std::endl;
}

Game::Game() {}
Game::~Game() {
    if (currentScene) {
        currentScene = nullptr;
    }

    for (Scene* scene : scenes) {
        delete scene;
    }
    scenes.clear();
}