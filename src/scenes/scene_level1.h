#pragma once

#include "engine.h"
#include <SFML/Audio.hpp>

using namespace sf;

class Level1Scene : public Scene {
public:

  Level1Scene() = default;
  ~Level1Scene() override = default;
  void Load() override;
  float fireTime;
  std::shared_ptr<Entity> player;
  std::vector<std::shared_ptr<Entity>> drones;
  std::shared_ptr<Entity> kills;

  int fontSize = 1;

  void UnLoad() override;

  void Update(const double& dt) override;

  void Render() override;
};