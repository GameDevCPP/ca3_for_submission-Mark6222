// Shooting component C++ file
#include "cmp_shooting.h"
#include "../game.h"
#include "cmp_actor_movement.h"
#include "cmp_sprite.h"
#include <SFML/Graphics.hpp>

//#include "cmp_monster.h"
#include "cmp_enitity_health.h"
#include "system_renderer.h"
#include "engine.h"

using namespace std;
using namespace sf;

// ShootingComponent Variables
unsigned int bulletCount;
vector<Bullet> bullets(256);

float tempfloat = 0.f;
shared_ptr<float> angleshot = make_unique<float>(tempfloat);

// Bullet Variables
bool isVisible = false;
float angle;
Vector2f mousePos;

Texture temp2;
auto spriteTexture = make_shared<Texture>(temp2);

ShootingComponent::ShootingComponent(Entity* p) : ActorMovementComponent(p) {
	Bullet::init();
}

void ShootingComponent::update(double dt) {
	Bullet::update(dt);
}

void ShootingComponent::render() {
	Bullet::render();
}

void ShootingComponent::Fire() {
	auto spriteSize = _parent->GetCompatibleComponent<SpriteComponent>()[0]->getSprite().getLocalBounds();
	Vector2f spriteCenter = Vector2f(spriteSize.width * 0.5, spriteSize.height * 0.5);
	// Firing the bullets
	Bullet::fire(_parent->getPosition());
}

void ShootingComponent::EnemyFire(const Vector2f& playerPos) {
	Bullet::enemyFire(_parent->getPosition(), playerPos);
}

std::vector<Bullet> ShootingComponent::getBullets() {
	return bullets;
}

Bullet::Bullet()
{
	_damage = 34;
}

sf::FloatRect Bullet::getBounds() const {
	return this->getLocalBounds();
}

void Bullet::init() {
	spriteTexture = Resources::get<Texture>("fire.png");
	for (auto& b : bullets) {
		b.setPosition(Vector2f(0, 0));
		b.setTexture(*spriteTexture);
		b.setScale(0.5f, 0.5f);
		b.setTextureRect(IntRect(0, 0, 50, 43));
		b.setOrigin(Vector2f(27 / 2.0f, 26 / 2.0f));
		b.setAngle(0.f, b);
		b.mousePos = Vector2f(0, 0);
		b.isVisible = false;
	}
}

void Bullet::setAngle(float a, Bullet& b) {
	b.angle = a;
}

void Bullet::update(const double dt) {
	for (auto& b : bullets) {
		b._update(dt);
	}
}

void Bullet::render() {
	for (auto& b : bullets) {
		if (b.isVisible) {
			Renderer::queue(&b);
		}
	}
}

void Bullet::fire(const Vector2f& pos) {
	RenderWindow& window = Engine::GetWindow();

	auto mousePos = window.mapPixelToCoords(Mouse::getPosition(window));

	bulletCount++;
	bulletCount = bulletCount % 256;
	bullets[bulletCount].setPosition(pos);
	bullets[bulletCount].isVisible = true;
	auto activeScene = Engine::_activeScene;
	if (level1.isLoaded()) {
		bullets[bulletCount].setGroup("player", "enemy");

	}else if (level2.isLoaded()) {
		bullets[bulletCount].setGroup("player", "spider");

	}else if (level3.isLoaded()) {
		bullets[bulletCount].setGroup("player", "enemy");
	}
	// Sets the angle of the bullet.
	*angleshot = atan2(mousePos.y - bullets[bulletCount].getPosition().y, mousePos.x - bullets[bulletCount].getPosition().x);
	bullets[bulletCount].setAngle(*angleshot, bullets[bulletCount]);
	*angleshot = atan2(mousePos.y - bullets[bulletCount].getPosition().y, mousePos.x - bullets[bulletCount].getPosition().x);

	// Convert the angle from radians to degrees
	float angleInDegrees = *angleshot * (180.0f / 3.14159265f);

	// Set the rotation of the bullet
	bullets[bulletCount].setRotation(angleInDegrees);

}
void Bullet::setGroup(string group, string mask) {
	_groupType = group;
	_mask = mask;
}
void Bullet::enemyFire(const sf::Vector2f& pos, const sf::Vector2f& playerPos) {
	RenderWindow& window = Engine::GetWindow();

	auto mousePos = window.mapPixelToCoords(Mouse::getPosition(window));

	bulletCount++;
	bulletCount = bulletCount % 256;
	bullets[bulletCount].setPosition(pos);
	bullets[bulletCount].isVisible = true;
	bullets[bulletCount].setGroup("enemy", "player");

	*angleshot = atan2(playerPos.y - bullets[bulletCount].getPosition().y, playerPos.x - bullets[bulletCount].getPosition().x);
	bullets[bulletCount].setAngle(*angleshot, bullets[bulletCount]);
	*angleshot = atan2(playerPos.y - bullets[bulletCount].getPosition().y, playerPos.x - bullets[bulletCount].getPosition().x);

	float angleInDegrees = *angleshot * (180.0f / 3.14159265f);

	bullets[bulletCount].setRotation(angleInDegrees);

}


void Bullet::_update(const double dt) {
	elapsedTime += dt;
	RenderWindow& window = Engine::GetWindow();
	const View view = window.getView();

	// If bullet is out of bounds. remove/return;
	if (getPosition().x < view.getCenter().x - 100 - view.getSize().x * 0.5 || getPosition().x > view.getCenter().x + 100 + view.getSize().x * 0.5
		|| getPosition().y < view.getCenter().y - 100 - view.getSize().y * 0.5 || getPosition().y > view.getCenter().y + 100 + view.getSize().y * 0.5)
	{
		this->isVisible = false;
		return;
	}
	else {
		this->move(cos(this->angle) * 200.f * dt, 0);
		this->move(0, sin(this->angle) * 200.f * dt);
	}

	auto ecm = Engine::_activeScene->getEcm();

	auto enemies = ecm.find(_mask);
	auto boundingBox = getGlobalBounds();

	for (auto enemy : enemies)
	{
		auto sprite = enemy->GetCompatibleComponent<SpriteComponent>()[0]->getSprite();
		auto spriteBounds = sprite.getGlobalBounds();
		spriteBounds.top += 40;
		spriteBounds.left += 40;
		spriteBounds.width -= 70;
		spriteBounds.height -= 70;
		if (enemy->isAlive() && spriteBounds.intersects(boundingBox))
		{
			this->isVisible = false;
			setPosition(-100, -100);
			// // Hit Sound
			// soundHit_buffer = Resources::get<SoundBuffer>("Hit.wav");
			// soundHit = make_shared<Sound>(*soundHit_buffer);
			// soundHit->setVolume(volume);
			// soundHit->play();
			if (elapsedTime > 15.0) {
				enemy->GetCompatibleComponent<Health>()[0]->collisionDetected(_groupType, "bullet");
			}
		}
	}
}


