#include "./Headers/TextureManager.hpp"

SDL_Texture* TextureManager::LoadTexture(const char *fileName) {

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
