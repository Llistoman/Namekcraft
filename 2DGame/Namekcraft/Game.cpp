#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


void Game::init()
{
	bPlay = true;
    glClearColor(0.f, 0.f, 0.f, 1.0f);
	scene.init();
    menu.init();
    state = false;
}

bool Game::update(int deltaTime)
{
    if(state == false) menu.update(deltaTime);
    else scene.update(deltaTime);
	
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if(state == false) menu.render();
    else scene.render();
}

void Game::keyPressed(int key)
{
	if(key == 27) // Escape code
		bPlay = false;
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
    mouse_x = x;
    mouse_y = y;
}

void Game::mousePress(int button)
{
    if(button == 0) mButton[0] = true;
    if(button == 2) mButton[1] = true;
}

void Game::mouseRelease(int button)
{
    if(button == 0) mButton[0] = false;
    if(button == 2) mButton[1] = false;
}

std::pair<int,int> Game::getMousePos() const
{
    return std::make_pair(mouse_x,mouse_y);
}

bool Game::leftClick() const
{
    return mButton[0];
}

bool Game::rightClick() const
{
    return mButton[1];
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}

bool Game::menuState() const
{
    return (state == false);
}

bool Game::sceneState() const
{
    return (state == true);
}

void Game::switchScene()
{
    state = !state;
}





