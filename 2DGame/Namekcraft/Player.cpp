#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT
};


void Player::init(const glm::ivec2 &position, const glm::vec2 &spSize, ShaderProgram &shaderProgram)
{
	bJumping = false;
    spriteSize = spSize;
    spritesheet.loadFromFile("images/bub.png", TEXTURE_PIXEL_FORMAT_RGBA);
    //ALL OF THIS DEPENDS ON SPRITESHEET, MUST BE HARDCODED

    sprite = Sprite::createSprite(spSize, glm::vec2(0.25, 0.25), &spritesheet, &shaderProgram);
    sprite->setNumberAnimations(4);
	
    sprite->setAnimationSpeed(STAND_LEFT, 8);
    sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.f));

    sprite->setAnimationSpeed(STAND_RIGHT, 8);
    sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.25f, 0.f));

    sprite->setAnimationSpeed(MOVE_LEFT, 8);
    sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.f));
    sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.25f));
    sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.5f));

    sprite->setAnimationSpeed(MOVE_RIGHT, 8);
    sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.f));
    sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.25f));
    sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.5f));
		
	sprite->changeAnimation(0);
    posPlayer = position;
    //tileMapDispl = tileMapPos;
    //sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
    sprite->setPosition(glm::vec2(posPlayer.x,posPlayer.y));
	
}

void Player::update(int deltaTime)
{
    sprite->update(deltaTime);
	if(Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	{
		if(sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);
        posPlayer.x -= 2;
        if(world->collisionMoveLeft(posPlayer, spriteSize))
		{
			posPlayer.x += 2;
			sprite->changeAnimation(STAND_LEFT);
        }
	}
	else if(Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
	{
		if(sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);
        posPlayer.x += 2;
        if(world->collisionMoveRight(posPlayer, spriteSize))
		{
			posPlayer.x -= 2;
			sprite->changeAnimation(STAND_RIGHT);
        }
	}
    //EXTRA DEBUG
    /*else if (Game::instance().getSpecialKey(GLUT_KEY_UP))
    {
        posPlayer.y -= 10;
    }
    else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN))
    {
        posPlayer.y += 10;
    }*/
	else
	{
		if(sprite->animation() == MOVE_LEFT)
			sprite->changeAnimation(STAND_LEFT);
		else if(sprite->animation() == MOVE_RIGHT)
			sprite->changeAnimation(STAND_RIGHT);
	}
	
    if(bJumping)
	{
        jumpAngle += JUMP_ANGLE_STEP;
		if(jumpAngle == 180)
		{
			bJumping = false;
			posPlayer.y = startY;
		}
		else
		{
            posPlayer.y = int(startY - (spriteSize.y*3) * sin(3.14159f * jumpAngle / 180.f));
            if(jumpAngle > 90)
                bJumping = !world->collisionMoveDown(posPlayer, spriteSize, &posPlayer.y);
        }
	}
	else
	{
        /*posPlayer.y += FALL_STEP;
        if(world->collisionMoveDown(posPlayer, spriteSize, &posPlayer.y))
		{
			if(Game::instance().getSpecialKey(GLUT_KEY_UP))
			{
				bJumping = true;
				jumpAngle = 0;
				startY = posPlayer.y;
            }
        }*/
	}
	
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setWorld(World *w)
{
    world = w;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

glm::ivec2 Player::getPos() {
    return posPlayer;
}

glm::ivec2 Player::getSpSize() {
    return spriteSize;
}




