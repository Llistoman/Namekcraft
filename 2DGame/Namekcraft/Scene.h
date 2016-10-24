#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include <SFML/Audio.hpp>
#include "ShaderProgram.h"
#include "SoundManager.h"
#include "World.h"
#include "Player.h"
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

private:
	void initShaders();

private:
    Quad *quad;
    Texture texs[3]; //vec de textures
    TexturedQuad *background[3];
    //TileMap *map;
    World *world;
    Player *player;
    SoundManager *manager;
    ShaderProgram texProgram;
    float currentTime;
    glm::mat4 projection;
};


#endif // _SCENE_INCLUDE

