#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <time.h>
#include "SceneOver.h"
#include "Game.h"

SceneOver::SceneOver()
{

}

SceneOver::~SceneOver()
{

}


void SceneOver::init()
{
    state = false;

    //SHADERS
	initShaders();

    //OPTIONS
    glm::vec2 geom1[2] = {glm::vec2(SCREEN_WIDTH/4.f, 0.f), glm::vec2(3*SCREEN_WIDTH/4.f, 2*SCREEN_HEIGHT/4.f)};
    glm::vec2 texCoords1[2] = {glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f)};
    options[0] = TexturedQuad::createTexturedQuad(geom1, texCoords1, texProgram);
    glm::vec2 geom2[2] = {glm::vec2(SCREEN_WIDTH/4.f, 2*SCREEN_HEIGHT/4.f), glm::vec2(2*SCREEN_WIDTH/4.f, 3*SCREEN_HEIGHT/4.f)};
    glm::vec2 texCoords2[2] = {glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f)};
    options[1] = TexturedQuad::createTexturedQuad(geom2, texCoords2, texProgram);
    glm::vec2 geom3[2] = {glm::vec2(2*SCREEN_WIDTH/4.f, 2*SCREEN_HEIGHT/4.f), glm::vec2(3*SCREEN_WIDTH/4.f, 3*SCREEN_HEIGHT/4.f)};
    glm::vec2 texCoords3[2] = {glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f)};
    options[2] = TexturedQuad::createTexturedQuad(geom3, texCoords3, texProgram);

    //OPTIONS
    congrats.loadFromFile("images/congrats.png",TEXTURE_PIXEL_FORMAT_RGBA);
    gameover.loadFromFile("images/gameover.png", TEXTURE_PIXEL_FORMAT_RGBA);
    texs[0].loadFromFile("images/yes1.png", TEXTURE_PIXEL_FORMAT_RGBA);
    texs[1].loadFromFile("images/yes2.png", TEXTURE_PIXEL_FORMAT_RGBA);
    texs[2].loadFromFile("images/no1.png", TEXTURE_PIXEL_FORMAT_RGBA);
    texs[3].loadFromFile("images/no2.png", TEXTURE_PIXEL_FORMAT_RGBA);

    //CAMERA
    projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;

}

void SceneOver::update(int deltaTime)
{
    currentTime += deltaTime;
    std::pair<int,int> pos = Game::instance().getMousePos();
    if(Game::instance().leftClick()) {
        if(pos.first >= SCREEN_WIDTH/4.f and pos.first <= 3*SCREEN_WIDTH/4.f) {
            if(pos.second >=  2*SCREEN_HEIGHT/4.f and pos.second <= 3*SCREEN_HEIGHT/4.f and pos.first >= SCREEN_WIDTH/4.f and pos.first <= 2*SCREEN_WIDTH/4.f) {
                Game::instance().reestart();
                Game::instance().switchOver();
            }
            if(pos.second >=  2*SCREEN_HEIGHT/4.f and pos.second <= 3*SCREEN_HEIGHT/4.f and pos.first >= 2*SCREEN_WIDTH/4.f and pos.first <= 3*SCREEN_WIDTH/4.f) {
                Game::instance().reestart();
                Game::instance().switchOver();
                Game::instance().switchScene();
            }
        }
    }
}

void SceneOver::render()
{
    glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
    if(not state) options[0]->render(gameover);
    else options[0]->render(congrats);

    //render
    std::pair<int,int> pos = Game::instance().getMousePos();
    if(pos.first >= SCREEN_WIDTH/4.f and pos.first <= 3*SCREEN_WIDTH/4.f) {
        if(pos.second >=  2*SCREEN_HEIGHT/4.f and pos.second <= 3*SCREEN_HEIGHT/4.f and pos.first >= SCREEN_WIDTH/4.f and pos.first <= 2*SCREEN_WIDTH/4.f) options[1]->render(texs[1]);
        else options[1]->render(texs[0]);
        if(pos.second >=  2*SCREEN_HEIGHT/4.f and pos.second <= 3*SCREEN_HEIGHT/4.f and pos.first >= 2*SCREEN_WIDTH/4.f and pos.first <= 3*SCREEN_WIDTH/4.f) options[2]->render(texs[3]);
        else options[2]->render(texs[2]);
    }
    else {
        options[1]->render(texs[0]);
        options[2]->render(texs[2]);
    }
}

void SceneOver::switchState()
{
    state = !state;
}

void SceneOver::initShaders()
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



