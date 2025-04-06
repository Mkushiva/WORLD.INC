#include "Gameclock.h"

GameClock::GameClock(sf::RenderWindow& window, sf::Vector2f& position) {
    interval = sf::seconds(5);
    hours = 22;
    minutes = 0;

    if (!font.loadFromFile("fonts/ARCHITXT.ttf")) {
        std::cerr << "Couldn't load font!\n";
    }
    float scaler = window.getSize().x / 1200.0f;
    float sizeOfText = 36 * scaler;
    text.setFont(font);
    text.setCharacterSize(sizeOfText);
    text.setFillColor(sf::Color::White);
    text.setPosition(position);
    updateText();
}

void GameClock::updateText() {
    std::ostringstream ss;
    ss << std::setw(2) << std::setfill('0') << hours << ":"
        << std::setw(2) << std::setfill('0') << minutes;
    text.setString(ss.str());
}

void GameClock::update() {
    if (interval == sf::Time::Zero) {
        return;
    }

    if (clock.getElapsedTime() >= interval) {
        clock.restart();
        minutes += 5;
        if (minutes >= 60) {
            minutes = 0;
            hours = (hours + 1) % 24;
        }
        updateText();
    }
}

void GameClock::drawing(sf::RenderWindow& window) {
    window.draw(text);
}

void GameClock::move(sf::Vector2f position) {
    text.setPosition(position);
}

void GameClock::stop() {
    interval = sf::Time::Zero;
}

void GameClock::reset() {
    hours = 22;
    minutes = 0;
    clock.restart();
    interval = sf::seconds(5);
    updateText();
}

int GameClock::hour() {
    return hours;
}

int GameClock::minute() {
    return minutes;

}

std::string GameClock::get_clock_as_text(){
    return text.getString();
}
