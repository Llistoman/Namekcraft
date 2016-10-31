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

enum CraftAnims
{
  NORMAL, SELECTED
};

enum ItemAnims
{
  PICO, SWORD, DIRT, ROCK, WOOD, NAMEKITA, COSMIC, LIMONITA, POTION, SENZU,  NONE, NAMEKPICO, COSMICPICO, NAMEKSWORD, COSMICSWORD
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
      craftItems[i]->setNumberAnimations(15);  
      craftItems[i]->setAnimationSpeed(PICO, 8);
      craftItems[i]->addKeyframe(PICO, glm::vec2(0.f, 0.f));
      craftItems[i]->setAnimationSpeed(SWORD, 8);
      craftItems[i]->addKeyframe(SWORD, glm::vec2(0.125f, 0.f));
      craftItems[i]->setAnimationSpeed(DIRT, 8);
      craftItems[i]->addKeyframe(DIRT, glm::vec2(0.625f, 0.f));
      craftItems[i]->setAnimationSpeed(ROCK, 8);
      craftItems[i]->addKeyframe(ROCK, glm::vec2(0.625f, 0.25f)); 
      craftItems[i]->setAnimationSpeed(NONE, 8);
      craftItems[i]->addKeyframe(NONE, glm::vec2(0.f, 0.75f));
      craftItems[i]->setAnimationSpeed(WOOD, 8);
      craftItems[i]->addKeyframe(WOOD, glm::vec2(0.25, 0.));
      craftItems[i]->setAnimationSpeed(NAMEKITA, 8);
      craftItems[i]->addKeyframe(NAMEKITA, glm::vec2(0.25, 0.25));    
      craftItems[i]->setAnimationSpeed(COSMIC, 8);
      craftItems[i]->addKeyframe(COSMIC, glm::vec2(0.25, 0.5));  
      craftItems[i]->setAnimationSpeed(LIMONITA, 8);
      craftItems[i]->addKeyframe(LIMONITA, glm::vec2(0.625, 0.5)); 
      craftItems[i]->setAnimationSpeed(POTION, 8);
      craftItems[i]->addKeyframe(POTION, glm::vec2(0.375, 0.)); 
      craftItems[i]->setAnimationSpeed(SENZU, 8);
      craftItems[i]->addKeyframe(SENZU, glm::vec2(0.375, 0.5));  
      craftItems[i]->setAnimationSpeed(NONE, 8);
      craftItems[i]->addKeyframe(NONE, glm::vec2(0.f, 0.75f));
      craftItems[i]->setAnimationSpeed(NAMEKPICO, 8);
      craftItems[i]->addKeyframe(NAMEKPICO, glm::vec2(0.f, 0.25f));
      craftItems[i]->setAnimationSpeed(COSMICPICO, 8); 
      craftItems[i]->addKeyframe(COSMICPICO, glm::vec2(0.f, 0.5f));
      craftItems[i]->setAnimationSpeed(NAMEKSWORD, 8);
      craftItems[i]->addKeyframe(NAMEKSWORD, glm::vec2(0.125f, 0.25f));
      craftItems[i]->setAnimationSpeed(COSMICSWORD, 8); 
      craftItems[i]->addKeyframe(COSMICSWORD, glm::vec2(0.125f, 0.5f));
      craftItems[i]->changeAnimation(NONE); 
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
  //UPDATE ITEM CRAFT
  if (craftItems[0]->animation() == LIMONITA and qu[0]==1 and craftItems[1]->animation() == LIMONITA and qu[1]==1){ //POTION 
    craftItems[2]->changeAnimation(POTION);
  }
  else if (craftItems[0]->animation() == WOOD and qu[0]==1 and craftItems[1]->animation() == NAMEKITA and qu[1]==5){ //POTION 
    craftItems[2]->changeAnimation(NAMEKPICO);
  }
  else if (craftItems[0]->animation() == WOOD and qu[0]==1 and craftItems[1]->animation() == COSMIC and qu[1]==5){ //POTION 
    craftItems[2]->changeAnimation(COSMICPICO);
  }
  else if (craftItems[0]->animation() == WOOD and qu[0]==1 and craftItems[1]->animation() == NAMEKITA and qu[1]==10){ //POTION 
    craftItems[2]->changeAnimation(NAMEKSWORD);
  }
  else if (craftItems[0]->animation() == WOOD and qu[0]==1 and craftItems[1]->animation() == COSMIC and qu[1]==10){ //POTION 
    craftItems[2]->changeAnimation(COSMICSWORD);
  }
  else{
    craftItems[2]->changeAnimation(NONE);
  }
  
  
  //ITEM CRAFT MENU
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
    
