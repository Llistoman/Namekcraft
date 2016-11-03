#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "SoundManager.h"
#include "World.h"
#include "Player.h"
#include "Enemy.h"
#include "TexturedQuad.h"
#include "Quad.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

    void initbackground();
	void init();
	void update(int deltaTime);
	void render();
    void changeMusic(int i);

private:
	void initShaders();

private:
    Quad *quad;
    Texture shipTex;
    TexturedQuad *ship;
    Texture texs[5]; //vec de textures
    TexturedQuad *background[5];
    float animateClouds;
    //TileMap *map;
    World *world;
    Player *player;
    vector<Enemy *> enemies;
    SoundManager *manager;
    ShaderProgram texProgram;
    float currentTime;
    glm::mat4 projection;
};


#endif // _SCENE_INCLUDE

