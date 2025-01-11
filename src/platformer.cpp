//
// Created by Denis Flynn on 21/10/2024.
//
//platformer.cpp

/*******
    !!!!! Please note that this is placeholder code, 
    the final version should consist of less than 10 (or less again) lines of code.
*******/
#include <SFML/Graphics.hpp>

#include "engine.h"
#include "system_renderer.h"
#include "system_resources.h"
#include "scenes/scene_level1.h"
#include "scenes/scene_level2.h"
#include "scenes/scene_level3.h"
#include "scenes/scene_menu.h"
#include <SFML/Audio.hpp>

#include "AudioManager.h"

using namespace sf;

Level1Scene level1;
Level2Scene level2;
Level3Scene level3;
Vector2i resolution;
int volume;

using namespace std;

MenuScene menu;
// std::shared_ptr<sf::SoundBuffer> backgroundMusic = make_shared<SoundBuffer>();;
int main() {
    resolution = Vector2i(720, 1280);
    volume = 5;
    bool loaded = AudioManager::playMusic("background.wav", true);
    loaded = AudioManager::loadSound("blaze", "blaze.wav");
    loaded = AudioManager::loadSound("jump", "jump.wav");
    loaded = AudioManager::loadSound("landed", "landing.wav");
    if (!loaded) {
        std::cerr << "Failed to load sound: blaze.wav" << std::endl;
    }
    AudioManager::setMusicVolume(volume);
    AudioManager::setSoundVolume("blaze", volume);
    AudioManager::setSoundVolume("jump", volume);
    AudioManager::setSoundVolume("landed", volume);

    cout << "MenuScene Loaded!2222" << endl;

    Engine::Start(gameWidth, gameHeight, "Platformer", &level3);
}
