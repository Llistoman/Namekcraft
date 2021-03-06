#include "World.h"
#include "Game.h"

sf::SoundBuffer buffer;

using namespace std;

enum ItemAnims
{
  SENZU, PICO, SWORD, DIRT, ROCK, WOOD, NAMEKITA, COSMIC, LIMONITA, POTION, NONE //Ordre alterat
};

World *World::createWorld(int sd, const glm::ivec2 &wSize, const glm::ivec2 &bSize, const glm::ivec2 &tSize, int floorlvl, SoundManager *man, ShaderProgram &program) {
    World *world = new World(sd,wSize,bSize,tSize,floorlvl,man,program);
    return world;
}

World::World(int sd, const glm::ivec2 &wSize, const glm::ivec2 &bSize, const glm::ivec2 &tSize, int floorlvl, SoundManager *man, ShaderProgram &program) {
    prepareWorld(sd,wSize,floorlvl,man);
    prepareTexQuads(bSize,tSize,program);
}

World::~World(){}

void World::createBlock(int i, int j, int tile) {
    glm::vec2 tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);
    glm::vec2 halfTexel = glm::vec2(0.5f / tex.width(), 0.5f / tex.height());

    //geom -> position on screen
    glm::vec2 geom[2];
    //texCoords -> position on spreadsheet
    glm::vec2 texCoords[2];

    geom[0] = glm::vec2(j*blockSize.x, (worldSize.y - i - 1)*blockSize.y);
    geom[1] = glm::vec2(geom[0].x + blockSize.x, geom[0].y + blockSize.y);
    texCoords[0] = glm::vec2(float((tile-1)%8) / tilesheetSize.x, float((tile-1)/8) / tilesheetSize.y);
    texCoords[1] = texCoords[0] + tileTexSize;
    texCoords[1] -= halfTexel;
    tilemap[i][j] = TexturedQuad::createTexturedQuad(geom,texCoords,shader);
    mat[i][j] = tile;
}

void World::prepareWorld(int sd, const glm::ivec2 &wSize, int floorlvl,SoundManager *man) {
    worldSize = wSize;
    floor_level = floorlvl;
    manager = man;
    coords = vector<vector<float> > (worldSize.y,(vector<float>(worldSize.x,0)));
    tex.loadFromFile("images/itemsRelevant.png",TEXTURE_PIXEL_FORMAT_RGBA);
    tex.setWrapS(GL_CLAMP_TO_EDGE);
    tex.setWrapT(GL_CLAMP_TO_EDGE);
    tex.setMinFilter(GL_NEAREST);
    tex.setMagFilter(GL_NEAREST);
    generator = new mt19937;
    generator->seed(sd);
    SimplexNoise *simplex = new SimplexNoise(generator,100.f,0.f,1.0f);
    for(int i = 0; i < floor_level; ++i) {
        for(int j = 0; j < coords[0].size(); ++j) {
            float nx = float(i)/float(worldSize.x);
            float ny = float(j)/float(worldSize.y);
            coords[i][j] = simplex->noise(1000*nx,1000*ny);
        }
    }
}

void World::prepareTexQuads(const glm::ivec2 &bSize, const glm::ivec2 &tSize, ShaderProgram &program) {
    blockSize = bSize;
    tilesheetSize = tSize;
    shader = program;

    tilemap = vector<vector<TexturedQuad *> >(worldSize.y,vector<TexturedQuad *>(worldSize.x,NULL));
    mat = vector<vector<int> >(worldSize.y,vector<int>(worldSize.x,0));
    int tile;

    for(int i = 0; i < worldSize.y; ++i) {
        for(int j = 0; j < worldSize.x; ++j) {
            if (coords[i][j] < 0.4) tile = 0; //empty
            else if (coords[i][j] < 0.6 and coords[i][j] >= 0.4) tile = 8;
            else if (coords[i][j] < 0.8 and coords[i][j] >= 0.6) tile = 16;
            else if (coords[i][j] < 0.9 and coords[i][j] >= 0.8) tile = 24;
            else tile = 32;
            if(tile != 0) createBlock(i,j,tile);
        }
    }
}

