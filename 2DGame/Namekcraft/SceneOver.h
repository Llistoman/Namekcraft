#ifndef _SCENE_OVER_INCLUDE
#define _SCENE_OVER_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "SoundManager.h"
#include "TexturedQuad.h"
#include "Quad.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class SceneOver
{

public:
    SceneOver();
    ~SceneOver();

	void init();
	void update(int deltaTime);
	void render();
    void switchState();

private:
	void initShaders();

private:
    bool state;
    Texture congrats;
    Texture gameover;
    Texture texs[4]; //vec de textures opcions
    TexturedQuad *screen;
    TexturedQuad *options[3];
    ShaderProgram texProgram;
    float currentTime;
    glm::mat4 projection;
};


#endif // _SCENE_OVER_INCLUDE

