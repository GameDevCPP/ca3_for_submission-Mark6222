#include "cmp_player_physics.h"
#include "system_physics.h"
#include <LevelSystem.h>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/System.hpp>

#include "AudioManager.h"
// #include "../../cmake-build-debug/_deps/sfml-src/src/SFML/Window/CursorImpl.hpp"
#include "SFML/Audio/Music.hpp"
#include "../src/game.h"
using namespace std;
using namespace sf;
using namespace Physics;

bool _isChargingJump = false;
float _jumpChargeTime = 0.0f;
const float _maxChargeTime = 1.0f;
const float _minJumpForce = -2.0f;
const float _maxJumpForce = -12.0f;
bool landed = false;
sf::Music Jump;
sf::Music land;


bool PlayerPhysicsComponent::isGrounded() const {
    auto touch = getTouching();
    const auto &pos = _body->GetPosition();
    const float halfPlrHeigt = _size.y * .5f;
    const float halfPlrWidth = _size.x * .52f;
    b2WorldManifold manifold;
    for (const auto &contact: touch) {
        contact->GetWorldManifold(&manifold);
        const int numPoints = contact->GetManifold()->pointCount;
        bool onTop = numPoints > 0;
        // If all contacts are below the player.
        for (int j = 0; j < numPoints; j++) {
            onTop &= (manifold.points[j].y < pos.y - halfPlrHeigt);
        }
        if (onTop) {
            return true;
        }
    }

    return false;
}
void PlayerPhysicsComponent::quickJump() {
    setVelocity(Vector2f(getVelocity().x, 0.f));
    impulse(Vector2f(0, -12.0f));
}
void PlayerPhysicsComponent::update(double dt) {

    const auto pos = _parent->getPosition();
    _grounded = isGrounded();
    if (pos.y > ls::getHeight() * ls::getTileSize()) {
        teleport(ls::getTilePosition(ls::findTiles(ls::START)[0]));
    }

    if (Keyboard::isKeyPressed(Keyboard::A) ||
        Keyboard::isKeyPressed(Keyboard::D)) {
        if (Keyboard::isKeyPressed(Keyboard::D)) {
            if (getVelocity().x < _maxVelocity.x)
                impulse({(float) (dt * _groundspeed), 0});
            _parent->GetCompatibleComponent<SpriteComponent>()[0]->getSprite().setScale(1.f, 1.f);
        } else {
            if (getVelocity().x > -_maxVelocity.x)
                impulse({-(float) (dt * _groundspeed), 0});
            _parent->GetCompatibleComponent<SpriteComponent>()[0]->getSprite().setScale(-1.f, 1.f);
        }
    } else {
        dampen({0.9f, 1.0f});
    }


    // // Handle Jump
    // if (Keyboard::isKeyPressed(Keyboard::Up)) {
    //   _grounded = isGrounded();
    //   if (_grounded) {
    //     setVelocity(Vector2f(getVelocity().x, 0.f));
    //     teleport(Vector2f(pos.x, pos.y - 2.0f));
    //     impulse(Vector2f(0, -6.f));
    //   }
    // }
    auto s = _parent->GetCompatibleComponent<SpriteComponent>()[0];

    if (Keyboard::isKeyPressed(Keyboard::Space)) {
        dampen({0.9f, 1.0f});
        s->setTexureRect(26, 0, 27, 26);
        if (!_isChargingJump) {
            _isChargingJump = true;
            _jumpChargeTime = 0.0f;
        } else {
            _jumpChargeTime += dt;
            if (_jumpChargeTime > _maxChargeTime) {
                _jumpChargeTime = _maxChargeTime;
            }
        }
    } else if (_isChargingJump) {
        _grounded = isGrounded();
        if (_grounded) {
            float chargeRatio = _jumpChargeTime / _maxChargeTime;
            float jumpForce = _minJumpForce + chargeRatio * (_maxJumpForce - _minJumpForce);

            setVelocity(Vector2f(getVelocity().x, 0.f));
            impulse(Vector2f(0, jumpForce));
            AudioManager::playSound("jump");
        }

        _isChargingJump = false;
        _jumpChargeTime = 0.0f;
    } else {
        s->setTexureRect(0, 0, 27, 26);
    }
    //Are we in air?
    if (!_grounded) {
        // Check to see if we have landed yet
        _grounded = isGrounded();

        // disable friction while jumping
        setFriction(0.f);
    } else {
        setFriction(0.1f);
    }

    // Clamp velocity.
    auto v = getVelocity();
    v.x = copysign(min(abs(v.x), _maxVelocity.x), v.x);
    v.y = copysign(min(abs(v.y), _maxVelocity.y), v.y);
    if (!_isChargingJump) {
        setVelocity(v);
    }
    // Check for landing transition
    if (_grounded && !landed) { // Transition from air to ground
        AudioManager::playSound("landed");
    }

    landed = _grounded;
    PhysicsComponent::update(dt);
}

PlayerPhysicsComponent::PlayerPhysicsComponent(Entity *p,
                                               const Vector2f &size)
    : PhysicsComponent(p, true, size) {
    _size = sv2_to_bv2(size, true);
    _maxVelocity = Vector2f(200.f, 400.f);
    _groundspeed = 30.f;
    _grounded = false;
    _body->SetSleepingAllowed(false);
    _body->SetFixedRotation(true);
    //Bullet items have higher-res collision detection
    _body->SetBullet(true);
}
