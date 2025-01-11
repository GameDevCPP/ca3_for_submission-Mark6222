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

void Health::collisionDetected(std::string group, std::string mask) {
    std::cout << "Collision detected group: " << group << " mask: " << mask << " _group: " << _group << std::endl;
    if (mask == "spider" && group == "player" && _group == "player") {
        _health = _health - 1;
    }
    if (mask == "enemy" && group == "player") {
        _health = _health - 1;
    }
    else if (mask == "bullet" && group == "player" && _group == "spider") {
        _health = _health - 1;
    }
    else if (mask == "bullet" && group == "player" && _group != "spider" && _group == "enemy") {
        _health = _health - 1;
    }
    else if (mask == "bullet" && group == "enemy" && _group != "spider" && _group == "player") {
        _health = _health - 1;
    }
    else if (mask == "bullet" && group == "player" && _group != "spider" && _group != "player") {
        _health = _health - 1;
    }
}

void Health::setHealth(int health, std::string group) {
    _health = health;
    _group = group;
}

int Health::getHealth() {
    return _health;
}


void Health::render() {

}
