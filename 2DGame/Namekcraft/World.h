
#ifndef _WORLD_INCLUDE
#define _WORLD_INCLUDE


#include <glm/glm.hpp>
#include <vector>
#include "Texture.h"
#include "SimplexNoise.h"
#include "ShaderProgram.h"

class World
{

public:
    // Tile maps can only be created inside an OpenGL context
    World *createWorld(int sd, const glm::vec2 &minCoords, ShaderProgram &program);

    World(int sd, const glm::vec2 &minCoords, ShaderProgram &program);
    ~World();

    void render() const;
    void free();

    int position(int i, int j);
    int getTileSize() const;

    bool collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const;
    bool collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const;
    bool collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const;


private:

    void prepareWorld(int sd, const glm::vec2 &minCoords);
    void prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program);

private:
    GLuint vao;
    GLuint vbo;
    GLint posLocation, texCoordLocation;
    glm::ivec2 position, worldSize, tilesheetSize;
    int tileSize, blockSize;
    Texture tilesheet;
    glm::vec2 tileTexSize;
    int *world;
    vector<vector<int> > coords;

};


#endif // _WORLD_INCLUDE
