#include "cmp_enemy_ai.h"

#include <random>

#include "cmp_shooting.h"
using namespace std;
using namespace sf;

void EnemyAIComponent::update(double dt) {
  auto mov = _direction * (float)(dt * _speed);
  if (_direction == Vector2f(-1.0f, 0)) {
    mov.x += _direction.x * 0.f;
  }else {
    mov.x += _direction.x * 45.f;
  }
  if (!validMove(_parent->getPosition() + mov)) {
    _direction = -_direction;
  }
  elapsedTime += dt;
  if (elapsedTime >= timeToElapse) {
    if (timeToElapse != 0.0) _parent->GetCompatibleComponent<ShootingComponent>()[0]->EnemyFire(_player->getPosition());
    static std::random_device rd;
    static std::mt19937 gen(rd());
    uniform_real_distribution<> dis(6.0, 15.0);
    timeToElapse = dis(gen);
    elapsedTime = 0.0;
  }
  move(_direction * (float)(dt * _speed));
  ActorMovementComponent::update(dt);
}

void EnemyAIComponent::setPlayer(std::shared_ptr<Entity> player) {
  _player = player;
}
EnemyAIComponent::EnemyAIComponent(Entity* p) : ActorMovementComponent(p) {
  _direction = Vector2f(1.0f, 0);
  _speed = 100.0f;
}
