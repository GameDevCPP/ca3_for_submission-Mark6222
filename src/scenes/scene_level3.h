#pragma once

#include "engine.h"

namespace sf {
  class SoundBuffer;
  class Sound;
}

class Level3Scene : public Scene {
public:
  float playXStart;
  float fireTime;
  std::shared_ptr<sf::SoundBuffer> soundShoot_buffer;
  std::shared_ptr<sf::Sound> soundShoot;
  std::shared_ptr<Entity> player;
  std::vector<std::shared_ptr<Entity>> drones;  void Load() override;
  std::vector<std::shared_ptr<Entity>> spiders;
  std::shared_ptr<Entity> kills;
  void UnLoad() override;

  void Update(const double& dt) override;

  void Render() override;
};
