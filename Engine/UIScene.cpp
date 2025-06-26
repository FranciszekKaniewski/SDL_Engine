#include "Headers/UI/UIScene.hpp"

UIScene::UIScene() {}

UIScene::~UIScene() {
    clear();
}

void UIScene::clear() {
    for (auto& btn : buttons) {
        if (btn.texture) {
            SDL_DestroyTexture(btn.texture);
        }
    }
    buttons.clear();
    for (auto& t : textAreas) {
        if (t.texture) {
            SDL_DestroyTexture(t.texture);
        }
    }
    textAreas.clear();
    if (font.font) {
        TTF_CloseFont(font.font);
    }
}

void UIScene::onEnter(Game& game) {
    std::cout << "Entering Scene" << std::endl;
}

void UIScene::onExit(Game& game) {}

void UIScene::createButton(Game& game, const std::string& label, int x, int y, std::function<void()> onClick,SDL_Color textColor, SDL_Color bgColor, int padding) {
    SDL_Surface* textSurface = TTF_RenderText_Solid(font.font, label.c_str(), textColor);
    if (!textSurface) {
        std::cerr << "Failed to create surface: " << TTF_GetError() << std::endl;
        return;
    }

    int paddedWidth = textSurface->w + padding * 2;
    int paddedHeight = textSurface->h + padding * 2;

    if(x == -1) x = game.getWindowSize().wight / 2 - textSurface->w / 2;

    SDL_Surface* buttonSurface = SDL_CreateRGBSurfaceWithFormat(0, paddedWidth, paddedHeight, 32, SDL_PIXELFORMAT_RGBA32);
    SDL_FillRect(buttonSurface, nullptr, SDL_MapRGBA(buttonSurface->format, bgColor.r, bgColor.g, bgColor.b, bgColor.a));

    SDL_Rect textDest = { padding, padding, textSurface->w, textSurface->h };
    SDL_BlitSurface(textSurface, nullptr, buttonSurface, &textDest);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::renderer, buttonSurface);

    Button btn;
    btn.label = label;
    btn.texture = texture;
    btn.rect = { x, y, paddedWidth, paddedHeight };
    btn.onClick = std::move(onClick);
    btn.bgColor = bgColor;

    buttons.push_back(btn);

    SDL_FreeSurface(textSurface);
    SDL_FreeSurface(buttonSurface);
}

void UIScene::handleEvents(Game& game) {
    SDL_PollEvent(&Game::event);

    if (Game::event.type == SDL_MOUSEBUTTONDOWN) {
        int x = Game::event.button.x;
        int y = Game::event.button.y;
        handleClick(game, x, y);
    }
}

void UIScene::handleClick(Game& game, int x, int y) {
    for (const auto& btn : buttons) {
        if (x >= btn.rect.x && x <= btn.rect.x + btn.rect.w &&
            y >= btn.rect.y && y <= btn.rect.y + btn.rect.h) {

            if (btn.onClick) {
                btn.onClick();
            }
        }
    }
}

void UIScene::createTextArea(Game& game, const std::string& label,int x, int y){
    SDL_Color color = {230, 130, 140};
    SDL_Surface* surface = TTF_RenderText_Solid(font.font, label.c_str(), color);
    if (!surface) {
        std::cerr << "Failed to create surface: " << TTF_GetError() << std::endl;
        return;
    }

    if(x == -1) x = game.getWindowSize().wight / 2 - surface->w / 2;

    SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
    TextArea textArea;
    textArea.label = label;
    textArea.texture = texture;
    textArea.rect = { x, y, surface->w, surface->h };

    textAreas.push_back(textArea);
    SDL_FreeSurface(surface);
}

void UIScene::update(Game& game) {}

void UIScene::render(Game& game) {
    for (const auto& t : textAreas) {
        SDL_RenderCopy(Game::renderer, t.texture, nullptr, &t.rect);
    }
    for (const auto& btn : buttons) {
        SDL_SetRenderDrawColor(Game::renderer, btn.bgColor.r, btn.bgColor.g, btn.bgColor.b, btn.bgColor.a);
        SDL_RenderFillRect(Game::renderer, &btn.rect);

        SDL_RenderCopy(Game::renderer, btn.texture, nullptr, &btn.rect);
    }
}