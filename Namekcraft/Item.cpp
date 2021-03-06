#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Item.h"
#include "Game.h"

enum ItemAnims
{
  PICO, SWORD, DIRT, ROCK, WOOD, NAMEKITA, COSMIC, LIMONITA, POTION, SENZU,  NONE, NAMEKPICO, COSMICPICO, NAMEKSWORD, COSMICSWORD
};

void Item::init(const glm::ivec2 &tileMapPos, const glm::vec2 &spSize, ShaderProgram &shaderProgram)
{
    pico = sword =  0;   //Pico upgradejat 0
    spriteSize = spSize;
    action = false;
    spritesheet.loadFromFile("images/itemsRelevant.png", TEXTURE_PIXEL_FORMAT_RGBA);
    //ALL OF THIS DEPENDS ON SPRITESHEET, MUST BE HARDCODED

    sprite = Sprite::createSprite(spSize, glm::vec2(0.125, 0.25), &spritesheet, &shaderProgram);
    sprite->setNumberAnimations(15);
	
    sprite->setAnimationSpeed(PICO, 8);
    sprite->addKeyframe(PICO, glm::vec2(0.f, 0.f));

    sprite->setAnimationSpeed(SWORD, 8);
    sprite->addKeyframe(SWORD, glm::vec2(0.125f, 0.f));
    
    sprite->setAnimationSpeed(DIRT, 8);
    sprite->addKeyframe(DIRT, glm::vec2(0.625f, 0.f));
    
    sprite->setAnimationSpeed(ROCK, 8);
    sprite->addKeyframe(ROCK, glm::vec2(0.625f, 0.25f));
    
    sprite->setAnimationSpeed(WOOD, 8);
    sprite->addKeyframe(WOOD, glm::vec2(0.25, 0.));
    
    sprite->setAnimationSpeed(NAMEKITA, 8);
    sprite->addKeyframe(NAMEKITA, glm::vec2(0.25, 0.25));
    
    sprite->setAnimationSpeed(COSMIC, 8);
    sprite->addKeyframe(COSMIC, glm::vec2(0.25, 0.5));
    
    sprite->setAnimationSpeed(LIMONITA, 8);
    sprite->addKeyframe(LIMONITA, glm::vec2(0.625, 0.5));
    
    sprite->setAnimationSpeed(POTION, 8);
    sprite->addKeyframe(POTION, glm::vec2(0.375, 0.));
    
    sprite->setAnimationSpeed(SENZU, 8);
    sprite->addKeyframe(SENZU, glm::vec2(0.375, 0.5));
    
    sprite->setAnimationSpeed(NONE, 8);
    sprite->addKeyframe(NONE, glm::vec2(0.f, 0.75f));
    
    sprite->setAnimationSpeed(NAMEKPICO, 8);
    sprite->addKeyframe(NAMEKPICO, glm::vec2(0.f, 0.25f));
    
    sprite->setAnimationSpeed(COSMICPICO, 8); 
    sprite->addKeyframe(COSMICPICO, glm::vec2(0.f, 0.5f));
    
    sprite->setAnimationSpeed(NAMEKSWORD, 8);
    sprite->addKeyframe(NAMEKSWORD, glm::vec2(0.125f, 0.25f));
    
    sprite->setAnimationSpeed(COSMICSWORD, 8); 
    sprite->addKeyframe(COSMICSWORD, glm::vec2(0.125f, 0.5f));



	sprite->changeAnimation(NONE);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posItem.x), float(tileMapDispl.y + posItem.y)));
}

void Item::update(int deltaTime)
{
  currentTime += deltaTime;
  if(Game::instance().getKey('1'))
  {
    if(pico ==0) sprite->changeAnimation(PICO);
    else if(pico ==1) sprite->changeAnimation(NAMEKPICO);
    else if(pico ==2) sprite->changeAnimation(COSMICPICO);
  }
  else if(Game::instance().getKey('2'))
  {
    if(sword == 0) sprite->changeAnimation(SWORD);
    else if(sword ==1) sprite->changeAnimation(NAMEKSWORD);
    else if(sword ==2) sprite->changeAnimation(COSMICSWORD);
  }
    else if(Game::instance().getKey('3'))
  {
    sprite->changeAnimation(DIRT);
  }
    else if(Game::instance().getKey('4'))
  {
    sprite->changeAnimation(ROCK);
  }
    else if(Game::instance().getKey('5'))
  {
    sprite->changeAnimation(WOOD);
  }
    else if(Game::instance().getKey('6'))
  {
    sprite->changeAnimation(NAMEKITA);
  }
    else if(Game::instance().getKey('7'))
  {
    sprite->changeAnimation(COSMIC);
  }
    else if(Game::instance().getKey('8'))
  {
    sprite->changeAnimation(LIMONITA);
  }
    else if(Game::instance().getKey('9'))
  {
    sprite->changeAnimation(POTION);
  }
    else if(Game::instance().getKey('0'))
  {
    sprite->changeAnimation(SENZU);
  }
  
  if(Game::instance().leftClick()) {
    action = true;
  }
  else{
    action = false;
  }
  
	sprite->update(deltaTime);
}

void Item::render(int dir)
{
  if(action){
    float freq = currentTime;
    if(sprite->animation() == PICO or sprite->animation() == NAMEKPICO or sprite->animation() == COSMICPICO) freq *= (pico+1);
    else if(sprite->animation() == SWORD or sprite->animation() == NAMEKSWORD or sprite->animation() == COSMICSWORD) freq *= (sword+1);
    float value = (sin(freq / 100.f) + 1.0f) * 20.0f;
    sprite->render((dir+1)%2, value);
  }
    else sprite->render((dir+1)%2);
}

void Item::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Item::setPosition(const glm::vec2 &pos)
{
	posItem = pos;
	sprite->setPosition(glm::vec2(float(posItem.x), float(posItem.y)));
}

void Item::setPico(int i){
    pico = i;
}

void Item::setSword(int i){
    sword = i;
}

glm::ivec2 Item::getPos() {
    return posItem;
}

glm::ivec2 Item::getSpSize() {
    return spriteSize;
}
