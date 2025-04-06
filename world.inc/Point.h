#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <random>
#include <string>

#include "Button.h"
#include "Functions.h"


class Points {
    int points, max_point;
    bool debuff;
    sf::Clock clock, clock1;
    float interval;
    sf::Time timeBuffer;

public:
    Points();

    void add(int n);

    void debuff_animation(Buttons& icon, Texts& points_string);

    std::string get_minigame_points();

    std::string get_overall_points();

    bool get_debuff();

    void set_no_debuff();

    void set_yes_debuff();
};

