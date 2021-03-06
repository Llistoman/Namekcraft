#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <time.h>
#include "SceneMenu.h"
#include "Game.h"

SceneMenu::SceneMenu(){}

SceneMenu::~SceneMenu(){}

void SceneMenu::init()
{
    //SHADERS
	initShaders();

    //OPTIONS
    glm::vec2 geom1[2] = {glm::vec2(SCREEN_WIDTH/3.f, 4*SCREEN_HEIGHT/8.f), glm::vec2(2*SCREEN_WIDTH/3.f, 5*SCREEN_HEIGHT/8.f)};
    glm::vec2 texCoords1[2] = {glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f)};
    options[0] = TexturedQuad::createTexturedQuad(geom1, texCoords1, texProgram);
    glm::vec2 geom2[2] = {glm::vec2(SCREEN_WIDTH/3.f, 5*SCREEN_HEIGHT/8.f), glm::vec2(2*SCREEN_WIDTH/3.f, 6*SCREEN_HEIGHT/8.f)};
    glm::vec2 texCoords2[2] = {glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f)};
    options[1] = TexturedQuad::createTexturedQuad(geom2, texCoords2, texProgram);
    glm::vec2 geom3[2] = {glm::vec2(SCREEN_WIDTH/3.f, 6*SCREEN_HEIGHT/8.f), glm::vec2(2*SCREEN_WIDTH/3.f, 7*SCREEN_HEIGHT/8.f)};
    glm::vec2 texCoords3[2] = {glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f)};
    options[2] = TexturedQuad::createTexturedQuad(geom3, texCoords3, texProgram);
    glm::vec2 geom4[2] = {glm::vec2(2*SCREEN_WIDTH/3.f, 7*SCREEN_HEIGHT/8.f), glm::vec2(3*SCREEN_WIDTH/3.f, 8*SCREEN_HEIGHT/8.f)};
    glm::vec2 texCoords4[2] = {glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f)};
    options[3] = TexturedQuad::createTexturedQuad(geom4, texCoords4, texProgram);

    //OPTIONS
    menu = true;
    instructions = false;
    cred = false;
    texs[0].loadFromFile("images/play1.png", TEXTURE_PIXEL_FORMAT_RGBA);
    texs[1].loadFromFile("images/play2.png", TEXTURE_PIXEL_FORMAT_RGBA);
    texs[2].loadFromFile("images/inst1.png", TEXTURE_PIXEL_FORMAT_RGBA);
    texs[3].loadFromFile("images/inst2.png", TEXTURE_PIXEL_FORMAT_RGBA);
    texs[4].loadFromFile("images/credits1.png", TEXTURE_PIXEL_FORMAT_RGBA);
    texs[5].loadFromFile("images/credits2.png", TEXTURE_PIXEL_FORMAT_RGBA);
    texs[6].loadFromFile("images/back1.png", TEXTURE_PIXEL_FORMAT_RGBA);
    texs[7].loadFromFile("images/back2.png", TEXTURE_PIXEL_FORMAT_RGBA);
    menuScreen.loadFromFile("images/menuscreen.png",TEXTURE_PIXEL_FORMAT_RGBA);
    credits.loadFromFile("images/credits.png",TEXTURE_PIXEL_FORMAT_RGBA);
    inst.loadFromFile("images/inst.png",TEXTURE_PIXEL_FORMAT_RGBA);

    //SCREEN
    glm::vec2 geom[2] = {glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT)};
    glm::vec2 texCoords[2] = {glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f)};
    screen = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);

    //CAMERA
    projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;

}

void SceneMenu::update(int deltaTime)
{
    //ES UNA MICA BUGGY
	currentTime += deltaTime;
    std::pair<int,int> pos = Game::instance().getMousePos();
    if(instructions or cred) {
        if(Game::instance().leftClick() and pos.first >= 2*SCREEN_WIDTH/3.f and pos.first <= 3*SCREEN_WIDTH/3.f and pos.second >= 7*SCREEN_HEIGHT/8.f and pos.second <= 8*SCREEN_HEIGHT/8.f) {
            menu = true;
            instructions = false;
            cred = false;
        }
    }
    else if(pos.first >= SCREEN_WIDTH/3.f and pos.first <= 2*SCREEN_WIDTH/3.f) {
        if(pos.second >=  4*SCREEN_HEIGHT/8.f and pos.second <= 5*SCREEN_HEIGHT/8.f) {
            if(Game::instance().leftClick()) Game::instance().switchScene();
        }
        if(pos.second >= 5*SCREEN_HEIGHT/8.f and pos.second <= 6*SCREEN_HEIGHT/8.f) {
            if(Game::instance().leftClick()) {
                menu = false;
                instructions = true;
                cred = false;
            }
        }
        if(pos.second >= 6*SCREEN_HEIGHT/8.f and pos.second <= 7*SCREEN_HEIGHT/8.f) {
            if(Game::instance().leftClick()) {
                menu = false;
                instructions = false;
                cred = true;
            }
        }
    }
}

void SceneMenu::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

    std::pair<int,int> pos = Game::instance().getMousePos();
    if(instructions) {
        screen->render(inst);
        if (pos.first >= 2*SCREEN_WIDTH/3.f and pos.first <= 3*SCREEN_WIDTH/3.f and pos.second >= 7*SCREEN_HEIGHT/8.f and pos.second <= 8*SCREEN_HEIGHT/8.f) {
            options[3]->render(texs[7]);
        }
        else options[3]->render(texs[6]);
    }
    else if(cred) {
        screen->render(credits);
        if (pos.first >= 2*SCREEN_WIDTH/3.f and pos.first <= 3*SCREEN_WIDTH/3.f and pos.second >= 7*SCREEN_HEIGHT/8.f and pos.second <= 8*SCREEN_HEIGHT/8.f) {
            options[3]->render(texs[7]);
        }
        else options[3]->render(texs[6]);
    }
    else if(pos.first >= SCREEN_WIDTH/3.f and pos.first <= 2*SCREEN_WIDTH/3.f) {
        screen->render(menuScreen);
        if(pos.second >=  4*SCREEN_HEIGHT/8.f and pos.second <= 5*SCREEN_HEIGHT/8.f) options[0]->render(texs[1]);
        else options[0]->render(texs[0]);
        if(pos.second >=  5*SCREEN_HEIGHT/8.f and pos.second <= 6*SCREEN_HEIGHT/8.f) options[1]->render(texs[3]);
        else options[1]->render(texs[2]);
        if(pos.second >=  6*SCREEN_HEIGHT/8.f and pos.second <= 7*SCREEN_HEIGHT/8.f) options[2]->render(texs[5]);
        else options[2]->render(texs[4]);
    }
    else {
        screen->render(menuScreen);
        options[0]->render(texs[0]);
        options[1]->render(texs[2]);
        options[2]->render(texs[4]);
    }
}

void SceneMenu::initShaders()
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
