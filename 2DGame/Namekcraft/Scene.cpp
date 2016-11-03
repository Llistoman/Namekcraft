#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <time.h>
#include "Scene.h"
#include "Game.h"

#define BLOCK_X 32
#define BLOCK_Y 32

#define SCREEN_X 32
#define SCREEN_Y 16

#define PLAYER_SIZE_X 32
#define PLAYER_SIZE_Y 32

Scene::Scene()
{
    world = NULL;
	player = NULL;
    manager = NULL;
}

Scene::~Scene()
{
    if(world != NULL)
        delete world;
	if(player != NULL)
		delete player;
    if(manager != NULL)
        delete manager;
    for(int i = 0; i < enemies.size(); ++i) {
        if(enemies[i] != NULL)
            delete enemies[i];
    }
}

void Scene::initbackground() {
    animateClouds = 0;
    glm::ivec2 size = world->getWorldSize();
    //SKY
    glm::vec2 geom1[2] = {glm::vec2(0.f, 0.f), glm::vec2(size.x*BLOCK_X, size.y*BLOCK_Y)};
    glm::vec2 texCoords1[2] = {glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f)};
    //BACKGROUND
    glm::vec2 geom2[2] = {glm::vec2(0.f, size.y*BLOCK_Y/2 -(224.f *2.) +160.f), glm::vec2(size.x*BLOCK_X , size.y*BLOCK_Y/2 +160.f)};
    glm::vec2 texCoords2[2] = {glm::vec2(0.f, 0.f), glm::vec2(2.85f, 1.f)};
    //CLOUDS
    glm::vec2 geom3[2] = {glm::vec2(0.f, size.y*BLOCK_Y/2 -(224.f *2.) ), glm::vec2(10*size.x*BLOCK_X/3, size.y*BLOCK_Y/2)};
    glm::vec2 texCoords3[2] = {glm::vec2(0.f, 0.f), glm::vec2(10.f, 1.f)};
    //Ground
    glm::vec2 geom4[2] = {glm::vec2(0.f, size.y*BLOCK_Y/2), glm::vec2(size.x*BLOCK_X*2 , size.y*BLOCK_Y)};
    glm::vec2 texCoords4[2] = {glm::vec2(0.f, 0.f), glm::vec2(13.f, 17.f)};
    //BACK BACK
    glm::vec2 geom5[2] = {glm::vec2(0.f, size.y*BLOCK_Y/2 -(224.f *2.) +160.f), glm::vec2(size.x*BLOCK_X , size.y*BLOCK_Y/2 +160.f)};
    glm::vec2 texCoords5[2] = {glm::vec2(0.f, 0.f), glm::vec2(2.85f, 1.f)};
    
    background[0] = TexturedQuad::createTexturedQuad(geom1, texCoords1, texProgram);
    background[1] = TexturedQuad::createTexturedQuad(geom2, texCoords2, texProgram);
    background[2] = TexturedQuad::createTexturedQuad(geom3, texCoords3, texProgram);
    background[3] = TexturedQuad::createTexturedQuad(geom4, texCoords4, texProgram);
    background[4] = TexturedQuad::createTexturedQuad(geom5, texCoords5, texProgram); 

    // Load textures
    texs[0].loadFromFile("images/Sky.png", TEXTURE_PIXEL_FORMAT_RGBA);
    texs[1].loadFromFile("images/backgroundsimetric_rework.png",TEXTURE_PIXEL_FORMAT_RGBA);
    texs[2].loadFromFile("images/clouds.png",TEXTURE_PIXEL_FORMAT_RGBA);
    texs[3].loadFromFile("images/background_ground_sim.png",TEXTURE_PIXEL_FORMAT_RGBA);
    texs[4].loadFromFile("images/Background_15.png",TEXTURE_PIXEL_FORMAT_RGBA);
}

