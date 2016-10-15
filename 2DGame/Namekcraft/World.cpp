#include "World.h"

using namespace std;

World *World::createWorld(int sd, const glm::ivec2 &wSize, const glm::ivec2 &bSize, const glm::ivec2 &tSize, const glm::vec2 &minCoords, ShaderProgram &program) {
    World *world = new World(sd,wSize,bSize,tSize,minCoords,program);
    return world;
}

World::World(int sd, const glm::ivec2 &wSize, const glm::ivec2 &bSize, const glm::ivec2 &tSize, const glm::vec2 &minCoords, ShaderProgram &program) {
    prepareWorld(sd,wSize);
    prepareTexQuads(bSize,tSize,minCoords,program);
}

World::~World(){
    if(world != NULL) {
        delete world;
    }
}

void World::rec(int min, int max, int heightmin, int heightmax) {
    int mid = (max+min)/2;
    int y = (rand()%(heightmax))+heightmin;
    //assign block
    coords[y][mid] = 7;
    for(int i = 0; i < y; ++i) coords[i][mid] = 7;
    if(max-1 != min) {
        rec(mid,max,y,heightmax);
        rec(min,mid,heightmin,y);
    }
}

void World::prepareWorld(int sd, const glm::ivec2 &wSize) {

    worldSize = wSize;
    coords = vector<vector<int> > (worldSize.y,(vector<int>(worldSize.x,0)));
    tex.loadFromFile("images/rocks.jpg",TEXTURE_PIXEL_FORMAT_RGBA);
    srand(sd);
    int mid = worldSize.x/2;
    //minus 1 tile to evade y = Size.y, minimum 1
    int y = (rand()%(worldSize.y-1))+1;
    //assign block
    coords[y][mid] = 7;
    for(int i = 0; i < y; ++i) coords[i][mid] = 7;

    rec(mid,worldSize.x,(rand()%y)+1,(rand()%y)+1);
    rec(0,mid,(rand()%y)+1,(rand()%y)+1);

    //DEBUG
    /*for(int i = worldSize.y-1; i >= 0; --i) {
        for(int j = 0; j < worldSize.x; ++j) {
            cout << coords[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;*/
}

void World::prepareTexQuads(const glm::ivec2 &bSize, const glm::ivec2 &tSize, const glm::vec2 &minCoords, ShaderProgram &program) {
    blockSize = bSize;
    tilesheetSize = tSize;
    glm::vec2 tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);

    //geom -> position on screen
    glm::vec2 geom[2];
    //texCoords -> position on spreadsheet
    glm::vec2 texCoords[2];

    tilemap = vector<vector<TexturedQuad *> >(worldSize.y,vector<TexturedQuad *>(worldSize.x,NULL));

    for(int i = 0; i < worldSize.y; ++i) {
        for(int j = 0; j < worldSize.x; ++j) {
            int tile = coords[i][j];
            if(tile != 0) {
                //ALGO D'AQUI FALLA PROBABLEMENT
                geom[0] = glm::vec2(j*blockSize.x, (worldSize.y + ((worldSize.y-1) - i))*blockSize.y);
                //geom[0] = glm::vec2(j*blockSize.x,(minCoords.y - ((i+2)))*blockSize.y);
                geom[1] = glm::vec2(geom[0].x + blockSize.x, geom[0].y + blockSize.y);
                texCoords[0] = glm::vec2(float((tile-1)%8) / tilesheetSize.x, float((tile-1)/8) / tilesheetSize.y);
                texCoords[1] = glm::vec2(texCoords[0].x + tileTexSize.x, texCoords[0].y + tileTexSize.y);
                tilemap[i][j] = TexturedQuad::createTexturedQuad(geom,texCoords,program);
            }
        }
    }
}

void World::render(glm::ivec2 &pos, glm::ivec2 &screen) {

    int i = max(0,int(((worldSize.y-1) - pos.y) - (screen.y/2)-blockSize.y)/blockSize.y);
    int j = max(0,int(pos.x - (screen.x/2)-blockSize.x)/blockSize.x);
    int iend = min(worldSize.y-1,int(pos.y + (screen.y/2)+blockSize.y)/blockSize.y);
    int jend = min(worldSize.x-1,int(pos.x + (screen.x/2)+blockSize.x)/blockSize.x);

    for(int k = i; k<=iend; ++k) {
        for(int l = j; l<=jend; ++l) {
            if(tilemap[k][l] != NULL) {
                tilemap[k][l]->render(tex);
            }
        }
    }
}

void World::free() {
    for(int i = 0; i < worldSize.y; ++i) {
        for(int j = 0; j < worldSize.x; ++j) {
            if(tilemap[i][j] != NULL) tilemap[i][j]->free();
        }
    }
}

glm::ivec2 World::getWorldSize() {
    return worldSize;
}

glm::ivec2 World::getBlockSize() {
    return blockSize;
}

int World::position(int i, int j) {
    return coords[i][j];
}


bool World::collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
    /*int x, y0, y1;

    x = pos.x / blockSize.x;
    y0 = pos.y / blockSize.y;
    y1 = (pos.y + size.y - 1) / blockSize.y;
    for(int y=y0; y<=y1; y++)
    {
        //if(map[y*mapSize.x+x] != 0)
        if(coords[y][x] != 0)
            return true;
    }*/

    return false;
}

bool World::collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
    /*int x, y0, y1;

    x = (pos.x + size.x - 1) / blockSize.x;
    y0 = pos.y / blockSize.y;
    y1 = (pos.y + size.y - 1) / blockSize.y;
    for(int y=y0; y<=y1; y++)
    {
        //if(map[y*mapSize.x+x] != 0)
        if(coords[y][x] != 0)
            return true;
    }*/

    return false;
}

bool World::collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const
{
    /*int x0, x1, y;

    x0 = pos.x / blockSize.x;
    x1 = (pos.x + size.x - 1) / blockSize.x;
    y = (pos.y + size.y - 1) / blockSize.y;
    for(int x=x0; x<=x1; x++)
    {
        //if(map[y*mapSize.x+x] != 0)
        if(coords[y][x] != 0)
        {
            if(*posY - blockSize.y * y + size.y <= 4)
            {
                *posY = blockSize.y * y - size.y;
                return true;
            }
        }
    }*/

    return false;
}

