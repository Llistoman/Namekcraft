#include "SoundManager.h"

SoundManager::SoundManager() {
    if(!music.openFromFile("sound/garden.WAV")) std::cout << "error loading music" << std::endl;
    if(!createBuff.loadFromFile("sound/General Sounds/Neutral Sounds/sfx_sound_neutral8.wav")) std::cout << "error loading create sound" << std::endl;
    if(!destroyBuff.loadFromFile("sound/General Sounds/Neutral Sounds/sfx_sound_neutral11.wav")) std::cout << "error loading destroy sound" << std::endl;
}

void SoundManager::playMusic() {
    music.setLoop(true);
    music.play();
}

void SoundManager::playCreate() {
    create.setBuffer(createBuff);
    create.play();
}

void SoundManager::playDestroy() {
    destroy.setBuffer(destroyBuff);
    destroy.play();
}
