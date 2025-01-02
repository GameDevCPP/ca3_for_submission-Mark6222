#pragma once

#include "cmp_actor_movement.h"

class EnemyAIComponent : public ActorMovementComponent {
protected:
  sf::Vector2f _direction;
  double elapsedTime = 0;
  double timeToElapse = 0.0;
  std::shared_ptr<Entity> _player;
public:
  void update(double dt) override;
  void setPlayer(std::shared_ptr<Entity> player);
  explicit EnemyAIComponent(Entity* p);
  EnemyAIComponent() = delete;
};
