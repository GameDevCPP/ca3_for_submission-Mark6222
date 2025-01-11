//
// Created by marky on 31/12/2024.
//

#ifndef CMP_COLLISION_H
#define CMP_COLLISION_H
#include "ecm.h"
#include "engine.h"
using namespace std;

class CollisionComponent : public Component {
protected:
    string _group;
    string _masks;
    double elapsedTime = 0;
    EntityManager _level;
public:
    explicit CollisionComponent(Entity *p);

    CollisionComponent() = delete;

    void update(double dt) override;

    void setCollision(string group, string masks, EntityManager level);

    void render() override {}
};


#endif //CMP_COLLISION_H
