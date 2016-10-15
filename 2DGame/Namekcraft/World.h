
#ifndef _WORLD_INCLUDE
#define _WORLD_INCLUDE


#include <glm/glm.hpp>
#include <vector>
#include <stdlib.h>
#include <iostream>
#include "Texture.h"
#include "TexturedQuad.h"
#include "ShaderProgram.h"

class World
{

public:
    // Tile maps can only be created inside an OpenGL context
    static World *createWorld(int sd, const glm::ivec2 &wSize, const glm::ivec2 &bSize, const glm::ivec2 &tSize, const glm::vec2 &minCoords, ShaderProgram &program);

    World(int sd, const glm::ivec2 &wSize, const glm::ivec2 &bSize, const glm::ivec2 &tSize, const glm::vec2 &minCoords, ShaderProgram &program);
    ~World();

    void render(glm::ivec2 &pos,glm::ivec2 &screen);
    void free();

    int position(int i, int j);
    glm::ivec2 getWorldSize();
    glm::ivec2 getBlockSize();
    glm::ivec2 getTileSheetSize();

    bool collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const;
    bool collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const;
    bool collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const;
    bool collisionMoveUp(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const;

private:
    void rec(int min, int max, int heightmin, int heightmax);
    void prepareWorld(int sd, const glm::ivec2 &wSize);
    void prepareTexQuads(const glm::ivec2 &bSize, const glm::ivec2 &tSize, const glm::vec2 &minCoords, ShaderProgram &program);

private:

    World *world;
    glm::ivec2 blockSize;
    glm::ivec2 worldSize;
    glm::ivec2 tilesheetSize;
    vector<vector<int> > coords;
    vector<vector<TexturedQuad *> > tilemap;
    Texture tex;

};


#endif // _WORLD_INCLUDE
