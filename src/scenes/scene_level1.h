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
  std::shared_ptr<sf::SoundBuffer> soundShoot_buffer;
  std::shared_ptr<sf::Sound> soundShoot;
  std::shared_ptr<Entity> player;
  std::vector<std::shared_ptr<Entity>> drones;
  void UnLoad() override;

  void Update(const double& dt) override;

  void Render() override;
};