#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4

#define ITEM_DISTANCE 18

#define ITEM_SIZE_X 24
#define ITEM_SIZE_Y 24

#define INIT_ITEM_X_TILES 4
#define INIT_ITEM_Y_TILES 25

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

enum PlayerAnims
{
	STAND, MOVE, RUN, JUMP
};


void Player::init(const glm::ivec2 &position, const glm::vec2 &spSize, ShaderProgram &shaderProgram)
{
	bJumping = false;
    spriteSize = spSize;
    spritesheet.loadFromFile("images/character.png", TEXTURE_PIXEL_FORMAT_RGBA);
    //ALL OF THIS DEPENDS ON SPRITESHEET, MUST BE HARDCODED

    sprite = Sprite::createSprite(spSize, glm::vec2(0.25, 0.25), &spritesheet, &shaderProgram);
    sprite->setNumberAnimations(4);
	
    sprite->setAnimationSpeed(STAND, 8);
    sprite->addKeyframe(STAND, glm::vec2(0.f, 0.f));

    sprite->setAnimationSpeed(MOVE, 6);
    sprite->addKeyframe(MOVE, glm::vec2(0.f, 0.f));
    sprite->addKeyframe(MOVE, glm::vec2(0.25f, 0.f));
    sprite->addKeyframe(MOVE, glm::vec2(0.5f, 0.f));
    sprite->addKeyframe(MOVE, glm::vec2(0.75f, 0.f));

    sprite->setAnimationSpeed(RUN, 10);
    sprite->addKeyframe(RUN, glm::vec2(0.f, 0.f));
    sprite->addKeyframe(RUN, glm::vec2(0.25f, 0.f));
    sprite->addKeyframe(RUN, glm::vec2(0.5f, 0.f));
    sprite->addKeyframe(RUN, glm::vec2(0.75f, 0.f));
    
    sprite->setAnimationSpeed(JUMP, 8);
    sprite->addKeyframe(JUMP, glm::vec2(0.25f, 0.25f));


	sprite->changeAnimation(0);
    posPlayer = position;
    sprite->setPosition(glm::vec2(posPlayer.x,posPlayer.y));
    
  //Item
  item = new Item();
  item->init(position,glm::vec2(ITEM_SIZE_X ,ITEM_SIZE_Y), shaderProgram);
  item->setPosition(glm::vec2(INIT_ITEM_X_TILES * 32, INIT_ITEM_Y_TILES * 32));
   
  //Inventory
  inventory = new Inventory();
  inventory->init(position, shaderProgram);
  inventory->setPosition(glm::vec2(INIT_ITEM_X_TILES * 32, INIT_ITEM_Y_TILES * 32)); 
	
}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
  
  //RUN OR NOT
  if(Game::instance().getKey('z')){
    playerSpeed = 5;
  }
  else{
    playerSpeed = 2;
  }
  
  
	if(Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	{
    dir = 0;
      if(playerSpeed == 2){
        if(sprite->animation() != MOVE and sprite->animation() != JUMP) sprite->changeAnimation(MOVE);
      }
      else{
        if(sprite->animation() != RUN and sprite->animation() != JUMP) sprite->changeAnimation(RUN);
      }
		posPlayer.x -= playerSpeed;
        if(world->collisionMoveLeft(posPlayer, spriteSize))
		{
			posPlayer.x += playerSpeed;
			sprite->changeAnimation(STAND);
		}
	}
	else if(Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
  {
    dir = 1;
      if(playerSpeed == 2){
        if(sprite->animation() != MOVE and sprite->animation() != JUMP) sprite->changeAnimation(MOVE);
      }
      else{
        if(sprite->animation() != RUN and sprite->animation() != JUMP) sprite->changeAnimation(RUN);
      }
    posPlayer.x += playerSpeed;
        if(world->collisionMoveLeft(posPlayer, spriteSize))
    {
      posPlayer.x -= playerSpeed;
      sprite->changeAnimation(STAND);
    }
  }
	else
	{
		if(sprite->animation() == MOVE)
			sprite->changeAnimation(STAND);
		else if(sprite->animation() == MOVE)
			sprite->changeAnimation(STAND);
	}
	
	if(bJumping)
	{
		jumpAngle += JUMP_ANGLE_STEP;
		if(jumpAngle == 180)
		{
			bJumping = false;
			posPlayer.y = startY;
      sprite->changeAnimation(STAND);
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
		posPlayer.y += FALL_STEP;
        if(world->collisionMoveDown(posPlayer, spriteSize, &posPlayer.y))
          {
            if(Game::instance().getSpecialKey(GLUT_KEY_UP) or Game::instance().getKey(32))
            {
              bJumping = true;
                //if(sprite->animation() != JUMP) sprite->changeAnimation(JUMP);
              jumpAngle = 0;
              startY = posPlayer.y;
            }
          }
          if(sprite->animation() != JUMP and bJumping) sprite->changeAnimation(JUMP);
	}
	
    sprite->setPosition(glm::vec2(float(posPlayer.x), float(posPlayer.y)));
    
    //ITEM
    if(dir==0)item->setPosition(glm::vec2(float(posPlayer.x - ITEM_DISTANCE), float(posPlayer.y)));
    else item->setPosition(glm::vec2(float(posPlayer.x + ITEM_DISTANCE), float(posPlayer.y)));
    item->update(deltaTime);
    
    //INVENTORY
    inventory->setPosition(glm::vec2(float(posPlayer.x - SCREEN_WIDTH/2 + 48), float(posPlayer.y - SCREEN_HEIGHT/2 + 48)));
    inventory->update(deltaTime);
}

void Player::render()
{
	sprite->render(dir);
  item->render(dir);
  inventory->render();
  
}

void Player::setWorld(World *w)
{
    world = w;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
    sprite->setPosition(glm::vec2(float(posPlayer.x), float(posPlayer.y)));
}

glm::ivec2 Player::getPosRender() {
    return posPlayer;
}

glm::ivec2 Player::getSpSize() {
    return spriteSize;
}




