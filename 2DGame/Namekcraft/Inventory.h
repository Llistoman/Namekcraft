#ifndef _INVENTORY_INCLUDE
#define _INVENTORY_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "Text.h"


class Inventory
{

public:
  void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
  void update(int deltaTime);
  void render();
  void setTileMap(TileMap *tileMap);
  void setPosition(const glm::vec2 &pos);
  void incS(int x, int i);  
  void decS(int x, int i);
  bool enoughS(int x,int i);
  int getSelected();
  void setPico(int i);
  void setSword(int i);
    glm::ivec2 getPos();
    glm::ivec2 getSpSize();
  
private:
    glm::ivec2 tileMapDispl, posInventory, spriteSize;
    glm::vec2 spSize, spSize2;
  vector<glm::vec2> mapItems;
  int selected;
  int pico, sword;
  Texture spritesheet;
  Texture spritesheet2;
  vector<Sprite*> sprites;
  vector<Sprite*> inventoryItems;
  TileMap *map;
};


#endif // _INVENTORY_INCLUDE


