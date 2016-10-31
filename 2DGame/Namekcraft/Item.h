#ifndef _ITEM_INCLUDE
#define _ITEM_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Item is basically a Sprite that represents the item. As such it has


class Item
{

public:
  void init(const glm::ivec2 &tileMapPos, const glm::vec2 &spriteSize, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render(int dir);
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
    glm::ivec2 getPos();
    glm::ivec2 getSpSize();
	
private:
    glm::ivec2 tileMapDispl, posItem, spriteSize;
	int jumpAngle, startY;
  bool action;
  float itemSpeed;
  float currentTime;
	Texture spritesheet;
 	Sprite *sprite;
	TileMap *map;

};


#endif // _ITEM_INCLUDE


