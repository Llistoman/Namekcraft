#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Item.h"
#include "Game.h"


enum ItemAnims
{
	PICO, SWORD, DIRT, ROCK, NONE
};


void Item::init(const glm::ivec2 &tileMapPos, const glm::vec2 &spSize, ShaderProgram &shaderProgram)
{
    spriteSize = spSize;
    spritesheet.loadFromFile("images/itemsRelevant.png", TEXTURE_PIXEL_FORMAT_RGBA);
    //ALL OF THIS DEPENDS ON SPRITESHEET, MUST BE HARDCODED

    sprite = Sprite::createSprite(spSize, glm::vec2(0.125, 0.25), &spritesheet, &shaderProgram);
    sprite->setNumberAnimations(5);
	
    sprite->setAnimationSpeed(PICO, 8);
    sprite->addKeyframe(PICO, glm::vec2(0.f, 0.f));

    sprite->setAnimationSpeed(SWORD, 8);
    sprite->addKeyframe(SWORD, glm::vec2(0.125f, 0.f));
    
    sprite->setAnimationSpeed(DIRT, 8);
    sprite->addKeyframe(DIRT, glm::vec2(0.625f, 0.f));
    
    sprite->setAnimationSpeed(ROCK, 8);
    sprite->addKeyframe(ROCK, glm::vec2(0.625f, 0.25f));
    
    sprite->setAnimationSpeed(NONE, 8);
    sprite->addKeyframe(NONE, glm::vec2(0.f, 0.75f));



	sprite->changeAnimation(NONE);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posItem.x), float(tileMapDispl.y + posItem.y)));
}

void Item::update(int deltaTime)
{
  if(Game::instance().getKey('1'))
  {
    sprite->changeAnimation(PICO);
  }
  else if(Game::instance().getKey('2'))
  {
    sprite->changeAnimation(SWORD);
  }
    else if(Game::instance().getKey('3'))
  {
    sprite->changeAnimation(DIRT);
  }
    else if(Game::instance().getKey('4'))
  {
    sprite->changeAnimation(ROCK);
  }
    else if(Game::instance().getKey('0'))
  {
    sprite->changeAnimation(NONE);
  }
  
	sprite->update(deltaTime);
}

void Item::render(int dir)
{
	sprite->render((dir+1)%2); //Direccio va de 0 a 1, vui que sigui la inversa del player
}

void Item::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Item::setPosition(const glm::vec2 &pos)
{
	posItem = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posItem.x), float(tileMapDispl.y + posItem.y)));
}

glm::ivec2 Item::getPos() {
    return posItem;
}

glm::ivec2 Item::getSpSize() {
    return spriteSize;
}




