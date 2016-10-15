#include "World.h"

using namespace std;

World *World::createWorld(int sd, const glm::ivec2 &worldSize, const glm::ivec2 &blockSize, ShaderProgram &program) {
    World *world = new World(sd, worldSize, blockSize, program);
    return world;
}

World::World(int sd, const glm::ivec2 &worldSize, const glm::ivec2 &blockSize, ShaderProgram &program) {
    prepareWorld(sd,worldSize);
    prepareTexQuads(blockSize,program);
}

World::~World(){
    if(world != NULL)
        delete world;
}

void World::rec(int min, int max, int heightmin, int heightmax) {
    int mid = (max+min)/2;
    int y = (rand()%(heightmax))+heightmin;
    //assign block
    coords[y][mid] = 1;
    for(int i = 0; i < y; ++i) coords[i][mid] = 1;
    if(max-1 != min) {
        rec(mid,max,y,heightmax);
        rec(min,mid,heightmin,y);
    }
}

void World::prepareWorld(int sd, const glm::ivec2 &wSize) {

    worldSize = wSize;
    coords = vector<vector<int> > (worldSize.y,(vector<int>(worldSize.x,0)));
    srand(sd);
    int mid = worldSize.x/2;
    //minus 1 tile to evade y = Size.y, minimum 1
    int y = (rand()%(worldSize.y-1))+1;
    //assign block
    coords[y][mid] = 1;
    for(int i = 0; i < y; ++i) coords[i][mid] = 1;

    rec(mid,worldSize.x,(rand()%y)+1,(rand()%y)+1);
    rec(0,mid,(rand()%y)+1,(rand()%y)+1);

    //DEBUG
    /*for(int i = Size.y-1; i >= 0; --i) {
        for(int j = 0; j < Size.x; ++j) {
            cout << coords[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;*/
}

void World::prepareTexQuads(const glm::ivec2 &bSize, ShaderProgram &program) {
    blockSize = bSize;
    //tilemap = vector<vector<TexturedQuad> >
}

glm::ivec2 World::size() {
    return worldSize;
}

int World::position(int i, int j) {
    return coords[i][j];
}
