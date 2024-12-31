
#include "cmp_sprite.h"
#include "system_renderer.h"

using namespace std;


void SpriteComponent::setTexure(std::shared_ptr<sf::Texture> tex)
{
  _texture = tex;
  _sprite->setTexture(*_texture);
}
void SpriteComponent::setTexureRect(int left, int top, int width, int height)
{
    _sprite->setTextureRect(sf::IntRect(left, top, width, height));
}

SpriteComponent::SpriteComponent(Entity* p)
    : Component(p), _sprite(make_shared<sf::Sprite>()) {}

void SpriteComponent::update(double dt) {
  _sprite->setPosition(_parent->getPosition());
  _sprite->setRotation(sf::degrees(_parent->getRotation()));
}

void SpriteComponent::render() { Renderer::queue(_sprite.get()); }

void ShapeComponent::update(double dt) {
  _shape->setPosition(_parent->getPosition());
  _shape->setRotation(sf::degrees(_parent->getRotation()));
}

void ShapeComponent::render() { Renderer::queue(_shape.get()); }

sf::Shape& ShapeComponent::getShape() const { return *_shape; }

ShapeComponent::ShapeComponent(Entity* p)
    : Component(p), _shape(make_shared<sf::CircleShape>()) {}

sf::Sprite& SpriteComponent::getSprite() const { return *_sprite; }

AnimationComponent::AnimationComponent(Entity* p) : Component(p), frameCount(0), _left(0), _top(0), _width(0), _height(0), _frames(0), _speed(0)
{

}
void AnimationComponent::update(double dt) {
    auto s = _parent->GetCompatibleComponent<SpriteComponent>()[0];
    elapsedTime += dt;
    if (elapsedTime >= _speed) {
        s->setTexureRect(_left, _top, _width, _height);
        _left = _left + _width;
        elapsedTime = 0;
        frameCount++;
        if (frameCount >= _frames) {
            frameCount = 0;
            _left = 0;
        }
    }
}

void AnimationComponent::setAnimation(int width, int height, int frames, float speed) {
    _frames = frames;
    _height = height;
    _width = width / _frames;
    _speed = speed;
}
void AnimationComponent::render() {}