void World::update(glm::ivec2 &pos,glm::ivec2 &screen, float time) {

    currentTime += time;
    pair<int,int> mousePos = Game::instance().getMousePos();
    glm::vec2 mPos = glm::vec2(pos.x- screen.x/2 + mousePos.first, pos.y- screen.y/2 + mousePos.second);
    int x0, x1, y0, y1;
    x0 = pos.x - 60;
    x1 = pos.x + 32;
    y0 = pos.y - 60;
    y1 = pos.y + 32;

    //THIS CREATES DIRT BLOCKS
    if(Game::instance().leftClick() and inventory->getSelected() == DIRT and inventory->enoughS(1,DIRT-1) and mPos.x >= x0 and mPos.x <= x1 and mPos.y >= y0 and mPos.y <= y1) {

        int i = (pos.y- screen.y/2 + mousePos.second)/blockSize.y + 1;
        int j = (pos.x- screen.x/2 + mousePos.first)/blockSize.x + 1;
        if (i >= 0 and i < worldSize.y and j >= 0 and j < worldSize.x and mat[(worldSize.y - i - 1)][j] == 0) {
            manager->playCreate();
            if(mat[(worldSize.y - i - 1)][j] == 0){
              inventory->decS(1,DIRT-1);
              createBlock((worldSize.y - i - 1),j,8);
            }
        }
    }
    
    //THIS CREATES ROCK BLOCKS
    if(Game::instance().leftClick() and inventory->getSelected() == ROCK and inventory->enoughS(1,ROCK-1) and mPos.x >= x0 and mPos.x <= x1 and mPos.y >= y0 and mPos.y <= y1) {

        int i = (pos.y- screen.y/2 + mousePos.second)/blockSize.y + 1;
        int j = (pos.x- screen.x/2 + mousePos.first)/blockSize.x + 1;
        if (i >= 0 and i < worldSize.y and j >= 0 and j < worldSize.x and mat[(worldSize.y - i - 1)][j] == 0) {
            manager->playCreate();
            if(mat[(worldSize.y - i - 1)][j] == 0){
              inventory->decS(1,ROCK-1);
              createBlock((worldSize.y - i - 1),j,16);
            }
        }
    }
    
    //THIS CREATES NAMEKITA BLOCKS
    if(Game::instance().leftClick() and inventory->getSelected() == NAMEKITA and inventory->enoughS(1,NAMEKITA-1) and mPos.x >= x0 and mPos.x <= x1 and mPos.y >= y0 and mPos.y <= y1) {

        int i = (pos.y- screen.y/2 + mousePos.second)/blockSize.y + 1;
        int j = (pos.x- screen.x/2 + mousePos.first)/blockSize.x + 1;
        if (i >= 0 and i < worldSize.y and j >= 0 and j < worldSize.x and mat[(worldSize.y - i - 1)][j] == 0) {
            manager->playCreate();
            if(mat[(worldSize.y - i - 1)][j] == 0){
              inventory->decS(1,NAMEKITA-1);
              createBlock((worldSize.y - i - 1),j,24);
            }
        }
    }
    
    
    //THIS CREATES COSMIC BLOCKS
    if(Game::instance().leftClick() and inventory->getSelected() == COSMIC and inventory->enoughS(1,COSMIC-1) and mPos.x >= x0 and mPos.x <= x1 and mPos.y >= y0 and mPos.y <= y1) {

        int i = (pos.y- screen.y/2 + mousePos.second)/blockSize.y + 1;
        int j = (pos.x- screen.x/2 + mousePos.first)/blockSize.x + 1;
        if (i >= 0 and i < worldSize.y and j >= 0 and j < worldSize.x and mat[(worldSize.y - i - 1)][j] == 0) {
            manager->playCreate();
            if(mat[(worldSize.y - i - 1)][j] == 0){
              inventory->decS(1,COSMIC-1);
              createBlock((worldSize.y - i - 1),j,32);
            }
        }
    }
    
    
    //THIS DESTROYS BLOCKS
    else if(Game::instance().leftClick() and inventory->getSelected() == PICO and mPos.x >= x0 and mPos.x <= x1 and mPos.y >= y0 and mPos.y <= y1) {

        int i = (pos.y- screen.y/2 + mousePos.second)/blockSize.y + 1;
        int j = (pos.x- screen.x/2 + mousePos.first)/blockSize.x + 1;
        if (i >= 0 and i < worldSize.y and j >= 0 and j < worldSize.x) {
            if(mat[(worldSize.y - i - 1)][j] != 0  and currentTime >= (500./(inventory->getPico()+1))) {
                currentTime = 0;
                if(mat[(worldSize.y - i - 1)][j] == 8)inventory->incS(1,DIRT-1);  //DIRT
                else if(mat[(worldSize.y - i - 1)][j] == 16)inventory->incS(1,ROCK-1); //ROCK
                else if(mat[(worldSize.y - i - 1)][j] == 24)inventory->incS(1,NAMEKITA-1); //NAMEKITA
                else if(mat[(worldSize.y - i - 1)][j] == 32)inventory->incS(1,COSMIC-1); //COSMIC
                manager->playDestroy();
                delete tilemap[(worldSize.y - i - 1)][j];
                tilemap[(worldSize.y - i - 1)][j] = NULL;
                mat[(worldSize.y - i - 1)][j] = 0;
            }
        }
    }
}

