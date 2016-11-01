#include <cmath>
#include <iostream>
#include <cstdio>
#include <ctime>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Enemy.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4

enum EnemyAnims
{
    ATK,MOVE
};


void Enemy::init(Player *pl, int t, const glm::ivec2 &position, ShaderProgram &shaderProgram)
{
    type = t;
    death = false;
    bpatrol = false;
    onPatrol = false;
    bJumping = false;

    //ALL OF THIS DEPENDS ON SPRITESHEET, MUST BE HARDCODED

    if(type == 0) {
        hp = 60;
        spriteSize.x = 42;
        spriteSize.y = 46;
        glm::vec2 spSize;
        spSize = spriteSize;
        spritesheet.loadFromFile("images/alien.png", TEXTURE_PIXEL_FORMAT_RGBA);
        sprite = Sprite::createSprite(spSize, glm::vec2(1, 0.25), &spritesheet, &shaderProgram);
        sprite->setNumberAnimations(2);

        sprite->setAnimationSpeed(MOVE, 6);
        sprite->addKeyframe(MOVE, glm::vec2(0.f, 0.f));
        sprite->addKeyframe(MOVE, glm::vec2(0.f, 0.25f));
        sprite->addKeyframe(MOVE, glm::vec2(0.f, 0.50f));

        //FER ALGO MILLOR
        sprite->setAnimationSpeed(ATK, 6);
        sprite->addKeyframe(ATK, glm::vec2(0.f, 0.f));
        sprite->addKeyframe(ATK, glm::vec2(0.f, 0.25f));
        sprite->addKeyframe(ATK, glm::vec2(0.f, 0.50f));

        sprite->changeAnimation(MOVE);
    }
    else {
        hp = 40;
        spriteSize.x = 56;
        spriteSize.y = 46;
        glm::vec2 spSize;
        spSize = spriteSize;
        spritesheet.loadFromFile("images/jelly.png", TEXTURE_PIXEL_FORMAT_RGBA);
        sprite = Sprite::createSprite(spSize, glm::vec2(1, 0.125), &spritesheet, &shaderProgram);
        sprite->setNumberAnimations(2);

        sprite->setAnimationSpeed(MOVE, 8);
        sprite->addKeyframe(MOVE, glm::vec2(0.f, 0.f));
        sprite->addKeyframe(MOVE, glm::vec2(0.f,0.125f));
        sprite->addKeyframe(MOVE, glm::vec2(0.f,2*0.125f));
        sprite->addKeyframe(MOVE, glm::vec2(0.f,3*0.125f));

        sprite->setAnimationSpeed(ATK, 10);
        sprite->addKeyframe(ATK, glm::vec2(0.f,4.f*0.125f));
        sprite->addKeyframe(ATK, glm::vec2(0.f,5.f*0.125f));
        sprite->addKeyframe(ATK, glm::vec2(0.f,6.f*0.125f));
        sprite->changeAnimation(MOVE);
    }

    posEnemy = position;
    sprite->setPosition(glm::vec2(posEnemy.x,posEnemy.y));

    //Player
    player = pl;
    timer = time(0);
}

void Enemy::moveLeft() {
    dir = 0;
    posEnemy.x -= enemySpeed;
    if(world->collisionMoveLeft(posEnemy, spriteSize))
    {
        bpatrol = !bpatrol;
        posEnemy.x += enemySpeed;
    }
}

void Enemy::moveRight() {
    dir = 1;
    posEnemy.x += enemySpeed;
    if(world->collisionMoveRight(posEnemy, spriteSize))
    {
        bpatrol = !bpatrol;
        posEnemy.x -= enemySpeed;
    }
}

void Enemy::jump() {
    if(bJumping)
    {
        jumpAngle += JUMP_ANGLE_STEP;
        if(jumpAngle == 180)
        {
            bJumping = false;
            posEnemy.y = startY;
        }
        else
        {
            posEnemy.y = int(startY - (spriteSize.y*3) * sin(3.14159f * jumpAngle / 180.f));
            if(jumpAngle > 90) {
                bJumping = !world->collisionMoveDown(posEnemy, spriteSize, &posEnemy.y);
            }
        }
    }
    else
    {
        posEnemy.y += FALL_STEP;
        if(world->collisionMoveDown(posEnemy, spriteSize, &posEnemy.y))
        {
            if(world->collisionMoveRight(posEnemy, spriteSize) or world->collisionMoveLeft(posEnemy, spriteSize))
            {
                bJumping = true;
                jumpAngle = 0;
                startY = posEnemy.y;
            }
        }
    }
}

