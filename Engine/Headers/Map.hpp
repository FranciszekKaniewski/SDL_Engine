#pragma once
#include "string"
#include "./ECS.hpp"

class Map {
public:

    Map(const char* mfp,int ms,int ts, Manager& manager);
    ~Map();

    void LoadMap(std::string path,int sizeX,int sizeY);

private:
    const char* mapFilePath;
    int mapScale;
    int tileSize;
    void AddTile(int srcX,int srcY, int xpos, int ypos);
    Manager& manager;

};