void Scene::init()
{
    //SHADERS
	initShaders();

    //MUSIC
    manager = new SoundManager();
    manager->playMenuMusic();

    //WORLD
    int seed = time(NULL);
    int floorlvl = 50;
    glm::ivec2 worldSize = glm::ivec2(100,100);
    glm::ivec2 blockSize = glm::ivec2(BLOCK_X,BLOCK_Y);
    glm::ivec2 tilesheetSize = glm::ivec2(8,4);
    world = World::createWorld(seed,worldSize,blockSize,tilesheetSize,floorlvl,manager,texProgram);

    //BACKGROUND
    initbackground();

    //PLAYER
    player = new Player();
    glm::ivec2 playerPos = glm::ivec2(world->getWorldSize().x/2*BLOCK_X,(floorlvl-10)*BLOCK_Y);
    glm::vec2 playerSize = glm::vec2(PLAYER_SIZE_X,PLAYER_SIZE_Y);

    player->init(playerPos, playerSize, texProgram);
    player->setWorld(world);
    player->setSoundManager(manager);
    world->setInventory(player->inventory);

    //ENEMY
    for (int i = 0; i < worldSize.x/20; ++i) {
        Enemy *enem = new Enemy();
        glm::ivec2 enemyPos = glm::ivec2((20*i)*BLOCK_X,(floorlvl-10)*BLOCK_Y);
        enem->init(player, 0, enemyPos, texProgram);
        enem->setWorld(world);
        enem->setSoundManager(manager);
        enemies.push_back(enem);
    }

    for (int j = 0; j < worldSize.x/25; ++j) {
        for (int k = 0; k < worldSize.y/25; ++k) {
            Enemy *enem = new Enemy();
            glm::ivec2 enemyPos = glm::ivec2((25*j)*BLOCK_X,(25*k)*BLOCK_Y);
            enem->init(player, 1, enemyPos, texProgram);
            enem->setWorld(world);
            enem->setSoundManager(manager);
            enemies.push_back(enem);
        }
    }
    player->setEnemies(enemies);

    //SHIP
    glm::ivec2 pos = glm::ivec2(world->getWorldSize().x/2*BLOCK_X,(world->getWorldSize().y - 10)*BLOCK_Y);
    glm::vec2 geom[2] = {glm::vec2(pos.x,pos.y), glm::vec2(geom[0].x + 3*blockSize.x, geom[0].y + 3*blockSize.y)};
    glm::vec2 texCoords[2] = {glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f)};

    world->setShipPos(pos);
    shipTex.loadFromFile("images/ship.png",TEXTURE_PIXEL_FORMAT_RGBA);
    ship = TexturedQuad::createTexturedQuad(geom,texCoords,texProgram);

    //CAMERA
    projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;

}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
    glm::ivec2 shipPos = world->getShipPos();

    for(int i = 0; i < enemies.size(); ++i) {
        if(enemies[i] != NULL) {
            if(enemies[i]->isDead()) {
                delete enemies[i];
                enemies[i] = NULL;
            }
            else enemies[i]->update(deltaTime);
        }
    }
    player->update(deltaTime);
        
     // BACK BACKGROUND
    float dipBack = player->getPosRender().x*0.1;
     
    glm::vec2 geom5[2] = {glm::vec2(-(100*BLOCK_X -dipBack) - dipBack, 100*BLOCK_Y/2 -(540.f) +400.f), glm::vec2((100*BLOCK_X -dipBack)*2 , 100*BLOCK_Y/2 +400.f)};
    glm::vec2 texCoords5[2] = {glm::vec2(0.f, 0.f), glm::vec2(3.f, 1.f)};
    
    background[4] = TexturedQuad::createTexturedQuad(geom5, texCoords5, texProgram);
     
    //CLOUDS UPDATE
    animateClouds += deltaTime *0.05;
    if (animateClouds >= 10*100*BLOCK_X/3) animateClouds = 0;
    
    glm::vec2 geom3[2] = {glm::vec2(-(10*100*BLOCK_X/3) + animateClouds, 100*BLOCK_Y/2 -(224.f *2.) ), glm::vec2(10*100*BLOCK_X/3 + animateClouds, 100*BLOCK_Y/2)};
    glm::vec2 texCoords3[2] = {glm::vec2(0.f, 0.f), glm::vec2(9.f, 1.f)};

    background[2] = TexturedQuad::createTexturedQuad(geom3, texCoords3, texProgram);
    
    glm::ivec2 newpos = player->getPosRender();
    glm::ivec2 screen = glm::ivec2(SCREEN_WIDTH,SCREEN_HEIGHT);
    world->update(newpos,screen,deltaTime);
    projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
    projection = glm::translate(projection, glm::vec3(-newpos.x+SCREEN_WIDTH/2 - PLAYER_SIZE_X, -newpos.y+SCREEN_HEIGHT/2 - PLAYER_SIZE_Y, 0.f));
}

void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

    background[0]->render(texs[0]);
    background[4]->render(texs[4]);
    background[2]->render(texs[2]);
    background[1]->render(texs[1]);
    background[3]->render(texs[3]);

    glm::ivec2 newpos = player->getPosRender();
    glm::ivec2 screen = glm::ivec2(SCREEN_WIDTH,SCREEN_HEIGHT);
    world->render(newpos,screen);
    ship->render(shipTex);
    for(int i = 0; i < enemies.size(); ++i) {
        if(enemies[i] != NULL) {
            enemies[i]->render();
        }
    }
    player->render();

}

void Scene::changeMusic(int i)
{
    if(i == 0) manager->playMusic();
    else if(i == 1) manager->playOverMusic();
    else if(i == 2) manager->playWinMusic();
    else manager->playMenuMusic();
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}



