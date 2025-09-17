#pragma once
#include "../Font.hpp"
#include "../TextureManager.hpp"

#include <functional>

class TextAreaB{
private:
    const std::string& text;
    SDL_Rect rect;
    SDL_Texture* texture = nullptr;

public:
    Font font;
    SDL_Color color;

    TextAreaB(Font font,const std::string& text, SDL_Color color={255,255,255}, SDL_Rect rect={0,0,0,0}): text(text){
        this->font = font;
        this->color = color;
        this->rect=rect;

        texture = TextureManager::LoadTextTexture(font,text,color);
    };

    void changeRect(int x, int y, int w, int h) {
        this->rect = {x, y, w, h};
    }

    void draw(){
        SDL_RenderCopy(Game::renderer, texture, nullptr, &rect);
    }

    void clean(){
        SDL_DestroyTexture(texture);
    }
};

class ButtonB{
private:
    SDL_Texture* texture = nullptr;
    std::function<void()> onClick;

    TextAreaB textArea;
    bool isHovered;

public:
    SDL_Rect rect;

    ButtonB(Font font, const char* texturePath, const std::string& text, SDL_Rect rect,std::function<void()> onClick , SDL_Color color={255,255,255}):
            textArea(font,text,color,rect)
    {
        this->rect=rect;
        this->onClick = onClick;

        texture = TextureManager::LoadTexture(texturePath);
    };

    void setHover(bool state) {
        isHovered = state;
    }

    void handleClick(){
        if(onClick && isHovered) onClick();
    }

    void changeRect(int x, int y, int w, int h) {
        this->rect = {x, y, w, h};
    }

    void draw(){
        if(isHovered)
            SDL_SetTextureColorMod(texture, 128, 128, 128);
        else
            SDL_SetTextureColorMod(texture, 255, 255, 255);
        SDL_RenderCopy(Game::renderer, texture, nullptr, &rect);
        textArea.draw();
    }

    void clean(){
        textArea.clean();
        SDL_DestroyTexture(texture);
    }
};

class BoxB{
private:
    SDL_Texture* texture = nullptr;
    SDL_Texture* overTexture = nullptr;
    SDL_Rect rect;

    TextAreaB* textArea;
    bool isSelected;

public:
    BoxB(Font font, const char* texturePath, const std::string& text,SDL_Rect rect, SDL_Color color={255,255,255}):
            textArea(new TextAreaB(font,text,color, {rect.x + rect.w*2/3 , rect.y + rect.h*2/3,rect.w/3,rect.h/3}))
    {
        this->rect = rect;

        texture = TextureManager::LoadTexture(texturePath);
    };

    void setSelected(bool state) {
        isSelected = state;
    }

    void changeRect(int x, int y, int w, int h) {
        this->rect = {x, y, w, h};
    }

    void addItem(const char* texturePath){
        overTexture = TextureManager::LoadTexture(texturePath);
    }

    void updateText(const std::string& text){
        if(textArea) {
            Font f = textArea->font;
            SDL_Color color = textArea->color;
            textArea->clean();
            delete textArea;

            textArea = new TextAreaB(f, text, color, {rect.x + rect.w*2/3, rect.y + rect.h*2/3, rect.w/3, rect.h/3});
        }
    }

    void draw(){
        if(isSelected)
            SDL_SetTextureColorMod(texture, 128, 128, 128);
        else
            SDL_SetTextureColorMod(texture, 255, 255, 255);
        SDL_RenderCopy(Game::renderer, texture, nullptr, &rect);

        if(overTexture) {
            SDL_RenderCopy(Game::renderer, overTexture, nullptr, &rect);
        }

        textArea->draw();
    }

    void clean(){
        textArea->clean();
        delete textArea;
        if(texture) SDL_DestroyTexture(texture);
        if(overTexture) SDL_DestroyTexture(overTexture);
    }
};