void Enemy::patrol() {
    if(not bpatrol) {
        if(pmin < posEnemy.x) {
            moveLeft();
        }
        else bpatrol = true;
    }
    else {
        if(pmax > posEnemy.x) {
            moveRight();
        }
        else bpatrol = false;
    }
}

void Enemy::follow() {
    if(player->getPosRender().x > posEnemy.x) {
        posEnemy.x += enemySpeed;
    }
    else if(player->getPosRender().x <= posEnemy.x) {
        posEnemy.x -= enemySpeed;
    }
    if(player->getPosRender().y-2 > posEnemy.y) {
        posEnemy.y += enemySpeed;
    }
    else if(player->getPosRender().y+2 <= posEnemy.y) {
        posEnemy.y -= enemySpeed;
    }
}

void Enemy::update1() {
    enemySpeed = 2;
    glm::ivec2 playerPos = player->getPosRender();
    glm::ivec2 playerDist = glm::ivec2(abs(playerPos.x-posEnemy.x), abs(playerPos.y-posEnemy.y));

    //ATTACK
    if (playerDist.x <= 15 and playerDist.y <= 15) {
        onPatrol = false;
        if(sprite->animation() != ATK) sprite->changeAnimation(ATK);
        int duration = difftime(time(0),timer);
        if(duration >= 1) {
            player->damage(10);
            timer = time(0);
        }
    }
    else {
        if(playerDist.x < 100 and playerDist.y < 100 and player->getPosRender().x < posEnemy.x) { //LEFT
            if(sprite->animation() != MOVE) sprite->changeAnimation(MOVE);
            onPatrol = false;
            moveLeft();
        }
        else if(playerDist.x < 100 and playerDist.y < 100 and player->getPosRender().x >= posEnemy.x) { //RIGHT
            if(sprite->animation() != MOVE) sprite->changeAnimation(MOVE);
            onPatrol = false;
            moveRight();
        }
        else {
            if(not onPatrol) {
                if(sprite->animation() != MOVE) sprite->changeAnimation(MOVE);
                bpatrol = false;
                pmax = posEnemy.x + 100;
                pmin = posEnemy.x - 100;
                onPatrol = true;
            }
            patrol();
        }
    }
    jump();
}

void Enemy::update2() { //MAKE HIM RUN!
    enemySpeed = 1;
    glm::ivec2 playerPos = player->getPosRender();
    glm::ivec2 playerDist = glm::ivec2(abs(playerPos.x-posEnemy.x), abs(playerPos.y-posEnemy.y));

    if(playerPos.x > posEnemy.x) dir = 1;
    else dir = 0;

    if(playerDist.x <= 10 and playerDist.y <= 10) {
        if(sprite->animation() != ATK) sprite->changeAnimation(ATK);
        int duration = difftime(time(0),timer);
        if(duration >= 1) {
            player->damage(10);
            timer = time(0);
        }
    }
    else if(player->dir == dir and playerDist.x <= 300 and playerDist.y <= 300) {
        if(sprite->animation() != MOVE) sprite->changeAnimation(MOVE);
        follow();
    }
    else {
        if(sprite->animation() != MOVE) sprite->changeAnimation(MOVE);
    }
}

void Enemy::update(int deltaTime)
{
    if(0 >= hp) dead();
    sprite->update(deltaTime);

    if(!death){
        if(type == 0) update1();
        else update2();
        sprite->setPosition(glm::vec2(float(posEnemy.x), float(posEnemy.y)));
    }
}

void Enemy::render()
{
    sprite->render(dir);
}

void Enemy::free()
{
    sprite->free();
}

void Enemy::setSoundManager(SoundManager *m)
{
    manager = m;
}

void Enemy::setWorld(World *w)
{
    world = w;
}

void Enemy::setPosition(const glm::vec2 &pos)
{
    posEnemy = pos;
    sprite->setPosition(glm::vec2(float(posEnemy.x), float(posEnemy.y)));
}

void Enemy::damage(int d)
{
    hp -= d;
    if(0 > hp){ 
      hp = 0;
      dead();
    }
    else {
        if(type == 0) manager->playDmg1();
        else manager->playDmg2();
    }
}

void Enemy::dead()
{
  death = true;
  if(type == 0) manager->playDeath1();
  else manager->playDeath2();
    //DO SOMETHING
}

bool Enemy::isDead()
{
    return death;
}

glm::ivec2 Enemy::getPosRender() {
    return posEnemy;
}

glm::ivec2 Enemy::getSpSize() {
    return spriteSize;
}