void World::render(glm::ivec2 &pos, glm::ivec2 &screen) {

    int i = max(0,int(((worldSize.y-1) - pos.y) - (screen.y/2)-blockSize.y)/blockSize.y);
    int j = max(0,int(pos.x - (screen.x/2)-blockSize.x)/blockSize.x);
    int iend = worldSize.y-1;
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
    for(int i = 0; i < tilemap.size(); ++i) {
        for(int j = 0; j < tilemap[0].size(); ++j) {
            if(tilemap[i][j] != NULL) tilemap[i][j]->free();
        }
    }
}

glm::ivec2 World::getShipPos() {
    return shipPos;
}

void World::setShipPos(glm::ivec2 &pos) {
    shipPos = pos;
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


void World::setInventory(Inventory *i)
{
    inventory = i;
}


//WARNING! KEEP IN MIND THAT COORD Y IS ALWAYS INVERTED

bool World::collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const
{	
    if(0. >= pos.x) return true;
    int x, y0, y1;
    
    x = pos.x / blockSize.x;
    y0 = pos.y / blockSize.y;
    y1 = (pos.y + size.y - 1) / blockSize.y;

    for(int y=y0; y<=y1; y++)
    {
        if(mat[worldSize.y -1 - y][x] != 0)
            return true;
    }

    return false;
}


bool World::collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
    if( pos.x >= 99*32) return true;
    int x, y0, y1;

    x = (pos.x + size.x - 1) / blockSize.x;
    y0 = pos.y / blockSize.y;
    y1 = (pos.y + size.y - 1) / blockSize.y;

    for(int y=y0; y<=y1; y++)
    {
        if(mat[worldSize.y -1 - y][x] != 0)
            return true;
    }

    return false;
}

bool World::collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const
{
    int x0, x1, y;

    x0 = pos.x / blockSize.x;
    x1 = (pos.x + size.x - 1) / blockSize.x;
    y = (pos.y + size.y - 1) / blockSize.y;

    for(int x=x0; x<=x1; x++)
    {
        if(mat[worldSize.y -1 - y][x] != 0)
        {
            if(*posY - blockSize.y * y + size.y <= 5)
            {
                *posY = blockSize.y * y - size.y;
                return true;
            }
        }
    }

    return false;
}

bool World::collisionMoveUp(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const
{
    int x0, x1, y;
    x0 = pos.x / blockSize.x;
    x1 = (pos.x + size.x) / blockSize.x;
    y = (pos.y + size.y - 1) / blockSize.y;

    for(int x=x0; x<=x1; x++)
    {
        if(mat[worldSize.y - y][x] != 0)
        {
            return true;
            if(*posY - blockSize.y * y + size.y > 5)
            {
                *posY = blockSize.y * y - size.y;
                return true;
            }
        }
    }

    return false;
}
