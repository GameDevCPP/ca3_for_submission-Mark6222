#include "cmp_text.h"
#include <system_renderer.h>
#include <system_resources.h>

void TextComponent::update(double dt) {
  _text.setString(_string);
  _text.setPosition(_parent->getPosition());
}

void TextComponent::render() { Renderer::queue(&_text); }

TextComponent::TextComponent(Entity* const p, const std::string& str)
    : Component(p), _string(str) {
  _text.setString(_string);
  _font = Resources::get<sf::Font>("RobotoMono-Regular.ttf");
  _text.setFont(*_font);
}

void TextComponent::SetText(const std::string& str, sf::Color color, int fontSize) {
  _string = str;
  _text.setString(_string);
  _text.setFillColor(color);
  _text.setScale(fontSize, fontSize);
}