    //Update craftSprites
    if(Game::instance().getKey('1'))
    {
      craftItems[1]->changeAnimation(PICO);
    }
    else if(Game::instance().getKey('2'))
    {
      craftItems[1]->changeAnimation(SWORD);
    }
      else if(Game::instance().getKey('3'))
    {
      craftItems[1]->changeAnimation(DIRT);
    }
      else if(Game::instance().getKey('4'))
    {
      craftItems[1]->changeAnimation(ROCK);
    }
      else if(Game::instance().getKey('5'))
    {
      craftItems[1]->changeAnimation(WOOD);
    }
      else if(Game::instance().getKey('6'))
    {
      craftItems[1]->changeAnimation(NAMEKITA);
    }
      else if(Game::instance().getKey('7'))
    {
      craftItems[1]->changeAnimation(COSMIC);
    }
      else if(Game::instance().getKey('8'))
    {
      craftItems[1]->changeAnimation(LIMONITA);
    }
      else if(Game::instance().getKey('9'))
    {
      craftItems[1]->changeAnimation(POTION);
    }
      else if(Game::instance().getKey('0'))
    {
      craftItems[1]->changeAnimation(SENZU);
    }
  }
  else {
    sprites[0]->changeAnimation(SELECTED);
    sprites[1]->changeAnimation(NORMAL);
    
        //Update craftSprites
    if(Game::instance().getKey('1'))
    {
      craftItems[0]->changeAnimation(PICO);
    }
    else if(Game::instance().getKey('2'))
    {
      craftItems[0]->changeAnimation(SWORD);
    }
      else if(Game::instance().getKey('3'))
    {
      craftItems[0]->changeAnimation(DIRT);
    }
      else if(Game::instance().getKey('4'))
    {
      craftItems[0]->changeAnimation(ROCK);
    }
      else if(Game::instance().getKey('5'))
    {
      craftItems[0]->changeAnimation(WOOD);
    }
      else if(Game::instance().getKey('6'))
    {
      craftItems[0]->changeAnimation(NAMEKITA);
    }
      else if(Game::instance().getKey('7'))
    {
      craftItems[0]->changeAnimation(COSMIC);
    }
      else if(Game::instance().getKey('8'))
    {
      craftItems[0]->changeAnimation(LIMONITA);
    }
      else if(Game::instance().getKey('9'))
    {
      craftItems[0]->changeAnimation(POTION);
    }
      else if(Game::instance().getKey('0'))
    {
      craftItems[0]->changeAnimation(SENZU);
    }
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
}

void Craft::renderTexts()
{

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

bool Craft::enoughQ(int x, int i)
{
  if(i >= 0 and 2 > i){  //retorna si tenim més de x del recurs i
    cout << (qu[i] >= x) << endl;
    return qu[i] >= x;
  }
  return false;
}



int  Craft::craftear(){ //Retorna l'objecte que crafteja, o -1 si no es una cmb valida
  if (craftItems[0]->animation() == LIMONITA and qu[0]==1 and craftItems[1]->animation() == LIMONITA and qu[1]==1){ //POTION 
    decC0(1);
    decC1(1);
    return POTION;
  }
  else if (craftItems[0]->animation() == PICO and qu[0]==1 and craftItems[1]->animation() == NAMEKITA and qu[1]==5){ //POTION 
    decC0(1);
    decC1(5);
    return NAMEKPICO;
  }
  else if (craftItems[0]->animation() == WOOD and qu[0]==1 and craftItems[1]->animation() == COSMIC and qu[1]==5){ //POTION 
    decC0(1);
    decC1(5);
    return COSMICPICO;
  }
  else if (craftItems[0]->animation() == WOOD and qu[0]==1 and craftItems[1]->animation() == NAMEKITA and qu[1]==10){ //POTION 
    decC0(1);
    decC1(10);
    return NAMEKSWORD;
  }
  else if (craftItems[0]->animation() == WOOD and qu[0]==1 and craftItems[1]->animation() == COSMIC and qu[1]==10){ //POTION 
    decC0(1);
    decC1(10);
    return COSMICSWORD;
  }
  return -1;

}

glm::ivec2 Craft::getPos() {
    return posCraft;
}

glm::ivec2 Craft::getSpSize() {
    return spriteSize;
}




