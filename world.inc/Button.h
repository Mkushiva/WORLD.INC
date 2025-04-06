#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
//#include <SFML/System.hpp>
//#include <SFML/Audio.hpp>
//#include <SFML/Network.hpp>
#include <iostream>
#include <string>
#include <random>
#include <functional>
#include <thread>
#include <future>
#include <memory>
#include <mutex>
#include <cmath>
#include <functional>
#include "Functions.h"


////////////////////////////////////////fonts
//REPLACED BY THE NEXT ONE
sf::Font fonts();

class ResourceManager {
public:
    static sf::Font& getFont();
};

////////////////////////////////////////uicomponent
class UIComponent {
public:
    virtual void drawing() = 0;
    virtual void handleEvent(const sf::Event& event) = 0;
    virtual ~UIComponent() {}
};
////////////////////////////////////////panel

class Panel : public UIComponent {
    std::vector<std::shared_ptr<UIComponent>> components;
    sf::RectangleShape background;
    sf::RenderWindow& window;
    sf::Sprite sprite;
    bool is_background_sprite;

public:
    Panel(sf::RenderWindow& window, const sf::Vector2f& position, const sf::Vector2f& size, sf::Color color);
    Panel(sf::RenderWindow& window, const sf::Vector2f& position, const sf::Vector2f& size, sf::Texture& texture);

    void addComponent(std::shared_ptr<UIComponent> component);

    virtual void drawing() override {
        window.draw(background);
        if(is_background_sprite){ 
            window.draw(sprite);
        }
        for (auto& comp : components) {
            comp->drawing();
        }
    }

    virtual void handleEvent(const sf::Event& event) override {
        for (auto& comp : components) {
            comp->handleEvent(event);
        }
    }

    void clear();

    void remove(std::shared_ptr<UIComponent> component);

};

////////////////////////////////////////button

class Buttons : public UIComponent {
    sf::RenderWindow& window;
    sf::Sprite buttonTexture;
    sf::Text text;
    sf::RectangleShape button;
    sf::Vector2f originalSize;
    sf::Vector2f originalPosition;
    float scale;
    std::function<void()> onClick;
    bool showButton = false, pressed = false, isText;


public:
    Buttons(sf::RenderWindow& window, int y, const std::string& str, std::function<void()> callback);
    Buttons(sf::RenderWindow& window, int y, sf::Font& font, const std::string& str, const int& char_size, const sf::Color& color, std::function<void()> callback);
    Buttons(sf::RenderWindow& window, sf::Vector2f position, sf::Font& font, const std::string& str, const int& char_size, const sf::Color& color, std::function<void()> callback);
    Buttons(sf::RenderWindow& window, sf::Texture &texture, sf::Vector2f position, sf::Vector2f size, std::function<void()> callback);
    Buttons(sf::RenderWindow& window, sf::Texture& texture, sf::Vector2f position, sf::Vector2f size, sf::Vector2f parent_panel_size, sf::Vector2f parent_panel_pos, std::function<void()> callback);

    bool isShown();
    virtual void handleEvent(const sf::Event& event);
    bool isPressed();
    virtual void drawing();
    void update_color(sf::Color color);
    sf::Color getColor();
    void dziabka(const sf::Color& color, int& thicc);

private:
    //DEFAULT VALUES
    int def_FontSize = 24;
    sf::Color def_FontColor = sf::Color::White;
};

//////////////////////////////////////////////////Text

class Texts : public UIComponent {
protected:
    sf::Text text;
    int def_FontSize = 24, y;
    sf::Color def_FontColor = sf::Color::White;
    sf::RenderWindow& window;
    std::function<void()> onClick;

public:
    Texts(sf::RenderWindow& window, int y, sf::Font& font, const std::string& str, const int& char_size, const sf::Color& color);
    Texts(sf::RenderWindow& window, int y, sf::Font& font, const std::string& str, const int& char_size, const sf::Color& color, std::function<void()> callback);
    void handleEvent(const sf::Event& event) {
        onClick();
    };
    virtual void drawing();
    void update(std::string& str);
};

//////////////////////////////////////////////////countdown
class Countdown : public Texts {
    int countdown;
    sf::Clock clock;
    float interval;
    bool finished = false;

public:
    Countdown(sf::RenderWindow& window, int y, sf::Font& font, int startValue, float intervalSec, const sf::Color& color);

    void update();

    bool isFinished() const;

    int get_n();

    void handleEvent(const sf::Event& event) override {}
};

//////////////////////////////////////////////////Obstacle

class Obstacle : public UIComponent {
    sf::RenderWindow& window;
    sf::Vector2f position;
    sf::Sprite sprite;
    float x_velocity = 1.0f, y_velocity = 1.5f;
    mutable std::mutex mtx; // Mutex do synchronizacji
    std::thread obstacle_thread; // W¹tek dla przeszkody
    bool running = true; // Flaga do zatrzymywania w¹tku

public:
    Obstacle(sf::Vector2f pos, sf::RenderWindow& win, std::shared_ptr<sf::Texture>& tex, sf::Vector2f velo);

    Obstacle(const Obstacle& other);

    Obstacle& operator=(const Obstacle& other) {
        if (this == &other)
            return *this;

        std::lock_guard<std::mutex> lock(other.mtx);
        position = other.position;
        sprite = other.sprite;
        x_velocity = other.x_velocity;
        y_velocity = other.y_velocity;

        return *this;
    }

    ~Obstacle();

    void start();

    void stop();

    void run();

    void movement();

    bool checkCollision(const sf::Sprite& other) const;

    bool checkCollision(const Obstacle& other) const;

    bool checkCollision(const sf::RectangleShape& other) const;

    void handleCollision(Obstacle& other);

    sf::Vector2f get_position() const;

    void drawing_const() const;

    void drawing();

    void handleEvent(const sf::Event& event);
};


class Heart : public UIComponent {
protected:
    sf::Vector2f position;
    sf::RenderWindow* window;
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Clock clock;
    bool collision = false;
    float invisibleDuration = 0.1f;
    int n;
    float speed = 200.0f; // Pixels per second
    sf::Vector2f velocity;

public:
    Heart();

    Heart(sf::RenderWindow& win, sf::Texture& texture, float scale);
    void setWindow(sf::RenderWindow& win);

    void setPos();

    void handleEvent(const sf::Event& event);

    virtual void updateVelocity() {
        velocity = sf::Vector2f(0.f, 0.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            velocity.x -= speed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            velocity.x += speed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            velocity.y -= speed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            velocity.y += speed;
        }
    }

    void update(float deltaTime);

    void checkCollision(std::vector<Obstacle>& obstacles);

    int get_n();

    void bounderies();

    void drawing();
};

class ModifiedHeart : public Heart {

public:

    ModifiedHeart() = default;

    ModifiedHeart(sf::RenderWindow& win, sf::Texture& texture, float scale);

    void setPos();

    void updateVelocity() override {
        velocity = sf::Vector2f(0.f, 0.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            velocity.x -= speed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            velocity.x += speed;
        }
    }

    void set_sprite(sf::Texture textur);

    sf::Sprite getSprite() const;
};

