#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Craft.h"
#include "Game.h"

#define MAX_STOCK 99
#define CRAFT_SIZE 3

#define CRAFT_SIZE_X 36
#define CRAFT_SIZE_Y 36

#define ITEM_CRAFT_SIZE_X 24

enum ItemAnims
{
  NORMAL, SELECTED
};

   vector<int> qu (3,0);
   vector<Text> stocksCraftText (3);

   
void Craft::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
    craftPosition = false;  //Primera posicio de craft
    sumat = false;  //Per controlar la pujada dels contadors
    spriteSize = glm::ivec2(CRAFT_SIZE_X,CRAFT_SIZE_Y);
    spSize = glm::vec2(CRAFT_SIZE_X,CRAFT_SIZE_Y);
    spSize2 = glm::vec2(ITEM_CRAFT_SIZE_X,ITEM_CRAFT_SIZE_X);
    spritesheet.loadFromFile("images/inventory_Block.png", TEXTURE_PIXEL_FORMAT_RGBA);
    spritesheet2.loadFromFile("images/itemsRelevant.png", TEXTURE_PIXEL_FORMAT_RGBA);
    //ALL OF THIS DEPENDS ON SPRITESHEET, MUST BE HARDCODED
    
    tileMapDispl = tileMapPos;

//     glm::vec2 mapItems[10] = { 
//      glm::vec2(0., 0.), //Pico
//      glm::vec2(0.125, 0.), //Espasa
//      glm::vec2(0.625, 0.), //Dirt
//      glm::vec2(0.625, 0.25), //Roca
//      glm::vec2(0.25, 0.), //Fusta
//      glm::vec2(0.25, 0.25), //Namekita
//      glm::vec2(0.25, 0.5), //Sulfato Cosmico
//      glm::vec2(0.625, 0.5), //Limonita
//      glm::vec2(0.375, 0.), //Potion
//      glm::vec2(0.375, 0.5)};  //Mongetes Magiques
    
    for(int i = 0; i<CRAFT_SIZE; ++i){
      sprites.push_back(Sprite::createSprite(spSize, glm::vec2(0.25, 1.), &spritesheet, &shaderProgram));
      //sprites[i] = Sprite::createSprite(spSize, glm::vec2(1., 1.), &spritesheet, &shaderProgram);
      sprites[i]->setNumberAnimations(2);
      
      sprites[i]->setAnimationSpeed(NORMAL, 8);
      sprites[i]->addKeyframe(NORMAL, glm::vec2(0.f, 0.f));
      
      sprites[i]->setAnimationSpeed(SELECTED, 8);
      sprites[i]->addKeyframe(SELECTED, glm::vec2(0.50f, 0.f));
      
      sprites[i]->changeAnimation(NORMAL);
      sprites[i]->setPosition(glm::vec2(float(tileMapDispl.x + posCraft.x), float(tileMapDispl.y + posCraft.y)));
      
      //Stocks
      //Carga Fonts
      if(!craftText.init("fonts/Andy")) cout << "Could not load font!!!" << endl;
      if(!stocksCraftText[i].init("fonts/Andy")) cout << "Could not load font!!!" << endl;
      qu[0] = 0;  //Inventari inicial
      qu[1] = 0;
      
      //Sprites items
      
      craftItems.push_back(Sprite::createSprite(spSize2, glm::vec2(0.125, 0.25), &spritesheet2, &shaderProgram));
      craftItems[i]->setNumberAnimations(1);
      craftItems[i]->setAnimationSpeed(NORMAL, 8);
      craftItems[i]->addKeyframe(NORMAL, glm::vec2(0.0, 0.75));
      craftItems[i]->changeAnimation(NORMAL); 
    }
    
    sprites.push_back(Sprite::createSprite(spSize, glm::vec2(0.25, 1.), &spritesheet, &shaderProgram));
    sprites[CRAFT_SIZE -1]->setNumberAnimations(2);   
    sprites[CRAFT_SIZE -1]->setAnimationSpeed(NORMAL, 8);
    sprites[CRAFT_SIZE -1]->addKeyframe(NORMAL, glm::vec2(0.75f, 0.f));   
    sprites[CRAFT_SIZE -1]->setAnimationSpeed(SELECTED, 8);
    sprites[CRAFT_SIZE -1]->addKeyframe(SELECTED, glm::vec2(0.25f, 0.f));   
    sprites[CRAFT_SIZE -1]->changeAnimation(NORMAL);
    sprites[CRAFT_SIZE -1]->setPosition(glm::vec2(float(tileMapDispl.x + posCraft.x), float(tileMapDispl.y + posCraft.y)));

}

