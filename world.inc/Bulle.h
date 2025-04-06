#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <string>

class Bullet {
    sf::RectangleShape shape;
    float speed = 5.0f;

public:
    Bullet(sf::Vector2f position);

    void update();

    bool isOffScreen() const;

    sf::FloatRect getBounds() const;

    void drawing(sf::RenderWindow& window) const;
};

