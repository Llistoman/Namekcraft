#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


#define SCREEN_X 32
#define SCREEN_Y 16

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 25


Scene::Scene()
{
	map = NULL;
	player = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
}


void Scene::initbackground() {
    glm::vec2 geom1[2] = {glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT)};
    glm::vec2 texCoords1[2] = {glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f)};
    glm::vec2 geom2[2] = {glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT/1.5f)};
    glm::vec2 texCoords2[2] = {glm::vec2(0.f, 0.f), glm::vec2(2.f, 1.f)};
    glm::vec2 geom3[2] = {glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT/3)};
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
	initShaders();
    initbackground();
	map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);
    projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
    glm::ivec2 newpos = player->getPos();
    //cout << newpos.x << " " << newpos.y << endl;
    projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
    projection = glm::translate(projection, glm::vec3(-newpos.x+SCREEN_WIDTH/2, -newpos.y+SCREEN_HEIGHT/2, 0.f));
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
	map->render();
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



