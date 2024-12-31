//
// Created by marky on 31/12/2024.
//

#include "cmp_enitity_health.h"

Health::Health(Entity *p)
    : Component(p), _health(10) {
}

void Health::update(double dt) {
    if (_health <= 0) {
        _parent->setForDelete();
    }
}

void Health::collisionDetected(std::string group) {
    std::cout << "Collision detected: " << group << std::endl;
    if (group == "player") {
        _health = _health - 1;
    } else if (group == "enemy") {
        _health = _health - 1;
    }
}

void Health::setHealth(int health) {
    _health = health;
}

int Health::getHealth() {
    return _health;
}


void Health::render() {

}
