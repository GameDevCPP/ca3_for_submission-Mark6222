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

using namespace sf;

Level1Scene level1;
Level2Scene level2;
Level3Scene level3;
Vector2i resolution;
int volume;

using namespace std;

MenuScene menu;
sf::Music backgroundMusic;
int main() {
    resolution = Vector2i(720, 1280);
    volume = 20;

    if (!backgroundMusic.openFromFile("res/sound/background.wav")) {
        std::cerr << "Error: Failed to load background music!" << std::endl;
        return -1;
    }

    backgroundMusic.setVolume(volume);
    backgroundMusic.setLoop(true);
    backgroundMusic.play();

    cout << "MenuScene Loaded!2222" << endl;

    Engine::Start(gameWidth, gameHeight, "Platformer", &menu);
}
