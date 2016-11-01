#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE

#include <vector>
#include "Sprite.h"
#include "World.h"
#include "Item.h"
#include "Inventory.h"
#include "Craft.h"
#include "Text.h"
#include "Enemy.h"
#include "SoundManager.h"
#include "SceneOver.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.

class Enemy; //forward declaration

class Player
{

public:
    void init(const glm::ivec2 &position, const glm::vec2 &spriteSize, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
    void free();
    int dir;
    void setSoundManager(SoundManager *m);
    void setWorld(World *w);
    void setEnemies(vector<Enemy *> &vec);
	void setPosition(const glm::vec2 &pos);
    void heal(int h);
    void damage(int d);
    void dead();
    glm::ivec2 getPosRender();
    glm::ivec2 getSpSize();
    Inventory *inventory;
	
private:
    int hp;
    bool usat;
    bool death;
    bool bJumping;
    int pico, sword;
    glm::ivec2 posPlayer, spriteSize;
    int jumpAngle, startY;
    Item *item;
    time_t timer;

    Craft *craft;
    float playerSpeed;
	Texture spritesheet;
	Sprite *sprite;
    World *world;
    Text topText;

    vector<Enemy *> enemies;
    SoundManager *manager;

};


#endif // _PLAYER_INCLUDE


