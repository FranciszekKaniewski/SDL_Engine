#include "./Headers/TextureManager.hpp"
#include "./Headers/game.h"

SDL_Texture* TextureManager::LoadTexture(const char *fileName) {
    if(strcmp(fileName, "none") == 0) return nullptr;

    SDL_Surface* tempSurface = IMG_Load(fileName);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer,tempSurface);
    SDL_FreeSurface(tempSurface);

    return tex;
}

SDL_Texture* TextureManager::LoadTextTexture(Font font, const std::string& label,SDL_Color color) {

    SDL_Surface* tempSurface = TTF_RenderText_Solid(font.font, label.c_str(), color);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer,tempSurface);
    SDL_FreeSurface(tempSurface);

    return tex;
}

void TextureManager::Draw(SDL_Texture *tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip) {
    SDL_RenderCopyEx(Game::renderer,tex,&src,&dest,0,nullptr,flip);
}

void TextureManager::DrawCircle(int centreX, int centreY, int radius, SDL_Color color) {
    SDL_SetRenderDrawColor(Game::renderer, color.r, color.g, color.b, color.a);

    int offsetX = 0;
    int offsetY = radius;
    int d = 1 - radius;

    while (offsetY >= offsetX) {
        SDL_RenderDrawPoint(Game::renderer, centreX + offsetX, centreY + offsetY);
        SDL_RenderDrawPoint(Game::renderer, centreX + offsetY, centreY + offsetX);
        SDL_RenderDrawPoint(Game::renderer, centreX - offsetX, centreY + offsetY);
        SDL_RenderDrawPoint(Game::renderer, centreX - offsetY, centreY + offsetX);
        SDL_RenderDrawPoint(Game::renderer, centreX + offsetX, centreY - offsetY);
        SDL_RenderDrawPoint(Game::renderer, centreX + offsetY, centreY - offsetX);
        SDL_RenderDrawPoint(Game::renderer, centreX - offsetX, centreY - offsetY);
        SDL_RenderDrawPoint(Game::renderer, centreX - offsetY, centreY - offsetX);

        if (d < 0) {
            d += 2 * offsetX + 3;
        } else {
            d += 2 * (offsetX - offsetY) + 5;
            offsetY--;
        }
        offsetX++;
    }
}
