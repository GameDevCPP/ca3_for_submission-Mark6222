//
// Created by marky on 01/01/2025.
//

#ifndef CMP_SPIDER_AI_H
#define CMP_SPIDER_AI_H
#include "cmp_actor_movement.h"


class SpiderAi  : public ActorMovementComponent {
protected:
    sf::Vector2f _direction;
    std::shared_ptr<Entity> _player;
public:
    void update(double dt) override;
    void setPlayer(std::shared_ptr<Entity> player);
    explicit SpiderAi(Entity* p);
    SpiderAi() = delete;
};



#endif //CMP_SPIDER_AI_H
