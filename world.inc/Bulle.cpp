#include "Bulle.h"

Bullet::Bullet(sf::Vector2f position) {
    shape.setSize(sf::Vector2f(5, 10));
    shape.setFillColor(sf::Color::White);
    shape.setPosition(position);
}

void Bullet::update() {
    shape.move(0, -speed);
}

bool Bullet::isOffScreen() const {
    return shape.getPosition().y + shape.getSize().y < 0;
}

sf::FloatRect Bullet::getBounds() const {
    return shape.getGlobalBounds();
}

void Bullet::drawing(sf::RenderWindow& window) const {
    window.draw(shape);
}