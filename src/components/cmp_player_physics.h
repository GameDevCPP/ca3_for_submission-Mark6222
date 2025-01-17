#pragma once

#include "cmp_physics.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

class PlayerPhysicsComponent : public PhysicsComponent {
protected:
  b2Vec2 _size;
  sf::Vector2f _maxVelocity;
  bool _grounded;
  float _groundspeed;
  float _jumpheight;

  bool isGrounded() const;

public:
  void update(double dt) override;
  void init(const std::string &path);
  explicit PlayerPhysicsComponent(Entity* p, const sf::Vector2f& size);
  void quickJump();
  void knockback(const sf::Vector2f& direction, float force);
  PlayerPhysicsComponent() = delete;
};
