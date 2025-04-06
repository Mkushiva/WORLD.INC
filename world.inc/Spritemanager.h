#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <string>


class SpriteManager {
    sf::Vector2f windowSize;
    std::string fileName;
    sf::Vector2f pos;
    sf::Vector2f size;
    sf::Texture texture;
    sf::Sprite sprite;
public:
    SpriteManager(sf::Vector2f windowSize, const std::string& fileName, sf::Vector2f pos, sf::Vector2f size);

    sf::Sprite getSprite() const;
};




