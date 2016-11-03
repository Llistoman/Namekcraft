#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE

#include "SceneMenu.h"
#include "SceneOver.h"
#include "Scene.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480


// Game is a singleton (a class with a single instance) that represents our whole application


class Game
{

public:
    Game();
    ~Game();
	
	static Game &instance()
	{
		static Game G;
	
		return G;
	}
	
	void init();
	bool update(int deltaTime);
	void render();
    void win();
    void reestart();
    void killed();
    void switchScene();
    void switchOver();

	// Input callback methods
	void keyPressed(int key);
	void keyReleased(int key);
	void specialKeyPressed(int key);
	void specialKeyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);
	
    std::pair<int,int> getMousePos() const;
    bool leftClick() const;
    bool rightClick() const;
	bool getKey(int key) const;
	bool getSpecialKey(int key) const;
    bool menuState() const;
    bool sceneState() const;

private:
    bool state, gameover;
	bool bPlay;                       // Continue to play game?
    SceneMenu *menu;
    SceneOver *over;
    Scene *scene;                      // Scene to render
    int mouse_x, mouse_y;             // Mouse position
    bool mButton[2];                  // Mouse button
	bool keys[256], specialKeys[256]; // Store key states so that 
                                      // we can have access at any time

};


#endif // _GAME_INCLUDE


