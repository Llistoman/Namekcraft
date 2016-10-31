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
    enem = NULL;
}

Scene::~Scene()
{
    if(world != NULL)
        delete world;
	if(player != NULL)
		delete player;
    if(enem != NULL)
        delete enem;
}


void Scene::initbackground() {
    glm::ivec2 size = world->getWorldSize();
    glm::vec2 geom1[2] = {glm::vec2(0.f, 0.f), glm::vec2(size.x*BLOCK_X, size.y*BLOCK_Y)};
    glm::vec2 texCoords1[2] = {glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f)};
    glm::vec2 geom2[2] = {glm::vec2(0.f, size.y*BLOCK_Y/2), glm::vec2(size.x*BLOCK_X , size.y*BLOCK_Y)};
    glm::vec2 texCoords2[2] = {glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f)};
    glm::vec2 geom3[2] = {glm::vec2(0.f, 0.f), glm::vec2(size.x*BLOCK_X/3, size.y*BLOCK_Y/3)};
    glm::vec2 texCoords3[2] = {glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f)};

    background[0] = TexturedQuad::createTexturedQuad(geom1, texCoords1, texProgram);
    background[1] = TexturedQuad::createTexturedQuad(geom2, texCoords2, texProgram);
    background[2] = TexturedQuad::createTexturedQuad(geom3, texCoords3, texProgram);

    // Load textures
    texs[0].loadFromFile("images/Sky.png", TEXTURE_PIXEL_FORMAT_RGBA);
    texs[1].loadFromFile("images/backgroundsimetric.png",TEXTURE_PIXEL_FORMAT_RGBA);
    texs[2].loadFromFile("images/clouds.png",TEXTURE_PIXEL_FORMAT_RGBA);
}

void Scene::init()
{
    //SHADERS
	initShaders();

    //MUSIC
    manager = new SoundManager();
    manager->playMusic();

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
    world->setInventory(player->inventory);

    //ENEMY
    enem = new Enemy();
    glm::ivec2 enemyPos = glm::ivec2((world->getWorldSize().x/2+2)*BLOCK_X,(floorlvl-10)*BLOCK_Y);
    enem->init(player, 0, enemyPos, texProgram);
    enem->setWorld(world);

    //CAMERA
    projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;

}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
    enem->update(deltaTime);
    glm::ivec2 newpos = player->getPosRender();
    glm::ivec2 screen = glm::ivec2(SCREEN_WIDTH,SCREEN_HEIGHT);
    world->update(newpos,screen);
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
    background[1]->render(texs[1]);
    background[2]->render(texs[2]);
    glm::ivec2 newpos = player->getPosRender();
    glm::ivec2 screen = glm::ivec2(SCREEN_WIDTH,SCREEN_HEIGHT);
    world->render(newpos,screen);
    enem->render();
    player->render();
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



