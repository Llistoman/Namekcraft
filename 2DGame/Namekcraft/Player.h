#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "World.h"
#include "Item.h"
#include "Inventory.h"
#include "Craft.h"
#include "Text.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
    void init(const glm::ivec2 &position, const glm::vec2 &spriteSize, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
    int dir;
    void setWorld(World *w);
	void setPosition(const glm::vec2 &pos);
    void heal(int h);
    void damage(int d);
    void dead();
    glm::ivec2 getPosRender();
    glm::ivec2 getSpSize();
    Inventory *inventory;
	
private:
    int hp;
    bool death;
    bool bJumping;
    glm::ivec2 posPlayer, spriteSize;
	int jumpAngle, startY;
    Item *item;

    Craft *craft;
    float playerSpeed;
	Texture spritesheet;
	Sprite *sprite;
    World *world;
    Text topText;

};


#endif // _PLAYER_INCLUDE


