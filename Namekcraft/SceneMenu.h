#ifndef _SCENE_MENU_INCLUDE
#define _SCENE_MENU_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "SoundManager.h"
#include "TexturedQuad.h"
#include "Quad.h"

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
    Texture menuScreen;
    Texture credits;
    Texture inst;
    Texture texs[8];
    TexturedQuad *screen;
    TexturedQuad *options[4];
    ShaderProgram texProgram;
    float currentTime;
    glm::mat4 projection;
};

#endif // _SCENE_MENU_INCLUDE
