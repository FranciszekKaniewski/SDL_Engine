#pragma once
#include "Font.hpp"

class TextureManager{
public:
    static SDL_Texture* LoadTexture(const char* fileName);
    static SDL_Texture* LoadTextTexture(Font font, const std::string& label,SDL_Color color);
    static void Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip);
    static void DrawCircle(int centreX, int centreY, int radius, SDL_Color color);
};