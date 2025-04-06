#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <iomanip>
#include <sstream>

class GameClock {
    sf::Clock clock;
    sf::Text text;
    sf::Time interval;
    sf::Font font;
    sf::Vector2f position;
    int hours;
    int minutes;

public:
    GameClock(sf::RenderWindow& window, sf::Vector2f& position);

    void updateText();

    void update();

    void drawing(sf::RenderWindow& window);

    void move(sf::Vector2f position);

    void stop();

    void reset();

    int hour();

    int minute();

    std::string get_clock_as_text();
};
