#ifndef _INVENTORY_INCLUDE
#define _INVENTORY_INCLUDE

#include "Sprite.h"
#include "TileMap.h"

class Inventory
{

public:
  void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
  void update(int deltaTime);
  void render();
  void setTileMap(TileMap *tileMap);
  void setPosition(const glm::vec2 &pos);
    glm::ivec2 getPos();
    glm::ivec2 getSpSize();
  
private:
    glm::ivec2 tileMapDispl, posInventory, spriteSize;
    glm::vec2 spSize;

  Texture spritesheet;
  vector<Sprite*> sprites;
  TileMap *map;
};


#endif // _INVENTORY_INCLUDE


