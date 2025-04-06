#include "Point.h"

Points::Points() {
    points = 600;
    max_point = 100;
    debuff = false;
    clock.restart();
    clock1.restart();
    timeBuffer = sf::Time::Zero;  // Inicjalizacja bufora czasu
}

void Points::add(int n) {
    max_point = max_point - 15 * n;
    points += max_point;
}

void Points::debuff_animation(Buttons& icon, Texts& points_string) {
    if (debuff) {
        // Dodaj czas od ostatniego wywo³ania do bufora
        timeBuffer += clock.restart();

        // Co 500 ms prze³¹czaj kolor ikony
        if (timeBuffer >= sf::milliseconds(500)) {
            if (icon.getColor() == sf::Color::Red) {
                icon.update_color(sf::Color::White);
            }
            else {
                icon.update_color(sf::Color::Red);
            }
            timeBuffer -= sf::milliseconds(500);  // Odejmij 500 ms od bufora
        }

        // Odejmuj punkty co 1 sekundê
        if (clock1.getElapsedTime().asMilliseconds()>30.0f) {
            points -= 1;
            std::string pom = "Points: " + get_overall_points() + " m";
            points_string.update(pom);
            clock1.restart();
        }
    }
    else {
        icon.update_color(sf::Color::White);
        clock.restart();
        timeBuffer = sf::Time::Zero;
    }
}

std::string Points::get_minigame_points() {
    int pom = max_point;
    max_point = 100;
    return std::to_string(pom);
}

std::string Points::get_overall_points() {
    return std::to_string(points);
}

bool Points::get_debuff() {
    return debuff;
}

void Points::set_no_debuff() {
    debuff = false;
}

void Points::set_yes_debuff() {
    debuff = true;
}