#pragma once
#include "vector"

enum ItemType{
    NONE,
    ITEM,
    WEAPON
};

struct Item{
    const char* name;
    const char* imgPath;
    ItemType type;
};

class ItemsManager{
public:
    std::vector<Item> allItems;

    ItemsManager(){
        allItems.push_back(Item({"Carrot","assets/items/Carrot.png",ITEM}));
        allItems.push_back(Item({"Carrot2","assets/items/Carrot2.png",ITEM}));
    }

    Item getItemByIndex(int index){
        return allItems[index];
    }
    Item getItemByName(const char* name){
        for(Item i : allItems){
            if(strcmp(i.name, name) == 0){
                return i;
            }
        }
        return Item({nullptr, nullptr, NONE});
    }
};