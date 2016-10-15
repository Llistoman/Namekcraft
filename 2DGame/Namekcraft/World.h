
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
    static World *createWorld(int sd, const glm::ivec2 &worldSize, const glm::ivec2 &blockSize, ShaderProgram &program);

    World(int sd, const glm::ivec2 &worldSize, const glm::ivec2 &blockSize, ShaderProgram &program);
    ~World();

    int position(int i, int j);
    glm::ivec2 size();

private:
    void rec(int min, int max, int heightmin, int heightmax);
    void prepareWorld(int sd, const glm::ivec2 &wSize);
    void prepareTexQuads(const glm::ivec2 &bSize, ShaderProgram &program);

private:

    World *world;
    glm::ivec2 blockSize;
    glm::ivec2 worldSize;
    vector<vector<int> > coords;
    vector<vector<TexturedQuad> > tilemap;

};


#endif // _WORLD_INCLUDE
