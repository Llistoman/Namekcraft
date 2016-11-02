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

#define MAX_HP 100
#define HP_SIZE 14

enum PlayerAnims
{
    STAND, MOVE, RUN, JUMP, DEATH
};

enum ItemAnims
{
    SENZU, PICO, SWORD, DIRT, ROCK, WOOD, NAMEKITA, COSMIC, LIMONITA, POTION, NONE, NAMEKPICO, COSMICPICO, NAMEKSWORD, COSMICSWORD //Ordre alterat
};


void Player::init(const glm::ivec2 &position, const glm::vec2 &spSize, ShaderProgram &shaderProgram)
{
    pico = sword =  0;   //Pico upgradejat 0
    usat = true;
    //Carga Fonts del HP TEXT
    if(!topText.init("fonts/Andy")) cout << "Could not load font!!!" << endl;
    hp = MAX_HP;
    death = false;
    
    bJumping = false;
    spriteSize = spSize;
    spritesheet.loadFromFile("images/character.png", TEXTURE_PIXEL_FORMAT_RGBA);
    //ALL OF THIS DEPENDS ON SPRITESHEET, MUST BE HARDCODED

    sprite = Sprite::createSprite(spSize, glm::vec2(0.25, 0.25), &spritesheet, &shaderProgram);
    sprite->setNumberAnimations(5);

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
    
    sprite->setAnimationSpeed(DEATH, 8);
    sprite->addKeyframe(DEATH, glm::vec2(0.75f, 0.25f));


    sprite->changeAnimationHTC(0);
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
    
    //Inventory
    craft = new Craft();
    craft->init(position, shaderProgram);
    craft->setPosition(glm::vec2(INIT_ITEM_X_TILES * 32, INIT_ITEM_Y_TILES * 32));
    timer = time(0);

}

