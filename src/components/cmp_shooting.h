//Shooting component header file
#pragma once

#include "ecm.h"
#include "cmp_actor_movement.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
using namespace std;
class Bullet : sf::Sprite {
protected:
	void _update(const double dt);
	bool isVisible;
	sf::Vector2f mousePos;
	float angle;
	int _damage;
	std::shared_ptr<sf::SoundBuffer> soundHit_buffer;
	std::shared_ptr<sf::Sound> soundHit;
	string _groupType;
	string _mask;

	EntityManager level;
	double elapsedTime = 0.0;

public:
	static void update(const double dt);
	static void render();
	static void fire(const sf::Vector2f& pos);
	static void enemyFire(const sf::Vector2f& pos, const sf::Vector2f& playerPos);
	static void init();
	static void setAngle(float a, Bullet& b);
	void setGroup(string group, string mask);
    sf::FloatRect getBounds() const;
	~Bullet() = default;
	Bullet();
};

class ShootingComponent : public ActorMovementComponent {
protected:

	std::vector<Bullet> bullets;
	unsigned int bulletCount = 0;
	float angleshot = 0.0f;

public:
	ShootingComponent() = delete;

	explicit ShootingComponent(Entity* p);
	void Fire();
	void setLevel(EntityManager e);
	std::vector<Bullet> getBullets();
	void EnemyFire(const sf::Vector2f& playerPos);
	void update(double dt) override;
	void render() override;
};
