#include "scene_menu.h"
#include "../components/cmp_text.h"
#include "../game.h"
#include <SFML/Window/Keyboard.hpp>
#include"../components/cmp_button.h"

#include <iostream>
shared_ptr<Entity> btnStart;
shared_ptr<Entity> btnExit;
Sprite background;

void MenuScene::Load() {
    menuView.reset(FloatRect(0, 0, resolution.x, resolution.y));

    auto backTexture = Resources::get<Texture>("background.png");
    background.setTexture(*backTexture);
    background.setPosition(-200, 0);
    background.setScale(1.0f, 1.35f);

    text.setFont(*Resources::get<sf::Font>("RobotoMono-Regular.ttf"));
    text.setCharacterSize(50);
    text.setFillColor(Color::Yellow);
    text.setPosition(gameWidth * 0.3f - text.getLocalBounds().width * 0.5f,
                     gameHeight * 0.5f - text.getLocalBounds().height * 0.5f);
    text.setString("press space to start");

    btnStart = makeEntity();
    auto btn1Pos = Vector2f(menuView.getSize().x / 2.0f, menuView.getSize().y / 3.f);
    auto button1 = btnStart->addComponent<Button>(btn1Pos, "Play", sf::Color::Magenta, sf::Color::Yellow, sf::Color::Red);
    btnExit = makeEntity();
    auto btn2Pos = Vector2f(menuView.getSize().x / 2.0f, menuView.getSize().y / 2.f);
    auto button2 = btnExit->addComponent<Button>(btn2Pos, "Exit", sf::Color::Magenta, sf::Color::Yellow, sf::Color::Red);

    setLoaded(true);
}

void MenuScene::Update(const double &dt) {
    btnStart->update(dt);
    btnExit->update(dt);
    // if (Keyboard::isKeyPressed(Keyboard::Space)) {
    //   Engine::ChangeScene(&level1);
    // }
    if (btnStart->GetCompatibleComponent<Button>()[0]->isPressed()) {
        Engine::ChangeScene(&level1);
    }
    else if (btnExit->GetCompatibleComponent<Button>()[0]->isPressed())
    {
        Engine::GetWindow().close();
    }
    if (!sf::Mouse::isButtonPressed(Mouse::Button::Left))
        Button::_mouseState = BUTTON_IDLE;
    Scene::Update(dt);
}

void MenuScene::Render() {
    Renderer::queue(&background);
    // Engine::setView(menuView);
    Scene::Render();
}