void Craft::update(int deltaTime)
{
  if(Game::instance().getKey('a'))
  {
    craftPosition = false;
  }
  else if(Game::instance().getKey('d'))
  {
    craftPosition = true;
  }
  
  if(Game::instance().getKey('w') and !craftPosition and !sumat)
  {
    incC0(1);
    sumat = true;
  }
  else  if(Game::instance().getKey('s') and !craftPosition and !sumat)
  {
    decC0(1);
    sumat = true;
  }
  if(Game::instance().getKey('w') and craftPosition and !sumat)
  {
    incC1(1);
    sumat = true;
  }
  else  if(Game::instance().getKey('s') and craftPosition and !sumat)
  {
    decC1(1);
    sumat = true;
  }
  
  if(sumat and !Game::instance().getKey('w') and !Game::instance().getKey('s')){ //tornem a permetres sumar o decrementar contadors
    sumat = false;
  }
  
  
  if(craftPosition){
    sprites[1]->changeAnimation(SELECTED);
    sprites[0]->changeAnimation(NORMAL);
  }
  else {
    sprites[0]->changeAnimation(SELECTED);
    sprites[1]->changeAnimation(NORMAL);
  }

    sprites[0]->update(deltaTime);
    sprites[1]->update(deltaTime);
}

void Craft::render()
{
  for(int i = 0; i<CRAFT_SIZE; ++i){
    sprites[i]->render(0);
    craftItems[i]->render(0);
  }
  for(int i = 0; i<CRAFT_SIZE-1; ++i){ //En un mateix bucle hi havia bugs de renderitzat
    stocksCraftText[i].render(to_string(qu[i]), glm::vec2(float((CRAFT_SIZE_X +2)*(1+i) -18), float(CRAFT_SIZE_Y*1.9 + (CRAFT_SIZE_Y +2))), 14, glm::vec4(1, 1, 1, 1));
  }
  craftText.render("Crafting W,A,S,D", glm::vec2(float((CRAFT_SIZE_X +2) -18), float(CRAFT_SIZE_Y*1.9f)), 14, glm::vec4(1, 1, 1, 1));
  
  
}

void Craft::setTileMap(TileMap *tileMap)
{
  map = tileMap;
}

void Craft::setPosition(const glm::vec2 &pos)
{
  posCraft = pos;
    for(int i = 0; i<CRAFT_SIZE; ++i){
      sprites[i]->setPosition(glm::vec2(float(posCraft.x + i * (CRAFT_SIZE_X +2)), float(posCraft.y + (CRAFT_SIZE_Y*1.5 +2))));
      craftItems[i]->setPosition(glm::vec2(float(posCraft.x + i * (CRAFT_SIZE_X +2) +6), float(posCraft.y +2)+6 + (CRAFT_SIZE_Y*1.5 +2)));
    }
  
  
}


void Craft::decC0(int x)
{
    qu[0] -= x;
    if(0 > qu[0]){ 
      qu[0] = 0;
    }
}

void Craft::incC0(int x)
{
    qu[0] += x;
    if(qu[0] > MAX_STOCK) qu[0] = MAX_STOCK;
}

void Craft::decC1(int x)
{
    qu[1] -= x;
    if(0 > qu[1]){ 
      qu[1] = 0;
    }
}

void Craft::incC1(int x)
{
    qu[1] += x;
    if(qu[1] > MAX_STOCK) qu[1] = MAX_STOCK;
}



glm::ivec2 Craft::getPos() {
    return posCraft;
}

glm::ivec2 Craft::getSpSize() {
    return spriteSize;
}




