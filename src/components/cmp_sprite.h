#pragma once

#include "ecm.h"
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Sprite.hpp>

class SpriteComponent : public Component {
protected:
  std::shared_ptr<sf::Sprite> _sprite;
  std::shared_ptr<sf::Texture> _texture;
public:
  SpriteComponent() = delete;

  explicit SpriteComponent(Entity* p);
  void update(double dt) override;
  void render() override;

  sf::Sprite& getSprite() const;


  void setTexure(std::shared_ptr<sf::Texture> tex);
  void setTexureRect(int left, int top, int width, int height);

};

class ShapeComponent : public Component {
protected:
  std::shared_ptr<sf::Shape> _shape;
  // sf::Shape _shape;

public:
  ShapeComponent() = delete;

  explicit ShapeComponent(Entity* p);

  void update(double dt) override;
  void render() override;
  sf::Shape& getShape() const;
  template <typename T, typename... Targs> void setShape(Targs... params) {
    _shape.reset(new T(params...));
  }
};

class AnimationComponent : public Component {
protected:
  int _width, _height, _frames;
  float _speed;
  double elapsedTime = 0;
  int _left, _top;
  int frameCount;

public:

  AnimationComponent() = delete;

  explicit AnimationComponent(Entity* p);

  void update(double dt) override;
  void setAnimation(int width, int height, int frames, float speed);
  void render();
};