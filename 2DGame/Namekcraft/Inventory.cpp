#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Inventory.h"
#include "Game.h"


#define INVENTORY_SIZE 10

#define INVENTORY_SIZE_X 36
#define INVENTORY_SIZE_Y 36

enum ItemAnims
{
  NORMAL, SELECTED
};



void Inventory::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
    spriteSize = glm::ivec2(INVENTORY_SIZE_X,INVENTORY_SIZE_Y);
    spSize = glm::vec2(INVENTORY_SIZE_X,INVENTORY_SIZE_Y);
    spritesheet.loadFromFile("images/inventory_Block.png", TEXTURE_PIXEL_FORMAT_RGBA);
    //ALL OF THIS DEPENDS ON SPRITESHEET, MUST BE HARDCODED
    
    tileMapDispl = tileMapPos;

    for(int i = 0; i<INVENTORY_SIZE; ++i){
      sprites.push_back(Sprite::createSprite(spSize, glm::vec2(0.25, 1.), &spritesheet, &shaderProgram));
      //sprites[i] = Sprite::createSprite(spSize, glm::vec2(1., 1.), &spritesheet, &shaderProgram);
      sprites[i]->setNumberAnimations(2);
      
      sprites[i]->setAnimationSpeed(NORMAL, 8);
      sprites[i]->addKeyframe(NORMAL, glm::vec2(0.f, 0.f));
      
      sprites[i]->setAnimationSpeed(SELECTED, 8);
      sprites[i]->addKeyframe(SELECTED, glm::vec2(0.25f, 0.f));
      
      sprites[i]->changeAnimation(NORMAL);
      sprites[i]->setPosition(glm::vec2(float(tileMapDispl.x + posInventory.x), float(tileMapDispl.y + posInventory.y)));
    }

}

void Inventory::update(int deltaTime)
{
  if(Game::instance().getKey('0'))
  {
    sprites[INVENTORY_SIZE-1]->changeAnimation(SELECTED);
  }
  else if ((Game::instance().getKey('0') or
            Game::instance().getKey('1') or
            Game::instance().getKey('2') or
            Game::instance().getKey('3') or
            Game::instance().getKey('4') or
            Game::instance().getKey('5') or
            Game::instance().getKey('6') or
            Game::instance().getKey('7') or
            Game::instance().getKey('8') or 
            Game::instance().getKey('9')) and sprites[INVENTORY_SIZE-1]->animation() != NORMAL)
  {
    sprites[INVENTORY_SIZE-1]->changeAnimation(NORMAL);
  }
  
  sprites[INVENTORY_SIZE-1]->update(deltaTime);


  for(int i = 1; i<INVENTORY_SIZE; ++i){
    if(Game::instance().getKey(i + '0'))
    {
      sprites[i-1]->changeAnimation(SELECTED);
    }
    else if ((Game::instance().getKey('0') or
              Game::instance().getKey('1') or
              Game::instance().getKey('2') or
              Game::instance().getKey('3') or
              Game::instance().getKey('4') or
              Game::instance().getKey('5') or
              Game::instance().getKey('6') or
              Game::instance().getKey('7') or
              Game::instance().getKey('8') or 
              Game::instance().getKey('9')) and sprites[i-1]->animation() != NORMAL)
    {
      sprites[i-1]->changeAnimation(NORMAL);
    }
    
    sprites[i]->update(deltaTime);
  }
}

void Inventory::render()
{
  for(int i = 0; i<INVENTORY_SIZE; ++i){
    sprites[i]->render(0);
  }
}

void Inventory::setTileMap(TileMap *tileMap)
{
  map = tileMap;
}

void Inventory::setPosition(const glm::vec2 &pos)
{
  posInventory = pos;
    for(int i = 0; i<INVENTORY_SIZE; ++i){
      sprites[i]->setPosition(glm::vec2(float(posInventory.x + i * (INVENTORY_SIZE_X +2)), float(posInventory.y)));
    }
  
  
}

glm::ivec2 Inventory::getPos() {
    return posInventory;
}

glm::ivec2 Inventory::getSpSize() {
    return spriteSize;
}




