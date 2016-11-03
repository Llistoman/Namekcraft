#ifndef _SCENE_MENU_INCLUDE
#define _SCENE_MENU_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "SoundManager.h"
#include "TexturedQuad.h"
#include "Quad.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class SceneMenu
{

public:
    SceneMenu();
    ~SceneMenu();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();

private:
    bool menu, cred, instructions;
    Texture credits;
    Texture inst;
    Texture texs[6];
    TexturedQuad *screen;
    TexturedQuad *options[3];
    ShaderProgram texProgram;
    float currentTime;
    glm::mat4 projection;
};


#endif // _SCENE_MENU_INCLUDE

