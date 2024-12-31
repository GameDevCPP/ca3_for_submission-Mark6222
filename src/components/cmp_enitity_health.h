//
// Created by marky on 31/12/2024.
//

#ifndef CMP_ENITITY_HEALTH_H
#define CMP_ENITITY_HEALTH_H
#include <string>

#include "ecm.h"


class Health : public Component {
protected:
    int _health;
public:
    explicit Health(Entity *p);

    Health() = delete;
    int getHealth();

    void collisionDetected(std::string group);
    void setHealth(int health);
    void update(double dt) override;
    void render() override;
};



#endif //CMP_ENITITY_HEALTH_H
