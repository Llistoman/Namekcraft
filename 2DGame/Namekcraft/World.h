
#ifndef _WORLD_INCLUDE
#define _WORLD_INCLUDE


#include <glm/glm.hpp>
#include <vector>
#include <stdlib.h>
#include <iostream>
#include "SoundManager.h"
#include "Texture.h"
#include "TexturedQuad.h"
#include "ShaderProgram.h"
#include "SimplexNoise.h"
#include "Inventory.h"

class World
{

public:
    // Tile maps can only be created inside an OpenGL context
    static World *createWorld(int sd, const glm::ivec2 &wSize, const glm::ivec2 &bSize, const glm::ivec2 &tSize, int floorlvl, SoundManager *man, ShaderProgram &program);

    World(int sd, const glm::ivec2 &wSize, const glm::ivec2 &bSize, const glm::ivec2 &tSize, int floorlvl, SoundManager *man, ShaderProgram &program);
    ~World();

    void update(glm::ivec2 &pos,glm::ivec2 &screen, float time);
    void render(glm::ivec2 &pos,glm::ivec2 &screen);
    void free();

    int position(int i, int j);
    glm::ivec2 getWorldSize();
    glm::ivec2 getBlockSize();
    glm::ivec2 getTileSheetSize();

    bool collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const;
    bool collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const;
    bool collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const;
    bool collisionMoveUp(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const;
    void setInventory(Inventory *i);

private:
    void createBlock(int i, int j, int tile);
    void rec(int min, int max, int heightmin, int heightmax);
    void prepareWorld(int sd, const glm::ivec2 &wSize, int floorlvl, SoundManager *man);
    void prepareTexQuads(const glm::ivec2 &bSize, const glm::ivec2 &tSize, ShaderProgram &program);

private:

    mt19937 *generator;
    Inventory *inventory;
    glm::ivec2 blockSize;
    glm::ivec2 worldSize;
    glm::ivec2 tilesheetSize;
    int floor_level;
    vector<vector<int> > mat;
    float currentTime;
    vector<vector<float> > coords;
    vector<vector<TexturedQuad *> > tilemap;
    Texture tex;
    ShaderProgram shader;
    SoundManager *manager;

};


#endif // _WORLD_INCLUDE
