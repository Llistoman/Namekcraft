
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
    void playPlayerDmg();
    void playDeath1();
    void playDmg1();
    void playDeath2();
    void playDmg2();
    void playCraft();

private:
    sf::Music music;
    sf::SoundBuffer createBuff;
    sf::Sound create;
    sf::SoundBuffer destroyBuff;
    sf::Sound destroy;
    sf::Sound player;
    sf::SoundBuffer dmgBuff;
    sf::SoundBuffer death1Buff;
    sf::SoundBuffer dmg1Buff;
    sf::Sound enemy1;
    sf::SoundBuffer dmg2Buff;
    sf::SoundBuffer death2Buff;
    sf::Sound enemy2;
    sf::SoundBuffer craftBuff;
    sf::Sound craft;

};


#endif // _SOUND_MANAGER_INCLUDE
