//
// Created by marky on 31/12/2024.
//

#include "cmp_collision.h"

#include "cmp_enitity_health.h"
#include "cmp_shooting.h"
#include "cmp_sprite.h"
#include "../game.h"

class EntityHealth;

void CollisionComponent::update(double dt) {
    elapsedTime += dt;
    auto entities = _level.find(_masks);
    auto boundingBox = _parent->GetCompatibleComponent<SpriteComponent>()[0]->getSprite().getGlobalBounds();
    auto eh = _parent->GetCompatibleComponent<Health>()[0];
    for (auto entity : entities) {
        auto sprite = entity->GetCompatibleComponent<SpriteComponent>()[0]->getSprite();
        auto spriteBounds = sprite.getGlobalBounds();

        if (spriteBounds.intersects(boundingBox)) {
            if (elapsedTime >= 1.0) {
                eh->collisionDetected(_group);
                elapsedTime = 0.0;
            }
        }
    }
}


void CollisionComponent::setCollision(string group, string masks, EntityManager level){
    _group = group;
    _masks = masks;
    _level = level;
}

CollisionComponent::CollisionComponent(Entity* p)
    : Component(p){}