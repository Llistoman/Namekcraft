#include "World.h"

using namespace std;

World * World::createWorld(int sd, const glm::vec2 &minCoords, ShaderProgram &program) {
    World *world = new World(sd, minCoords, program);
    return world;
}

World::World(int sd, const glm::vec2 &minCoords, ShaderProgram &program) {
    prepareWorld(sd,minCoords);
    prepareArrays(minCoords,program);
}

World::~World(){
    if(world != NULL)
        delete world;
}

void World::render() const {
    glEnable(GL_TEXTURE_2D);
    tilesheet.use();
    glBindVertexArray(vao);
    glEnableVertexAttribArray(posLocation);
    glEnableVertexAttribArray(texCoordLocation);
    glDrawArrays(GL_TRIANGLES, 0, 6 * mapSize.x * mapSize.y);
    glDisable(GL_TEXTURE_2D);
}

void World::free() {
    glDeleteBuffers(1, &vbo);
}

void World::prepareWorld(int sd, const glm::vec2 &minCoords) {
    coords = vector<vector<int> > (minCoords.y,(vector<int>(minCoords.x,0)));
    for (int i = 0; i < minCoords.x; ++i) {
        float nx = i/minCoords.x -0.5f;
        int y = noise(nx);

    }

}

void World::prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program) {
    int tile, nTiles = 0;
    glm::vec2 posTile, texCoordTile[2], halfTexel;
    vector<float> vertices;

    halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
    for(int j=0; j<mapSize.y; j++)
    {
        for(int i=0; i<mapSize.x; i++)
        {
            tile = map[j * mapSize.x + i];
            if(tile != 0)
            {
                // Non-empty tile
                nTiles++;
                posTile = glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize);
                texCoordTile[0] = glm::vec2(float((tile-1)%2) / tilesheetSize.x, float((tile-1)/2) / tilesheetSize.y);
                texCoordTile[1] = texCoordTile[0] + tileTexSize;
                //texCoordTile[0] += halfTexel;
                texCoordTile[1] -= halfTexel;
                // First triangle
                vertices.push_back(posTile.x); vertices.push_back(posTile.y);
                vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
                vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y);
                vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
                vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
                vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
                // Second triangle
                vertices.push_back(posTile.x); vertices.push_back(posTile.y);
                vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
                vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
                vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
                vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize);
                vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
            }
        }
    }

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
    posLocation = program.bindVertexAttribute("position", 2, 4*sizeof(float), 0);
    texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4*sizeof(float), (void *)(2*sizeof(float)));
}

int World::position(int i, int j) {
    return coords[i][j];
}

int World::getTileSize() const {
    return tileSize;
}

bool World::collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
    int x, y0, y1;

    x = pos.x / tileSize;
    y0 = pos.y / tileSize;
    y1 = (pos.y + size.y - 1) / tileSize;
    for(int y=y0; y<=y1; y++)
    {
        if(map[y*mapSize.x+x] != 0)
            return true;
    }

    return false;
}

bool World::collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
    int x, y0, y1;

    x = (pos.x + size.x - 1) / tileSize;
    y0 = pos.y / tileSize;
    y1 = (pos.y + size.y - 1) / tileSize;
    for(int y=y0; y<=y1; y++)
    {
        if(map[y*mapSize.x+x] != 0)
            return true;
    }

    return false;
}

bool World::collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const
{
    int x0, x1, y;

    x0 = pos.x / tileSize;
    x1 = (pos.x + size.x - 1) / tileSize;
    y = (pos.y + size.y - 1) / tileSize;
    for(int x=x0; x<=x1; x++)
    {
        if(map[y*mapSize.x+x] != 0)
        {
            if(*posY - tileSize * y + size.y <= 4)
            {
                *posY = tileSize * y - size.y;
                return true;
            }
        }
    }

    return false;
}

/*bool World::collisionMoveTop(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const
{

}*/


