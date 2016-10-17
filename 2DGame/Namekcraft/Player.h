#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
    void init(const glm::ivec2 &tileMapPos, const glm::vec2 &spriteSize, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
  int dir;
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
    glm::ivec2 getPos();
    glm::ivec2 getSpSize();
	
private:
    bool bJumping;
    glm::ivec2 tileMapDispl, posPlayer, spriteSize;
	int jumpAngle, startY;

  float playerSpeed;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;

};


#endif // _PLAYER_INCLUDE


