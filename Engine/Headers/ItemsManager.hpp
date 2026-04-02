#pragma once
#include "../../Engine/SDL2/include/SDL2/SDL.h"
#include "../Headers/TextureManager.hpp"
#include "../Headers/AttackProfile.hpp"

#include "vector"
#include "cstring"

enum ItemType{
    NONE,
    ITEM,
    WEAPON
};

class Item{
public:
    const char*          name;
    SDL_Texture*         texture;
    ItemType             type;
    AttackProfile*       attackProfile      = nullptr;  // pojedynczy cios
    WeaponComboProfile*  weaponComboProfile = nullptr;  // kombo broni (ma priorytet)

    Item() : name(nullptr), texture(nullptr), type(NONE) {}

    Item(const char* name,const char* texturePath,ItemType type):
        name(name), type(type)
    {
        if(strcmp(texturePath, "none") == 0){
            texture = nullptr;
        }else{
            texture = TextureManager::LoadTexture(texturePath);
        }
    }

    void clear(){
        SDL_DestroyTexture(texture);
    }
};

class ItemsManager{
public:
    std::vector<Item*> allItems;

    ItemsManager(){
        allItems.push_back(new Item("Carrot","assets/items/Carrot.png",ITEM));
        allItems.push_back(new Item("Carrot2","assets/items/Carrot2.png",ITEM));
        allItems.push_back(new Item("Axe","assets/items/axe.png",WEAPON));
    }

    Item* getItemByIndex(int index){
        return allItems[index];
    }

    Item* getItemByName(const char* name){
        for(Item* i : allItems){
            if(strcmp(i->name, name) == 0){
                return i;
            }
        }
        return new Item();
    }

    void clear(){
        for (Item* i : allItems) {
            i->clear();
            delete i;
        }
        allItems.clear();
    }
};