void Player::update(int deltaTime)
{
    if(0 >= hp) dead();
    sprite->updateHTC(deltaTime);

    if(!death){ //Si estem morts no tenim control

        pair<int,int> mPos = Game::instance().getMousePos();
        if(mPos.first >= SCREEN_WIDTH/2) dir = 1;
        else dir = 0;

        //RUN OR NOT
        if(Game::instance().getKey('z')){
            playerSpeed = 5;
        }
        else{
            playerSpeed = 2;
        }

        //Attk
        if(Game::instance().leftClick()) {
            if(inventory->getSelected() == 2) {
                int x0, x1, y0, y1, dmg;
                if(inventory->getSelected() == 2) dmg = 20 + 20*sword; //Mal proporcional al arma
                else dmg = 5; //UNSELECTED
                if(dir == 0) { //LEFT
                    x0 = posPlayer.x - 20 - spriteSize.x;
                    x1 = posPlayer.x + 10;
                }
                else { //RIGHT
                    x0 = posPlayer.x - 10;
                    x1 = posPlayer.x + 20;
                }
                y0 = posPlayer.y - 20 - spriteSize.y;
                y1 = posPlayer.y + 20 + spriteSize.y;
                int duration = difftime(time(0),timer);
                if(duration >= 1) {
                    for(int i = 0; i < enemies.size(); ++i) {
                        if(enemies[i] != NULL) {
                            int x = enemies[i]->getPosRender().x;
                            int y = enemies[i]->getPosRender().y;
                            if(x > x0 and x <= x1 and y > y0 and y <= y1) {
                                enemies[i]->damage(dmg);
                            }
                        }
                    }
                    timer = time(0);
                }
            }
        }

        //Craft
        if(Game::instance().getKey('r')){
            int c = craft->craftear();
            if(c == POTION-1 and inventory->enoughS(2,LIMONITA-1)){
                inventory->decS(2,LIMONITA-1);
                inventory->incS(1,POTION-1);
                manager->playCraft();
            }
            else if(c == NAMEKPICO and inventory->enoughS(1,WOOD-1) and inventory->enoughS(5,NAMEKITA-1)){
                inventory->decS(5,NAMEKITA-1);
                inventory->decS(1, WOOD-1);
                inventory->setPico(1);
                item->setPico(1);
                pico = 1;
                manager->playCraft();
            }
            else if(c == NAMEKSWORD and inventory->enoughS(1,WOOD-1) and inventory->enoughS(10,NAMEKITA-1)){
                inventory->decS(10,NAMEKITA-1);
                inventory->decS(1, WOOD-1);
                inventory->setSword(1);
                item->setSword(1);
                sword = 1;
                manager->playCraft();
            }
            else if(c == COSMICPICO and inventory->enoughS(1,WOOD-1) and inventory->enoughS(5,COSMIC-1)){
                inventory->decS(5,COSMIC-1);
                inventory->decS(1, WOOD-1);
                inventory->setPico(2);
                item->setPico(2);
                pico = 2;
                manager->playCraft();
            }
            else if(c == COSMICSWORD and inventory->enoughS(1,WOOD-1) and inventory->enoughS(10,COSMIC-1)){
                inventory->decS(10,COSMIC-1);
                inventory->decS(1, WOOD-1);
                inventory->setSword(2);
                item->setSword(2);
                sword = 2;
                manager->playCraft();
            }
        }

        //FI CRAFT

        if(Game::instance().getSpecialKey(GLUT_KEY_LEFT))
        {
            dir = 0;
            if(playerSpeed == 2){
                if(sprite->animation() != MOVE) sprite->changeAnimationHTC(MOVE);
            }
            else{
                if(sprite->animation() != RUN) sprite->changeAnimationHTC(RUN);
            }
            posPlayer.x -= playerSpeed;
            if(world->collisionMoveLeft(posPlayer, spriteSize))
            {
                posPlayer.x += playerSpeed;
                sprite->changeAnimationHTC(STAND);
            }
        }
        else if(Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
        {
            dir = 1;
            if(playerSpeed == 2){
                if(sprite->animation() != MOVE) sprite->changeAnimationHTC(MOVE);
            }
            else{
                if(sprite->animation() != RUN) sprite->changeAnimationHTC(RUN);
            }
            posPlayer.x += playerSpeed;
            if(world->collisionMoveRight(posPlayer, spriteSize))
            {
                posPlayer.x -= playerSpeed;
                sprite->changeAnimationHTC(STAND);
            }
        }
        else
        {
            if(sprite->animation() == MOVE or sprite->animation() == RUN )
                sprite->changeAnimationHTC(STAND);
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
                if(world->collisionMoveUp(posPlayer, spriteSize, &posPlayer.y)){
                    startY = posPlayer.y;
                    bJumping = false;
                    jumpAngle = 0;
                }
                else posPlayer.y = int(startY - (spriteSize.y*3) * sin(3.14159f * jumpAngle / 180.f));
                if(jumpAngle > 90) {
                    bJumping = !world->collisionMoveDown(posPlayer, spriteSize, &posPlayer.y);
                }
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
                    jumpAngle = 0;
                    startY = posPlayer.y;
                }
            }
        }


        if(Game::instance().leftClick() and inventory->getSelected() == POTION and inventory->enoughS(1,POTION-1) and !usat) {
            heal(20);
            usat = true;
            inventory->decS(1,POTION-1);
        }
    }
    
    if(Game::instance().leftClick() and inventory->getSelected() == SENZU and inventory->enoughS(1,9) and !usat){  //Les mongetes magiques es poden pendre inclus mort
        heal(MAX_HP);
        death = false;
        usat = true;
        inventory->decS(1,9);
    }
    
    if(usat and !Game::instance().leftClick()) usat = false;

    //Marges del mapa
    if(0. >= posPlayer.x) posPlayer.x = 0;
    if( posPlayer.x >= 100*32) posPlayer.x = 100*32;
    if(0. >= posPlayer.y) posPlayer.y = 0.;
    if(posPlayer.y >= (100*32 -36)){
        posPlayer.y = 100*32 -36;
        dead();
    }
    
    //FINAL PLAYER
    if(!world->collisionMoveDown(posPlayer + glm::ivec2(0,1), spriteSize, &posPlayer.y) and sprite->animation() != JUMP )sprite->changeAnimationHTC(JUMP);
    else if(sprite->animation() == JUMP and world->collisionMoveDown(posPlayer + glm::ivec2(0,1), spriteSize, &posPlayer.y))sprite->changeAnimationHTC(STAND);
    
    sprite->setPosition(glm::vec2(float(posPlayer.x), float(posPlayer.y)));
    
    //ITEM
    if(dir==0)item->setPosition(glm::vec2(float(posPlayer.x - ITEM_DISTANCE), float(posPlayer.y)));
    else item->setPosition(glm::vec2(float(posPlayer.x + ITEM_DISTANCE), float(posPlayer.y)));
    item->update(deltaTime);
    
    //INVENTORY
    inventory->setPosition(glm::vec2(float(posPlayer.x - SCREEN_WIDTH/2 + 48), float(posPlayer.y - SCREEN_HEIGHT/2 + 48)));
    inventory->update(deltaTime);
    
    //Craft
    craft->setPosition(glm::vec2(float(posPlayer.x - SCREEN_WIDTH/2 + 48), float(posPlayer.y - SCREEN_HEIGHT/2 + 48)));
    craft->update(deltaTime);
}

void Player::render()
{
    sprite->render(dir);
    item->render(dir);
    craft->render();
    inventory->render();
    craft->renderTexts();
    topText.render( to_string(hp) + "/" + to_string(MAX_HP), glm::vec2(float(SCREEN_WIDTH/2 -40), float(SCREEN_HEIGHT/2 -38)), HP_SIZE, glm::vec4(1, 1, 1, 1));
}

void Player::free()
{
    sprite->free();
}

void Player::setEnemies(vector<Enemy *> &vec)
{
    enemies = vec;
}

void Player::setSoundManager(SoundManager *m)
{
    manager = m;
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

void Player::damage(int d)
{
    hp -= d;
    manager->playPlayerDmg();
    if(0 > hp){
        hp = 0;
        dead();
    }
}

void Player::dead()
{
    death = true;
    sprite->changeAnimationHTC(DEATH);
    Game::instance().switchOver();
}

void Player::heal(int h)
{
    hp += h;
    if(hp > MAX_HP) hp = MAX_HP;
}

glm::ivec2 Player::getPosRender() {
    return posPlayer;
}

glm::ivec2 Player::getSpSize() {
    return spriteSize;
}




