#ifndef _CRAFT_INCLUDE
#define _CRAFT_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "Text.h"


class Craft
{

public:
  void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
  void update(int deltaTime);
  void render();
  void renderTexts();
  void setTileMap(TileMap *tileMap);
  void setPosition(const glm::vec2 &pos);
  void decC0(int x);
  void decC1(int x);
  void incC0(int x);
  void incC1(int x);
    glm::ivec2 getPos();
    glm::ivec2 getSpSize();
  
private:
    glm::ivec2 tileMapDispl, posCraft, spriteSize;
    glm::vec2 spSize, spSize2;
  bool craftPosition;
  bool sumat;
  vector<glm::vec2> mapItems;
  Texture spritesheet;
  Texture spritesheet2;
  vector<Sprite*> sprites;
  vector<Sprite*> craftItems;
  TileMap *map;
  Text craftText;
};


#endif // _CRAFT_INCLUDE


