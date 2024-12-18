#pragma once

#include "engine.h"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/Text.hpp"
#include "system_renderer.h"
#include "system_resources.h"

using namespace std;
using namespace sf;

class MenuScene : public Scene {
public:
  MenuScene() = default;

  ~MenuScene() override = default;

  sf::View menuView;

  std::shared_ptr<Entity> btnExit;
  std::shared_ptr<Entity> btnStart;
  sf::Sprite background;

  Font font;
  Text text;

  void Load() override;

  void Render() override;

  void Update(const double &dt) override;
};