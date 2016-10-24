
#ifndef _SOUND_MANAGER_INCLUDE
#define _SOUND_MANAGER_INCLUDE

#include <iostream>
#include <SFML/Audio.hpp>

class SoundManager
{

public:
    SoundManager();
    void playMusic();
    void playCreate();
    void playDestroy();

private:
    sf::Music music;
    sf::SoundBuffer createBuff;
    sf::Sound create;
    sf::SoundBuffer destroyBuff;
    sf::Sound destroy;

};


#endif // _SOUND_MANAGER_INCLUDE
