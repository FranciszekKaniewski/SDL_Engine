#pragma once
#include "../SDL2/include/SDL2/SDL_ttf.h"
#include <iostream>
#include <ostream>
#include <string>

class Font {
public:
    TTF_Font* font = nullptr;
    const char *filePath;
    int fontSize;

    Font(){}
    Font(const char *filePath, int fontSize) : filePath(filePath), fontSize(fontSize){
        font = TTF_OpenFont(filePath, fontSize);
        if (!font) {
            std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
            return;
        }
    }

    void changeSize(int newSize){
        if (font) {
            TTF_CloseFont(font);
            font = nullptr;
        }

        fontSize = newSize;
        font = TTF_OpenFont(filePath, fontSize);
        if (!font) {
            std::cerr << "Failed to reopen font: " << TTF_GetError() << std::endl;
        }
    }

    void changeFontStyle(const char *filePath){
        if (font) {
            TTF_CloseFont(font);
            font = nullptr;
        }

        filePath = filePath;
        font = TTF_OpenFont(filePath, fontSize);
        if (!font) {
            std::cerr << "Failed to reopen font: " << TTF_GetError() << std::endl;
        }
    }

    ~Font(){}
};