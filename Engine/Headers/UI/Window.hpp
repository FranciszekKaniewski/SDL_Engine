#pragma once
#include "vector"
#include "./UIElements.hpp"
#include "../TextureManager.hpp"

class Window{
private:
    SDL_Texture* texture;
    std::vector<TextAreaB*> textAreas;
    std::vector<ButtonB*> buttons;
    std::vector<BoxB*> boxes;

public:
    SDL_Rect rect;
    bool isClosed;

    Window(SDL_Rect rect,const char* texturePath, bool isClosed=false): isClosed(isClosed){
        this->rect = rect;
        texture = TextureManager::LoadTexture(texturePath);
    }

    void updateMousePosClick(int x, int y){
        if(isClosed) return;
        for(ButtonB* btn : buttons){
            if (x >= btn->rect.x && x <= btn->rect.x + btn->rect.w &&
                y >= btn->rect.y && y <= btn->rect.y + btn->rect.h) {

                btn->setHover(true);
            }else{
                btn->setHover(false);
            }
        }
    }

    void changeRect(int x, int y, int w, int h){
        this->rect = {x, y, w, h};

        for(TextAreaB* textArea : textAreas){
            textArea->changeRect(x,y,w,h);
        }

        for(ButtonB* btn : buttons){
            btn->changeRect(x,y,w,h);
        }

        for(BoxB* box : boxes){
            box->changeRect(x,y,w,h);
        }
    }

    void handleClick(){
        if(isClosed) return;
        for(ButtonB* btn : buttons){
            btn->handleClick();
        }
    }

    void draw(){
        if(isClosed) return;

        if(texture) SDL_RenderCopy(Game::renderer, texture, nullptr, &rect);
        for(TextAreaB* textArea : textAreas){
            textArea->draw();
        }
        for(ButtonB* btn : buttons){
            btn->draw();
        }
        for(BoxB* box : boxes){
            box->draw();
        }
    }

    void addTextArea(Font font,const std::string& text, SDL_Color color={255,255,255}, SDL_Rect rect={0,0,64,64}){
        if(rect.x == -1)
            rect.x = this->rect.w/2 - rect.w/2;
        if(rect.y == -1)
            rect.y = this->rect.h/2 - rect.h/2;
        SDL_Rect windowedRect = {rect.x+this->rect.x,rect.y+this->rect.y,rect.w,rect.h};

        TextAreaB* textArea = new TextAreaB(font,text,color,windowedRect);
        textAreas.push_back(textArea);
    }

    void addButton(Font font, const char* texturePath, const std::string& text, SDL_Rect rect,std::function<void()> onClick , SDL_Color color={255,255,255}){
        if(rect.x == -1)
            rect.x = this->rect.w/2 - rect.w/2;
        if(rect.y == -1)
            rect.y = this->rect.h/2 - rect.h/2;
        SDL_Rect windowedRect = {rect.x+this->rect.x,rect.y+this->rect.y,rect.w,rect.h};

        ButtonB* btn = new ButtonB(font,texturePath,text,windowedRect,onClick,color);
        buttons.push_back(btn);
    }

    void addBox(Font font, const char* texturePath, const std::string& text,SDL_Rect rect, SDL_Color color={255,255,255}){
        if(rect.x == -1)
            rect.x = this->rect.w/2 - rect.w/2;
        if(rect.y == -1)
            rect.y = this->rect.h/2 - rect.h/2;
        SDL_Rect windowedRect = {rect.x+this->rect.x,rect.y+this->rect.y,rect.w,rect.h};

        BoxB* box = new BoxB(font,texturePath,text,windowedRect,color);
        boxes.push_back(box);
    }

    BoxB* getBoxById(int id){
        return boxes[id];
    }

    void toggle(){
        this->isClosed = !this->isClosed;
    }

    void clean(){
        for(TextAreaB* textArea : textAreas){
            textArea->clean();
        }
        textAreas.clear();

        for(ButtonB* btn : buttons){
            btn->clean();
        }
        buttons.clear();

        for(BoxB* box : boxes){
            box->clean();
        }
        boxes.clear();

        SDL_DestroyTexture(texture);
    }
};