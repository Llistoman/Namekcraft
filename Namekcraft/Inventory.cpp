#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Inventory.h"
#include "Game.h"

#define INVENTORY_SIZE 10

#define INVENTORY_SIZE_X 36
#define INVENTORY_SIZE_Y 36

#define ITEM_INVENTORY_SIZE_X 24

#define MAX_STOCK 99

enum ItemAnims
{
  NORMAL, SELECTED
};

   vector<int> stocks (10,0);
   vector<Text> stocksText (10);
   vector<Text> itemsText (10);

   
void Inventory::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
    pico = sword =  0;   //Pico upgradejat 0
    selected = 0;
    spriteSize = glm::ivec2(INVENTORY_SIZE_X,INVENTORY_SIZE_Y);
    spSize = glm::vec2(INVENTORY_SIZE_X,INVENTORY_SIZE_Y);
    spSize2 = glm::vec2(ITEM_INVENTORY_SIZE_X,ITEM_INVENTORY_SIZE_X);
    spritesheet.loadFromFile("images/inventory_Block.png", TEXTURE_PIXEL_FORMAT_RGBA);
    spritesheet2.loadFromFile("images/itemsRelevant.png", TEXTURE_PIXEL_FORMAT_RGBA);
    //ALL OF THIS DEPENDS ON SPRITESHEET, MUST BE HARDCODED
    
    tileMapDispl = tileMapPos;

    glm::vec2 mapItems[10] = { 
     glm::vec2(0., 0.), //Pico
     glm::vec2(0.125, 0.), //Espasa
     glm::vec2(0.625, 0.), //Dirt
     glm::vec2(0.625, 0.25), //Roca
     glm::vec2(0.25, 0.), //Fusta
     glm::vec2(0.25, 0.25), //Namekita
     glm::vec2(0.25, 0.5), //Sulfato Cosmico
     glm::vec2(0.625, 0.5), //Limonita
     glm::vec2(0.375, 0.), //Potion
     glm::vec2(0.375, 0.5)};  //Mongetes Magiques
    
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
      
      //Stocks
      //Carga Fonts
      if(!stocksText[i].init("fonts/Andy")) cout << "Could not load font!!!" << endl;
      if(!itemsText[i].init("fonts/Andy")) cout << "Could not load font!!!" << endl;
      stocks[0] = 1;  //Inventari inicial
      stocks[1] = 1;
      //stocks[9] = 1;
      //stocks[8] = 1;
      
      //Sprites items
      
      inventoryItems.push_back(Sprite::createSprite(spSize2, glm::vec2(0.125, 0.25), &spritesheet2, &shaderProgram));
      inventoryItems[i]->setNumberAnimations(3);
      inventoryItems[i]->setAnimationSpeed(NORMAL, 8);
      inventoryItems[i]->addKeyframe(NORMAL, mapItems[i]);
      inventoryItems[i]->changeAnimation(NORMAL); 
    }
    
    //HARDCODE PER UPGRADEJAR ITEMS
    inventoryItems[0]->setAnimationSpeed(1, 8); //Pics
    inventoryItems[0]->addKeyframe(1, glm::vec2(0.f, 0.25f));
    inventoryItems[0]->setAnimationSpeed(2, 8);
    inventoryItems[0]->addKeyframe(2, glm::vec2(0.f, 0.5f));
    inventoryItems[1]->setAnimationSpeed(1, 8); //Espases
    inventoryItems[1]->addKeyframe(1, glm::vec2(0.125f, 0.25f));
    inventoryItems[1]->setAnimationSpeed(2, 8);
    inventoryItems[1]->addKeyframe(2, glm::vec2(0.125f, 0.5f));

}

void Inventory::update(int deltaTime)
{
  if(Game::instance().getKey('g'))//MODO DIOS tots els items a tope
  {
    for(int i = 0; i<INVENTORY_SIZE; ++i){
      stocks[i] = 50;
    }
  }
  
  if(Game::instance().getKey('0'))
  {
    sprites[INVENTORY_SIZE-1]->changeAnimation(SELECTED);
    selected = 0;
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
      selected = i;
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
    inventoryItems[i]->render(0);
  }
  
  for(int i = 0; i<INVENTORY_SIZE; ++i){ //En un mateix bucle hi havia bugs de renderitzat
    stocksText[i].render(to_string(stocks[i]), glm::vec2(float((INVENTORY_SIZE_X +2)*(1+i) -18), float(INVENTORY_SIZE_Y*1.3)), 14, glm::vec4(1, 1, 1, 1));
    if(i!=9)itemsText[i].render(to_string(i+1), glm::vec2(float((INVENTORY_SIZE_X +2)*(1+i)) +2, float(INVENTORY_SIZE_Y-6)), 14, glm::vec4(1, 1, 1, 1));
    else itemsText[i].render(to_string(0), glm::vec2(float((INVENTORY_SIZE_X +2)*(1+i)) +2, float(INVENTORY_SIZE_Y-6)), 14, glm::vec4(1, 1, 1, 1));
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
      inventoryItems[i]->setPosition(glm::vec2(float(posInventory.x + i * (INVENTORY_SIZE_X +2) +6), float(posInventory.y +2)+6));
      
    }
  
  
}

void Inventory::decS(int x, int i)
{
  if(i >= 0 and INVENTORY_SIZE > i){  //el indice debe estar dentro de rango
    stocks[i] -= x;
    if(0 > stocks[i]){ 
      stocks[i] = 0;
    }
  }
}

bool Inventory::enoughS(int x, int i)
{
  if(i >= 0 and INVENTORY_SIZE > i){  //retorna si tenim més de x del recurs i
    return stocks[i] >= x;
  }
}

int Inventory::getSelected()
{
  return selected;
}

void Inventory::incS(int x, int i)
{
  if(i >= 0 and INVENTORY_SIZE > i){//el indice debe estar dentro de rango
    stocks[i] += x;
    if(stocks[i] > MAX_STOCK) stocks[i] = MAX_STOCK;
  }
}

void Inventory::setPico(int i){
    pico = i;
    if(pico == 0) inventoryItems[0]->changeAnimation(pico); //UPGRADES
    else if(pico ==1) inventoryItems[0]->changeAnimation(pico);
    else if(pico ==2) inventoryItems[0]->changeAnimation(pico);
}

int Inventory::getPico(){
    return pico;
}


void Inventory::setSword(int i){
    sword = i;
    if(sword == 0) inventoryItems[1]->changeAnimation(sword); //UPGRADES
    else if(sword ==1) inventoryItems[1]->changeAnimation(sword);
    else if(sword ==2) inventoryItems[1]->changeAnimation(sword);
}

glm::ivec2 Inventory::getPos() {
    return posInventory;
}

glm::ivec2 Inventory::getSpSize() {
    return spriteSize;
}
