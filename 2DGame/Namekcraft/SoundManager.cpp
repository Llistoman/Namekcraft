#include "SoundManager.h"

SoundManager::SoundManager() {
    if(!music.openFromFile("sound/garden.WAV")) std::cout << "error loading music" << std::endl;
    if(!menu.openFromFile("sound/menu.wav")) std::cout << "error loading music" << std::endl;
    if(!over.openFromFile("sound/over.wav")) std::cout << "error loading music" << std::endl;
    if(!win.openFromFile("sound/win.wav")) std::cout << "error loading music" << std::endl;
    if(!createBuff.loadFromFile("sound/General Sounds/Neutral Sounds/sfx_sound_neutral8.wav")) std::cout << "error loading create sound" << std::endl;
    if(!destroyBuff.loadFromFile("sound/General Sounds/Neutral Sounds/sfx_sound_neutral11.wav")) std::cout << "error loading destroy sound" << std::endl;
    if(!death1Buff.loadFromFile("sound/Death Screams/Alien/sfx_deathscream_alien1.wav")) std::cout << "error loading death1 sound" << std::endl;
    if(!dmg1Buff.loadFromFile("sound/Death Screams/Human/sfx_deathscream_human1.wav")) std::cout << "error loading dmg1 sound" << std::endl;
    if(!death2Buff.loadFromFile("sound/Death Screams/Android/sfx_deathscream_android8.wav")) std::cout << "error loading death2 sound" << std::endl;
    if(!dmg2Buff.loadFromFile("sound/Death Screams/Android/sfx_deathscream_android1.wav")) std::cout << "error loading dmg2 sound" << std::endl;
    if(!dmgBuff.loadFromFile("sound/Death Screams/Human/sfx_deathscream_human9.wav")) std::cout << "error loading playerdmg sound" << std::endl;
    if(!craftBuff.loadFromFile("sound/General Sounds/Positive Sounds/sfx_sounds_powerup3.wav")) std::cout << "error loading playerdmg sound" << std::endl;
}

void SoundManager::stopAll() {
    music.stop();
    over.stop();
    win.stop();
    menu.stop();
}

void SoundManager::playMenuMusic() {
    menu.setLoop(true);
    menu.play();
}

void SoundManager::playOverMusic() {
    over.play();
}

void SoundManager::playWinMusic() {
    win.play();
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

void SoundManager::playPlayerDmg() {
    player.setBuffer(dmgBuff);
    player.play();
}

void SoundManager::playDeath1() {
    enemy1.setBuffer(death1Buff);
    enemy1.play();
}

void SoundManager::playDmg1() {
    enemy1.setBuffer(dmg1Buff);
    enemy1.play();
}

void SoundManager::playDeath2() {
    enemy2.setBuffer(death2Buff);
    enemy2.play();
}

void SoundManager::playDmg2() {
    enemy2.setBuffer(dmg2Buff);
    enemy2.play();
}

void SoundManager::playCraft() {
    player.setBuffer(craftBuff);
    player.play();
}

