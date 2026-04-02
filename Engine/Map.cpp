#include "fstream"

#include "./Headers/Map.hpp"
#include "./ECS_Components/Components.hpp"

Map::Map(const char* mfp,int ms,int ts,Manager& manager) : mapFilePath(mfp), mapScale(ms),tileSize(ts), manager(manager){}

Map::~Map(){}

void Map::LoadMap(std::string path,int sizeX,int sizeY) {
    char c;
    std::fstream mapFile;
    mapFile.open(path);

    int srcX, srcY;

    for(int y =0;y<sizeY;y++){
        for(int x =0;x<sizeX;x++){

            mapFile.get(c);
            srcY = atoi(&c) * tileSize;
            mapFile.get(c);
            srcX = atoi(&c) * tileSize;
            AddTile(srcX,srcY,x * (tileSize*mapScale),y * (tileSize*mapScale));
            mapFile.ignore();
        }
    }
    mapFile.ignore();

    for(int y=0;y<sizeY;y++){
        for(int x=0;x<sizeX;x++){
            mapFile.get(c);
            if(c == '1'){
                auto& tcol(manager.addEntity());
                tcol.addComponent<ColliderComponent>("terrain",x*(tileSize*mapScale),y*(tileSize*mapScale),tileSize*mapScale);
                tcol.addGroup(Game::groupColliders);
            }else if(c == '2'){
                auto& tree(manager.addEntity());
                tree.addComponent<TransformComponent>(x*(tileSize*mapScale),y*(tileSize*mapScale),128,64,1);
                tree.addComponent<SpriteComponent>("assets/objects/tree.png");
                tree.addComponent<ColliderComponent>("tree",0,64,tileSize*mapScale);
                tree.addGroup(Game::groupColliders);
            }
            mapFile.ignore();
        }
    }

    mapFile.close();
}

void Map::AddTile(int srcX, int srcY, int xpos, int ypos) {
    auto& tile(manager.addEntity());
    tile.addComponent<TileComponent>(srcX,srcY,xpos,ypos,tileSize,mapScale,mapFilePath);
    tile.addGroup(Game::groupMap);
}