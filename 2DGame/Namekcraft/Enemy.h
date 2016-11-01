#ifndef _ENEMY_INCLUDE
#define _ENEMY_INCLUDE

#include "Sprite.h"
#include "World.h"
#include "Item.h"
#include "Inventory.h"
#include "Player.h"
#include "Text.h"
#include "SoundManager.h"

class Player; //forward declaration

class Enemy
{

public:

    void init(Player *pl, int t, const glm::ivec2 &position, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
    void free();
    int dir;
    void setSoundManager(SoundManager *m);
    void setWorld(World *w);
	void setPosition(const glm::vec2 &pos);
    void damage(int d);
    void dead();
    bool isDead();
    glm::ivec2 getPosRender();
    glm::ivec2 getSpSize();
	
private:
    void patrol();
    void moveLeft();
    void moveRight();
    void jump();
    void run();
    void follow();

    void update1();
    void update2();

    time_t timer, hurtTim;
    int hp, pmin, pmax, type, fleeDist;
    bool death, bJumping, bpatrol, onPatrol;
    glm::ivec2 posEnemy, spriteSize;
	int jumpAngle, startY;
    float enemySpeed;
	Texture spritesheet;
	Sprite *sprite;
    World *world;
    Player *player;
    Text topText;

    SoundManager *manager;

};


#endif // _ENEMY_INCLUDE


