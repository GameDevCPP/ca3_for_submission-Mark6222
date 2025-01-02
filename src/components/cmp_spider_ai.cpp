//
// Created by marky on 01/01/2025.
//

#include "cmp_spider_ai.h"

#include <random>

#include "LevelSystem.h"

using namespace std;
using namespace sf;
int randomDirection;
void SpiderAi::update(double dt) {

    _direction = Vector2f(_player->getPosition().x - _parent->getPosition().x, _player->getPosition().y - _parent->getPosition().y);
    _direction = normalize(_direction);
    auto mov = _direction * (float)(dt * _speed);

    if (!validMove(_parent->getPosition() + mov)) {
        _direction = Vector2f((float)randomDirection, 0);
        _direction = normalize(_direction);
    }
    move(_direction * (float) (dt * _speed));
    ActorMovementComponent::update(dt);
}

void SpiderAi::setPlayer(std::shared_ptr<Entity> player) {
    _player = player;
    randomDirection = (rand() % 2 == 0) ? 1 : -1;
}

SpiderAi::SpiderAi(Entity *p) : ActorMovementComponent(p) {
    _direction = Vector2f(1.0f, 0);
    _speed = 100.0f;
}
