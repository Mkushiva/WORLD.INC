#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
//#include <SFML/System.hpp>
//#include <SFML/Audio.hpp>
//#include <SFML/Network.hpp>
#include <iostream>
//#include <cstdlib>
#include <windows.h>
#include <ctime> // For time()
#include <random>
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <regex>
#include <functional>
#include <thread>
#include <future>
#include <memory>
#include <mutex>
#include <cmath>
#include <ranges>
#include <algorithm>



#include "Functions.h"
#include "Button.h"
#include "Gameclock.h"
#include "Bulle.h"
#include "Point.h"
#include "Spritemanager.h"

namespace fs = std::filesystem;
void minigame2(sf::RenderWindow& window, GameClock& gameclock, Points& point, int framelimit) { 

	float angle = 30;
	sf::Vector2f size(600, 600);
	sf::Vector2f position(0, 0);
	sf::Font font = fonts();
	sf::RenderWindow minigame_window(sf::VideoMode(size.x, size.y), "Cyberattack", sf::Style::Titlebar);
	minigame_window.setFramerateLimit(framelimit);

	Panel minigame_panel(window, sf::Vector2f(0, 0), size, sf::Color::Transparent);
	minigame_panel.addComponent(std::make_shared<Texts>(minigame_window, 20, font, "Shoot those viruses down!", 20, sf::Color::White));
	SpriteManager background(size, "minigame_background", sf::Vector2f(-1, -1), sf::Vector2f(-1, 900));
	auto texture = std::make_shared<sf::Texture>();
	auto texture1 = std::make_shared<sf::Texture>();
	auto texture2 = std::make_shared<sf::Texture>();

	if (!texture->loadFromFile("footpics/new_wirus.png") ||
		!texture1->loadFromFile("footpics/new_wirus.png") ||
		!texture2->loadFromFile("footpics/new_wirus.png")) {
		std::cerr << "Couldn't load the image!\n";
	}
	sf::Texture aship;
	if (!aship.loadFromFile("footpics/starship_red.png")) {
		std::cerr << "Couldn't load the image!\n";
	}
	ModifiedHeart heart(minigame_window, aship, 2.0f);
	std::vector<Obstacle> obstacles;

	int rows = 3, cols = 4;
	sf::Vector2f obstacleSize(texture->getSize());
	obstacleSize.x *= 0.7f;
	obstacleSize.y *= 0.7f;

	float horizontalSpacing = obstacleSize.x + 70.0f; // Odstęp między przeszkodami w poziomie
	float verticalSpacing = obstacleSize.y + 40.0f;   // Odstęp między przeszkodami w pionie

	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			sf::Vector2f pos(100.0f + j * horizontalSpacing, 50.0f + i * verticalSpacing);
			std::shared_ptr<sf::Texture> tex;
			if (i % 3 == 0)
				tex = texture;
			else if (i % 3 == 1)
				tex = texture1;
			else
				tex = texture2;

			obstacles.emplace_back(pos, minigame_window, tex, sf::Vector2f(1.0f, 0.0f));
		}
	}

	sf::RectangleShape bullet(sf::Vector2f(5, 10));
	bullet.setFillColor(sf::Color::White);
	std::vector<sf::RectangleShape> bullets;
	sf::Clock bulletClock;
	sf::Time bulletCooldown = sf::seconds(0.5f);
	Countdown count_down(minigame_window, 70, font, 16, 1.0f, sf::Color::White);
	Countdown game_timer(minigame_window, 70, font, 16, 1.0f, sf::Color::White);

	sf::Clock frameClock;
	sf::Clock startClock;
	int countdown = 5; // Start countdown from 5
	bool spacePressed = false;
	bool gameStarted = false;

	while (minigame_window.isOpen()) {
		float deltaTime = frameClock.restart().asSeconds();

		if (!gameStarted) {
			int elapsed = static_cast<int>(startClock.getElapsedTime().asSeconds());
			countdown = 5 - elapsed;
			if (countdown <= 0) {
				gameStarted = true;
				startClock.restart();
			}
		}
		else {
			game_timer.update();
			sf::Event event;
			while (minigame_window.pollEvent(event)) {
				if (event.type == sf::Event::Closed) {
					minigame_window.close();
				}
				heart.handleEvent(event);
				if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
					spacePressed = true;
				}

				if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Space) {
					spacePressed = false;
				}
			}
		}

		if (gameStarted) {
			if (spacePressed && bulletClock.getElapsedTime() >= bulletCooldown) {
				bullet.setPosition(heart.getSprite().getPosition().x + heart.getSprite().getGlobalBounds().width / 2, heart.getSprite().getPosition().y);
				bullets.push_back(bullet);
				bulletClock.restart();
			}

			heart.update(deltaTime);

			for (auto& bullet : bullets) {
				bullet.move(0, -5);
			}

			bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [&](const sf::RectangleShape& b) {
				return b.getPosition().y < 0;
				}), bullets.end());

			for (auto& obstacle : obstacles) {
				obstacle.movement();
			}

			for (auto it = bullets.begin(); it != bullets.end();) {
				bool hit = false;
				for (auto ob_it = obstacles.begin(); ob_it != obstacles.end() && !hit;) {
					if (ob_it->checkCollision(*it)) {
						ob_it = obstacles.erase(ob_it);
						it = bullets.erase(it);
						hit = true;
					}
					else {
						++ob_it;
					}
				}
				if (!hit) {
					++it;
				}
			}
		}

		minigame_window.clear();
		minigame_window.draw(background.getSprite());
		minigame_panel.drawing();
		heart.drawing();
		for (const auto& bullet : bullets) {
			minigame_window.draw(bullet);
		}
		for (const auto& obstacle : obstacles) {
			obstacle.drawing_const();
		}
		if (gameStarted) {
			game_timer.drawing();
		}
		else {
			Texts countdownText(minigame_window, 200, font, std::to_string(countdown), 100, sf::Color::White);
			countdownText.drawing();
		}
		minigame_window.display();

		if (obstacles.empty() || game_timer.isFinished()) {
			Texts points_string(minigame_window, 150, font, "", 20, sf::Color::White);
			std::string points_text = "Your points: ";
			point.add(game_timer.get_n());
			std::string pom = point.get_minigame_points();
			std::string your_points = points_text + pom;
			points_string.update(your_points);

			std::string choosing_stamp;
			Panel minigame_ended(minigame_window, sf::Vector2f(0, 0), size, sf::Color::Transparent);
			if (std::stoi(pom) <= 0) {
				choosing_stamp = "fail";
				minigame_ended.addComponent(std::make_shared<Texts>(minigame_window, 50, font, "You successfully failed the minigame!", 24, sf::Color::Red));
				minigame_ended.addComponent(std::make_shared<Texts>(minigame_window, 80, font, "You bring dishonor to our company!", 20, sf::Color::White));
				minigame_ended.addComponent(std::make_shared<Buttons>(minigame_window, 300, font, "I am a crap!", 30, sf::Color::Red, [&minigame_window]() {
					minigame_window.close();
					}));
			}
			else {
				choosing_stamp = "pass";
				minigame_ended.addComponent(std::make_shared<Texts>(minigame_window, 50, font, "You successfully beat the minigame!", 25, sf::Color::Green));
				minigame_ended.addComponent(std::make_shared<Texts>(minigame_window, 80, font, "You bring honor to our company!", 20, sf::Color::White));
				minigame_ended.addComponent(std::make_shared<Buttons>(minigame_window, 300, font, "I am ready to bring some more!", 30, sf::Color::White, [&minigame_window]() {
					minigame_window.close();
					}));
			}
			SpriteManager stamp(size, choosing_stamp, sf::Vector2f(60, 60), sf::Vector2f(-1, 100));
			sf::Sprite stamp_sprite = stamp.getSprite();
			stamp_sprite.setRotation(angle);

			while (minigame_window.isOpen()) {
				sf::Event event1;
				while (minigame_window.pollEvent(event1)) {
					minigame_ended.handleEvent(event1);
				}
				minigame_window.clear(sf::Color::Black);
				minigame_ended.drawing();
				points_string.drawing();
				minigame_window.draw(stamp_sprite);
				minigame_window.display();
			}

		}																																				  5
			; gameclock.update();
	}
}

void minigame(sf::RenderWindow& window, GameClock& gameclock, Points& point, int &framelimit, int &hardness) {
	std::random_device rd;  
	std::mt19937 gen(rd());  
	std::uniform_real_distribution<float> pos_dis(100.0f, 550.0f); 
	std::uniform_real_distribution<float> vel_dis(-3.0f, 3.0f);

	float angle = 30;
	sf::Vector2f size(600, 600);
	sf::RenderWindow minigame_window(sf::VideoMode(size.x, size.y), "Epidemy", sf::Style::Titlebar);
	minigame_window.setFramerateLimit(framelimit);

	SpriteManager background(size, "minigame_background", sf::Vector2f(-1, -1), sf::Vector2f(-1, 900));

	std::vector<std::shared_ptr<sf::Texture>> textures;


	textures.push_back(std::make_shared<sf::Texture>());

	int i = 1;
	for (auto texture : textures) {
		if (!texture->loadFromFile("footpics/new_wirus.png")) {
			std::cerr << "Couldn't load the images!\n";
			return;
		}
	}

	Panel minigame_panel(window, sf::Vector2f(0, 0), size, sf::Color::Transparent);
	sf::Font font = fonts();
	minigame_panel.addComponent(std::make_shared<Texts>(minigame_window, 50, font, "Dodge those viruses!", 20, sf::Color::White));

	std::vector<Obstacle> obstacles;
	for (int i = 0; i < hardness; ++i) {
		sf::Vector2f random_position;
		bool valid_position;

		do {
			valid_position = true;
			random_position = sf::Vector2f(pos_dis(gen), pos_dis(gen));
			for (const auto& obstacle : obstacles) {
				if (std::hypot(random_position.x - obstacle.get_position().x, random_position.y - obstacle.get_position().y) < 50) {
					valid_position = false;
					break;
				}
			}
		} while (!valid_position);

		sf::Vector2f random_velocity(vel_dis(gen) + 0.1 * i, vel_dis(gen) + 0.1 * i); // Losowanie prędkości
		obstacles.emplace_back(random_position, minigame_window, textures[0], random_velocity);
	}
	sf::Texture heart_texture;
	if (!heart_texture.loadFromFile("footpics/heart.png")) {
		std::cerr << "Couldn't load the image!\n";
	}
	Heart heart(minigame_window, heart_texture, 0.1f);
	Countdown countdown(minigame_window, 70, font, 5, 1.0f, sf::Color::White);
	Countdown game_timer(minigame_window, 70, font, 21, 1.0f, sf::Color::White);

	Panel minigame_ended(minigame_window, sf::Vector2f(0, 0), size, sf::Color::Transparent);
	Texts points_string(minigame_window, 150, font, "", 20, sf::Color::White);
	std::string points_text = "Your points: ";

	sf::Clock frameClock;

	while (minigame_window.isOpen()) {
		float deltaTime = frameClock.restart().asSeconds();

		sf::Event event;
		while (minigame_window.pollEvent(event)) {
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
				minigame_window.close();
			}
			heart.handleEvent(event);
		}

		heart.update(deltaTime);

		countdown.update();
		if (countdown.isFinished()) {
			game_timer.update();
			heart.checkCollision(obstacles);
			for (auto& obstacle : obstacles) {
				obstacle.movement();
			}
			for (size_t i = 0; i < obstacles.size(); ++i) {
				for (size_t j = i + 1; j < obstacles.size(); ++j) {
					obstacles[i].handleCollision(obstacles[j]);
				}
			}
		}

		minigame_window.clear();
		minigame_window.draw(background.getSprite());
		minigame_panel.drawing();

		for (auto& obstacle : obstacles) {
			obstacle.drawing();
		}
		heart.drawing();

		if (!countdown.isFinished()) {
			countdown.drawing();
		}
		else {
			game_timer.drawing();
			if (game_timer.isFinished()) {
				point.add(heart.get_n());

				std::string pom = point.get_minigame_points();
				std::string your_points = points_text + pom;
				points_string.update(your_points);

				std::string choosing_stamp;
				if (std::stoi(pom) < 0) {
					choosing_stamp = "fail";
					minigame_ended.addComponent(std::make_shared<Texts>(minigame_window, 50, font, "You successfully failed the minigame!", 24, sf::Color::Red));
					minigame_ended.addComponent(std::make_shared<Texts>(minigame_window, 80, font, "You bring dishonor to our company!", 20, sf::Color::White));
					minigame_ended.addComponent(std::make_shared<Buttons>(minigame_window, 300, font, "I am a crap!", 30, sf::Color::Red, [&minigame_window]() {
						minigame_window.close();
						}));
				}
				else {
					choosing_stamp = "pass";
					minigame_ended.addComponent(std::make_shared<Texts>(minigame_window, 50, font, "You successfully beat the minigame!", 25, sf::Color::Green));
					minigame_ended.addComponent(std::make_shared<Texts>(minigame_window, 80, font, "You bring honor to our company!", 20, sf::Color::White));
					minigame_ended.addComponent(std::make_shared<Buttons>(minigame_window, 300, font, "I am ready to bring some more!", 30, sf::Color::White, [&minigame_window]() {
						minigame_window.close();
						}));
				}

				SpriteManager stamp(size, choosing_stamp, sf::Vector2f(60, 60), sf::Vector2f(-1, 100));
				sf::Sprite stamp_sprite = stamp.getSprite();
				stamp_sprite.setRotation(angle);

				while (minigame_window.isOpen()) {
					sf::Event event1;
					while (minigame_window.pollEvent(event1)) {
						minigame_ended.handleEvent(event1);
					}
					minigame_window.clear(sf::Color::Black);
					minigame_window.draw(stamp_sprite);
					minigame_ended.drawing();
					points_string.drawing();
					minigame_window.display();
				}
			}
		}

		gameclock.update();
		minigame_window.display();
	}
}

std::string generatestring() {
	const std::string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	const int length = 8;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, characters.size() - 1);

	std::string random;
	for (int i = 0; i < length; ++i) {
		random += characters[dis(gen)];
	}
	return random;
}

void dontchangemywife(sf::RenderWindow& window, Points& points) {
	sf::Vector2u CurrRes = window.getSize();
	sf::Vector2f size(700, 600);
	sf::Vector2f position(0, 0);
	sf::Font font = fonts();
	sf::RenderWindow wifi_window(sf::VideoMode(size.x, size.y), "Wifi", sf::Style::Titlebar);
	wifi_window.setFramerateLimit(120);

	bool finished = false;
	Panel wifi(wifi_window, sf::Vector2f(210, 20), sf::Vector2f(CurrRes.x * (3 / 2), CurrRes.y / 2), sf::Color::Transparent);
	std::string random_string = generatestring();
	std::regex random(random_string);

	wifi.addComponent(std::make_shared <Texts>(wifi_window, 80, font, "Enter the password: " + random_string, 30, sf::Color::White));
	std::shared_ptr<Texts> input_text_component = std::make_shared<Texts>(wifi_window, 120, font, "Your password: ", 30, sf::Color::White);
	wifi.addComponent(input_text_component);

	SpriteManager keypad(size, "keypad", sf::Vector2f(-1, 50), sf::Vector2f(-1, 200));
	std::string input_string;

	while (wifi_window.isOpen()) {
		sf::Event event;
		while (wifi_window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				wifi_window.close();
			}
			if (event.type == sf::Event::TextEntered) {
				if (event.text.unicode < 128 && event.text.unicode != 8) { 
					input_string += static_cast<char>(event.text.unicode);
				}
				else if (event.text.unicode == 8 && !input_string.empty()) { 
					input_string.pop_back();
				}
				wifi.remove(input_text_component);
				input_text_component = std::make_shared<Texts>(wifi_window, 120, font, "Your password: " + input_string, 30, sf::Color::White);
				wifi.addComponent(input_text_component);
			}
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return) {
				finished = true;
				if (finished == true) {
					if (std::regex_match(input_string, random) == true) {
						points.set_no_debuff();
						wifi.addComponent(std::make_shared<Texts>(wifi_window, 180, font, "Success! You prevented the enemy from attacking.\n", 16, sf::Color::Green));
						wifi.addComponent(std::make_shared<Buttons>(wifi_window, 230, "Yaay!", [&wifi, &wifi_window, &points]() {

							wifi_window.close();

							}));
					}
					else {
						wifi.addComponent(std::make_shared<Texts>(wifi_window, 180, font, "Are you stupid? You're gonna get fired soon!.", 20, sf::Color::Red));
						wifi.addComponent(std::make_shared<Buttons>(wifi_window, 230, "Try again", [&window, &wifi_window, &points]() {
							wifi_window.close();
							dontchangemywife(window, points);
							}));
					}
				}
			}
			wifi.handleEvent(event);
		}
		wifi_window.clear(sf::Color::Black);
		wifi.drawing();
		wifi_window.draw(keypad.getSprite());
		wifi_window.display();
	}
}

void animateText(sf::RenderWindow& window, const std::string& textToAnimate, sf::Font& font) {
	sf::Text text("", font, 30);
	text.setFillColor(sf::Color::White);
	text.setPosition(10.f, 10.f);

	for (size_t i = 0; i < textToAnimate.size(); ++i) {
		text.setString(textToAnimate.substr(0, i + 1));
		window.clear(sf::Color::Black);
		window.draw(text);
		window.display();
		sf::sleep(sf::milliseconds(100));
	}

	bool removeText = false;
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
				removeText = true;
		}
	}
}

std::vector<std::string> loadtext(const fs::path& filepath, sf::RenderWindow &window) {
	std::vector<std::string> lines;
	std::ifstream file(filepath);
	float width = (window.getSize().y)/static_cast<float>(2);

	if (!file.is_open()) {
		std::cerr << "Couldn't load file!\n";
		return lines;
	}

	std::string pom;
	while (std::getline(file, pom)) {
		lines.push_back(pom);
	}
	file.close();
	return lines;
}

void tutorial(sf::RenderWindow& mainwindow, Panel& welcome, sf::Font& font, float maxWidth) {
	welcome.clear();
	fs::path tutorialpath = "text/tutorial.txt";
	std::vector<std::string> lines = loadtext(tutorialpath, mainwindow);
	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(30);
	text.setFillColor(sf::Color::White);
	sf::Texture mailbox;
	if (!mailbox.loadFromFile("footpics/mailbox.png"))
	{
		std::cout << "Could not load image" << std::endl;
	}

	float width = mailbox.getSize().x;
	float height = mailbox.getSize().y;
	sf::Sprite sprite(mailbox);

	sf::Vector2u windowSize = mainwindow.getSize(); 
	float windowWidth = static_cast<float>(windowSize.x);
	float windowHeight = static_cast<float>(windowSize.y);
	float scale = windowWidth / width/2;
	sprite.setScale(scale, scale);
	sprite.setPosition((windowWidth - width*scale) / 2, (windowHeight - height * scale) / 2);


	auto wrapText = [&font, maxWidth](const std::string& str) {
		std::vector<std::string> wrappedLines;
		std::istringstream words(str);
		std::string word;
		std::string line;
		sf::Text tempText;
		tempText.setFont(font);
		tempText.setCharacterSize(30);

		while (words >> word) {
			std::string testLine = line + (line.empty() ? "" : " ") + word;
			tempText.setString(testLine);
			if (tempText.getLocalBounds().width > maxWidth) {
				wrappedLines.push_back(line);
				line = word;
			}
			else {
				line = testLine;
			}
		}
		if (!line.empty()) {
			wrappedLines.push_back(line);
		}
		return wrappedLines;
		};

	size_t currentLineIndex = 0;
	size_t currentCharIndex = 0;
	sf::Clock charClock;
	bool waitingForSpace = false;
	std::vector<std::string> wrappedLines;

	if (!lines.empty()) {
		wrappedLines = wrapText(lines.front());
	}

	float startX = windowWidth / 4.0f; // X position at 1/4 of window width
	float startY = 2.0f * windowHeight / 5.0f; // Y position at 2/5 of window height

	while (!lines.empty()) {
		sf::Event event;
		while (mainwindow.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				mainwindow.close();
				return;
			}
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
				if (waitingForSpace) {

					waitingForSpace = false;
					currentLineIndex++;
					currentCharIndex = 0;
					charClock.restart();
					if (currentLineIndex >= wrappedLines.size()) {
						lines.erase(lines.begin());
						if (!lines.empty()) {
							wrappedLines = wrapText(lines.front());
							currentLineIndex = 0;
						}
					}
				}
			}
		}

		if (!lines.empty() && currentLineIndex < wrappedLines.size()) {
			std::string& currentLine = wrappedLines[currentLineIndex];
			if (currentCharIndex < currentLine.length()) {
				waitingForSpace = true;
				if (charClock.getElapsedTime().asSeconds() >= 0.03f) {
					currentCharIndex++;
					charClock.restart();
				}
			}
		}

		mainwindow.clear(sf::Color::Black);
		mainwindow.draw(sprite);

		float yOffset = startY;
		for (size_t i = 0; i <= currentLineIndex && i < wrappedLines.size(); ++i) {
			std::string displayString = wrappedLines[i];
			if (i == currentLineIndex) {
				displayString = wrappedLines[i].substr(0, currentCharIndex);
			}
			text.setString(displayString);
			text.setPosition(startX, yOffset);
			mainwindow.draw(text);
			yOffset += text.getCharacterSize() + 5;
		}
		mainwindow.display();
	}


	mainwindow.clear(sf::Color::Black);
	mainwindow.display();
}

void mailbox(sf::RenderWindow& window, float maxWidth, fs::path &path, std::string& name) {
	sf::Vector2u CurrRes = window.getSize();
	sf::Vector2f size(800, 800);
	sf::Vector2f position(0, 0);
	sf::Font font = fonts();
	sf::RenderWindow mailbox_window(sf::VideoMode(size.x, size.y), "Mailbox", sf::Style::Close);
	mailbox_window.setFramerateLimit(120);

	bool finished = false;
	Panel mailbox_panel(mailbox_window, sf::Vector2f(210, 20), sf::Vector2f(CurrRes.x * (3 / 2), CurrRes.y / 2), sf::Color::Black);

	mailbox_panel.addComponent(std::make_shared <Texts>(mailbox_window, 30, font, name, 35, sf::Color::Red));
	std::vector<std::string> lines = loadtext(path, mailbox_window);
	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(30);
	text.setFillColor(sf::Color::White);
	sf::Texture mailbox;
	if (!mailbox.loadFromFile("footpics/mailbox.png"))
	{
		std::cout << "Could not load image" << std::endl;
	}

	float width = mailbox.getSize().x;
	float height = mailbox.getSize().y;
	sf::Sprite sprite(mailbox);

	sf::Vector2u windowSize = mailbox_window.getSize();
	float windowWidth = static_cast<float>(windowSize.x);
	float windowHeight = static_cast<float>(windowSize.y);
	float scale = windowWidth / width ;
	sprite.setScale(scale, scale);
	sprite.setPosition((windowWidth - width * scale) / 2, (windowHeight - height * scale) / 2);

	auto wrapText = [&font, maxWidth](const std::string& str) {
		std::vector<std::string> wrappedLines;
		std::istringstream words(str);
		std::string word;
		std::string line;
		sf::Text tempText;
		tempText.setFont(font);
		tempText.setCharacterSize(30);

		while (words >> word) {
			std::string testLine = line + (line.empty() ? "" : " ") + word;
			tempText.setString(testLine);
			if (tempText.getLocalBounds().width > maxWidth) {
				wrappedLines.push_back(line);
				line = word;
			}
			else {
				line = testLine;
			}
		}
		if (!line.empty()) {
			wrappedLines.push_back(line);
		}
		return wrappedLines;
	};

	size_t currentLineIndex = 0;
	size_t currentCharIndex = 0;
	sf::Clock charClock;
	bool waitingForSpace = false;
	std::vector<std::string> wrappedLines;

	if (!lines.empty()) {
		wrappedLines = wrapText(lines.front());
	}

	float startX = windowWidth / 6.0f; // X position at 1/4 of window width
	float startY = 1.0f * windowHeight / 4.0f; // Y position at 1/4 of window height

	while (!lines.empty()) {
		sf::Event event;
		while (mailbox_window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				mailbox_window.close();
				return; 
			}
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
				if (waitingForSpace) {

					waitingForSpace = false;
					currentLineIndex++;
					currentCharIndex = 0;
					charClock.restart();
					if (currentLineIndex >= wrappedLines.size()) {
						lines.erase(lines.begin());
						if (!lines.empty()) {
							wrappedLines = wrapText(lines.front());
							currentLineIndex = 0;
						}
					}
				}
			}
		}

		if (!lines.empty() && currentLineIndex < wrappedLines.size()) {
			std::string& currentLine = wrappedLines[currentLineIndex];
			if (currentCharIndex < currentLine.length()) {
				waitingForSpace = true;
				if (charClock.getElapsedTime().asSeconds() >= 0.03f) {
					currentCharIndex++;
					charClock.restart();
				}
			}
		}

		mailbox_window.clear(sf::Color::Black);
		mailbox_panel.drawing();
		mailbox_window.draw(sprite);

		float yOffset = startY;
		for (size_t i = 0; i <= currentLineIndex && i < wrappedLines.size(); ++i) {
			std::string displayString = wrappedLines[i];
			if (i == currentLineIndex) {
				displayString = wrappedLines[i].substr(0, currentCharIndex);
			}
			text.setString(displayString);
			text.setPosition(startX, yOffset);
			mailbox_window.draw(text);
			yOffset += text.getCharacterSize() + 5;
		}
		mailbox_window.display();
	}

	mailbox_window.clear(sf::Color::Black);
	mailbox_window.display();
}


void window_settings(sf::RenderWindow & window, int& frameLimit) {
	sf::Vector2f size(800, 800);
	sf::Vector2f position(0, 0);
	sf::Font font = fonts();
	sf::RenderWindow settings(sf::VideoMode(size.x, size.y), "Settings", sf::Style::Close);
	settings.setFramerateLimit(frameLimit);
	sf::Vector2u CurrRes = window.getSize();

	Panel settings_panel(settings, sf::Vector2f(210, 20), sf::Vector2f(window.getSize().x * (3 / 2), window.getSize().y / 2), sf::Color::Black);
	settings_panel.addComponent(std::make_shared <Texts>(settings, 10, font, "Settings", 50, sf::Color::White));

	settings_panel.addComponent(std::make_shared<Texts>(settings, 200, font, "FPS limit:", 30, sf::Color::White));
	settings_panel.addComponent(std::make_shared<Buttons>(settings, 240, "15", [&window, &frameLimit]() {
		frameLimit = 15;
		window.setFramerateLimit(15);
		}));
	settings_panel.addComponent(std::make_shared<Buttons>(settings, 280, "30", [&window, &frameLimit]() {
		frameLimit = 30;
		window.setFramerateLimit(30);
		}));
	settings_panel.addComponent(std::make_shared<Buttons>(settings, 320, "60", [&window, &frameLimit]() {
		frameLimit = 60;
		window.setFramerateLimit(60);
		}));
	settings_panel.addComponent(std::make_shared<Buttons>(settings, 360, "120", [&window, &frameLimit]() {
		frameLimit = 120;
		window.setFramerateLimit(120);
		}));
	settings_panel.addComponent(std::make_shared<Buttons>(settings, 400, "OK", [&settings]() {
		settings.close();
		}));


	while (settings.isOpen()) {
		sf::Event event;
		while (settings.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				settings.close();
			}
			settings_panel.handleEvent(event);
		}
		settings.clear();
		settings_panel.drawing();
		settings.display();
	}
}

void notification(sf::RenderWindow& window) {
	sf::Vector2f size(800, 800);
	sf::Vector2f position(0, 0);
	sf::Font font = fonts();
	sf::RenderWindow notification_window(sf::VideoMode(size.x, size.y), "Notifications", sf::Style::Close);
	notification_window.setFramerateLimit(120);
	sf::Vector2u CurrRes = window.getSize();
	fs::path mailboxpath = "text/mail-box-guy-mail.txt";
	std::string mailboxname = "Mailbox Guy";
	fs::path bosspath = "text/boss_text.txt";
	std::string bossname = "B O S S";
	fs::path unknownpath = "text/unknown_text.txt";
	std::string unknownname = "Unknown";

	Buttons message1(notification_window, (0, 10), font, "MAILBOX GUY            (3 minutes ago)\nYou got a mail from Mailbox guy!",
		30, sf::Color::White, [&window, &mailboxpath, &mailboxname]() {
			mailbox(window, window.getSize().x / 2, mailboxpath, mailboxname);
		});
	int dziabka = 1;
	message1.dziabka(sf::Color::Blue, dziabka);
	Buttons message2(notification_window, (0, 70), font, "B        0      S        S            (10 minutes ago)\nGet to work. Don't make me regret hiring you.",
		30, sf::Color::White, [&window, &bosspath, &bossname]() {
			mailbox(window, window.getSize().x / 2, bosspath, bossname);
		});
	message2.dziabka(sf::Color::Blue, dziabka);

	Buttons message3(notification_window, (0, 130), font, "Unknown            (1 hour ago)\nWe've found you.",
		30, sf::Color::White, [&window, &unknownpath, &unknownname]() {
			mailbox(window, window.getSize().x / 2, unknownpath, unknownname);
		});
	message3.dziabka(sf::Color::Blue, dziabka);

	while (notification_window.isOpen()) {
		sf::Event event;
		while (notification_window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				notification_window.close();
			}
			message1.handleEvent(event);
			message2.handleEvent(event);
			message3.handleEvent(event);
		}
		notification_window.clear();
		message1.drawing();
		message2.drawing();
		message3.drawing();
		notification_window.display();
	}
}

void endgame(sf::RenderWindow &window, Points& points, std::string& name) {
	fs::path record_path = "text/records.txt";

		std::ofstream records(record_path, std::ios_base::app);
		if (!records.is_open())
		{
			std::cerr << "Couldn't load file!\n";
		}

		if (records.is_open()) {
			records << name << "\n";
			records << points.get_overall_points() << "\n";
			records.close();
		}
}

std::string save_records(sf::RenderWindow &window, Points &points, Panel &records_panel) {
	sf::Font font = fonts();
	
	std::vector<std::string> lines = loadtext("text/records.txt", window);

	std::string username;
	std::shared_ptr<Texts> input_text_component = std::make_shared<Texts>(window, window.getSize().y/2.25, font, "Enter username and press OK: ", 30, sf::Color::White);
	records_panel.addComponent(input_text_component);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			if (event.type == sf::Event::TextEntered) {
				if (event.text.unicode < 128 && event.text.unicode != 8) {
					username += static_cast<char>(event.text.unicode);
				}
				else if (event.text.unicode == 8 && !username.empty()) {
					username.pop_back();
				}
				records_panel.remove(input_text_component);
				input_text_component = std::make_shared<Texts>(window, window.getSize().y / 2.25, font, "Enter username and press OK: " + username, 30, sf::Color::White);
				records_panel.addComponent(input_text_component);
				records_panel.addComponent(std::make_shared<Buttons>(window, window.getSize().y / 2, "Ok", [&window]() {
					window.close();
					}));
			}
			records_panel.handleEvent(event);
		}
		window.clear(sf::Color::Black);
		records_panel.drawing();
		window.display();
	}
	return username;
}

void show_records(sf::RenderWindow& window, Panel& panel) {
	sf::Font font = fonts();
	bool finished = false;

	sf::Text header;
	header.setFont(font);
	header.setString("USERNAME\t\t|\t\tPOINTS");
	header.setCharacterSize(30);
	header.setFillColor(sf::Color::White);
	header.setPosition(window.getSize().x / 4.f, 50.f);

	std::vector<std::string> lines = loadtext("text/records.txt", window);

	std::vector<std::pair<std::string, int>> records;

	for (size_t i = 0; i + 1 < lines.size(); i += 2) {
		std::string username = lines[i];
		int points = std::stoi(lines[i + 1]);
		records.emplace_back(username, points);
	}

	auto filtered_records = records
		| std::views::filter([](const auto& record) {
		return record.second > 0;
			});

	std::vector<std::pair<std::string, int>> filtered_records_vec(filtered_records.begin(), filtered_records.end());

	std::ranges::sort(filtered_records_vec, [](const auto& a, const auto& b) {
		return a.second > b.second;
		});

	std::vector<sf::Text> usernameTexts;
	std::vector<sf::Text> pointTexts;
	float startY = 100.f;
	float lineHeight = 40.f;

	sf::View view(sf::FloatRect(0.f, 0.f, static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)));

	for (const auto& record : filtered_records_vec) {
		sf::Text usernameText;
		usernameText.setFont(font);
		usernameText.setString(record.first);
		usernameText.setCharacterSize(25);
		usernameText.setFillColor(sf::Color::White);
		usernameText.setPosition(200.f, startY);

		sf::Text pointText;
		pointText.setFont(font);
		pointText.setString(std::to_string(record.second));
		pointText.setCharacterSize(25);
		pointText.setFillColor(sf::Color::White);
		pointText.setPosition(window.getSize().x - 200.f, startY);

		usernameTexts.push_back(usernameText);
		pointTexts.push_back(pointText);
		startY += lineHeight;
	}

	float maxScroll = std::max(0.f, startY - window.getSize().y);

	std::shared_ptr<Buttons> exitButton = std::make_shared<Buttons>(window, window.getSize().y / 300, "<- Exit", [&window, &finished]() {
		finished = true;
		});
	panel.addComponent(exitButton);

	while (window.isOpen() && !finished) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}

			if (event.type == sf::Event::MouseWheelScrolled) {
				if (event.mouseWheelScroll.delta < 0 && view.getCenter().y < maxScroll + window.getSize().y / 2) {  // Przewijanie w dół
					view.move(0.f, 40.f);
				}
				else if (event.mouseWheelScroll.delta > 0 && view.getCenter().y > window.getSize().y / 2) {  // Przewijanie w górę
					view.move(0.f, -40.f);
				}
			}

			panel.handleEvent(event);
		}

		window.clear(sf::Color::Transparent);
		window.setView(view);

		panel.drawing();
		window.draw(header);

		for (size_t i = 0; i < usernameTexts.size(); ++i) {
			window.draw(usernameTexts[i]);
			window.draw(pointTexts[i]);
		}

		window.display();
	}
}


void resetGame(sf::RenderWindow& mainwindow, sf::Texture& map, sf::Sprite& sprite, Panel& top_index, Panel& left_index, GameClock& gameclock, bool& showAction, sf::Texture& wifi_icon, sf::Texture& settings_icon, sf::Texture&  leave_icon, sf::Texture&  bell, sf::Texture&  mail, sf::Vector2f& CurrRes, Points& points, Texts &point_text, int &framelimit, std::string &mailboxname, std::filesystem::path &mailboxpath) {
	gameclock.reset();
	points.set_no_debuff();
	float posX_clock = (mainwindow.getSize().x / 10) * 7; 
	float posY_clock = mainwindow.getSize().y / 48; 
	sf::Vector2f position(posX_clock, posY_clock); 
	gameclock.move(position);
	top_index.clear();
	left_index.clear();

	sprite.setTexture(map);
	float windowWidth = mainwindow.getSize().x;
	float windowHeight = mainwindow.getSize().y;
	float mapWidth = map.getSize().x;
	float mapHeight = map.getSize().y;

	float maxAllowedX = windowWidth - (windowWidth / 9.0f); // 1/9 of screen width from the left
	float maxAllowedY = windowHeight - (windowHeight / 8.0f); // 1/8 of screen height from the top

	float scaleX = (maxAllowedX / mapWidth);
	float scaleY = (maxAllowedY / mapHeight);
	float scale = std::min(scaleX, scaleY);

	sprite.setScale(scale, scale);

	float posX = std::max(windowWidth - mapWidth * scale, maxAllowedX - mapWidth * scale);
	float posY = std::max(windowHeight * (1.0f / 8.0f), maxAllowedY - mapHeight * scale);

	sprite.setPosition(posX, posY);

	sf::Vector2f left_index_size((CurrRes.x) / 9, CurrRes.y);
	sf::Vector2f top_index_size(CurrRes.x, (CurrRes.y) / 8);
	top_index.addComponent(std::make_shared<Buttons>(mainwindow, settings_icon, sf::Vector2f(90, -1), sf::Vector2f(100, 100), top_index_size, sf::Vector2f(0, 0), [&mainwindow, &framelimit]() {
		window_settings(mainwindow, framelimit);
		}));
	Buttons wifi_button(mainwindow, wifi_icon, sf::Vector2f(80, -1), sf::Vector2f(100, 100), top_index_size, sf::Vector2f(0, 0),
		[&mainwindow, &points]() {
			if (points.get_debuff() == true) {

				dontchangemywife(mainwindow, points);
				points.set_no_debuff();
			}
		});
	points = Points();
	std::string	pom = "Points: " + points.get_overall_points() + " m";
	point_text.update(pom);
	
	left_index.addComponent(std::make_shared<Buttons>(mainwindow, leave_icon, sf::Vector2f(-1, 85), sf::Vector2f(100, 100), left_index_size, sf::Vector2f(0, 0), [&mainwindow]() {mainwindow.close(); }));
	//fs::path mailboxpath = "text/mail-box-guy-mail.txt";
	//std::string mailboxname = "Mailbox Guy";
	left_index.addComponent(std::make_shared<Buttons>(mainwindow, mail, sf::Vector2f(-1, 75), sf::Vector2f(100, 100), left_index_size, sf::Vector2f(0, 0), [&mainwindow, &mailboxpath, &mailboxname]() {
		mailbox(mainwindow, mainwindow.getSize().x / 2, mailboxpath, mailboxname);
		}));
	left_index.addComponent(std::make_shared<Buttons>(mainwindow, bell, sf::Vector2f(-1, 65), sf::Vector2f(100, 100), left_index_size, sf::Vector2f(0, 0), [&mainwindow]() {
		notification(mainwindow);
		}));

	showAction = false;
}

int main() 
{
	bool ResolutionSet = false;
	int index = 0, fpscount;
	sf::Vector2f CurrRes(600, 350);
	int framelimit = 120;
	//********************************************************* S T A R T I N G  V I E W***************************************************************
	sf::RenderWindow settings(sf::VideoMode(CurrRes.x, CurrRes.y), "SETTINGS", sf::Style::Close);
	settings.setFramerateLimit(framelimit);

	CurrRes.x = 800;
	CurrRes.y = 600;

	sf::Font font = fonts();
	sf::Text fpsText("", font, 15);
	fpsText.setPosition(10, 10);
	sf::Clock clock;
	float lastTime = 0.0f;
	//============================================================ T E S T ====================================================================================
	Panel settingss(settings, sf::Vector2f(0, 0), CurrRes, sf::Color(200, 200, 200, 30));
	settingss.addComponent(std::make_shared <Texts>(settings, 50, font, "What resolution do you need?", 20, sf::Color::White));
	settingss.addComponent(std::make_shared<Buttons>(settings, 100, "800x600", [&CurrRes]() {
		CurrRes.x = 800;
		CurrRes.y = 600;
		}));
	settingss.addComponent(std::make_shared<Buttons>(settings, 150, "1200x900", [&CurrRes]() {
		CurrRes.x = 1200;
		CurrRes.y = 900;
		}));
	settingss.addComponent(std::make_shared<Buttons>(settings, 200, "1920x1080", [&CurrRes]() {
		CurrRes.x = 1920;
		CurrRes.y = 1080;
		}));
	settingss.addComponent(std::make_shared<Buttons>(settings, 250, "OK", [&settings]() {
		settings.close();
		}));
	//============================================================ T E S T  E N D ====================================================================================
	SpriteManager background_sett(sf::Vector2f(600, 350), "new_wirus", sf::Vector2f(-1, -1), sf::Vector2f(200, 200));


	while (settings.isOpen()) {
		sf::Event event;
		while (settings.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				settings.close();
				return 0;
			}
			settingss.handleEvent(event);
		}
		settings.clear();
		settings.draw(background_sett.getSprite());
		settingss.drawing();


		float currentTime = clock.restart().asSeconds();
		float fps = 1.0f / currentTime;
		lastTime = currentTime;
		fpsText.setString("FPS: " + std::to_string(static_cast<int>(fps)));
		settings.draw(fpsText);

		settings.display();
	}

	////********************************************************* G A M E  L O O P **********************************************************************
	bool game_is_on = true;

	if (game_is_on == true) {


		sf::RenderWindow mainwindow(sf::VideoMode(CurrRes.x, CurrRes.y), "World.Inc", sf::Style::Close);
		mainwindow.setFramerateLimit(120);

		bool cont = false;
		bool if_tutorial = false;
		SpriteManager logo(sf::Vector2f(CurrRes.x, CurrRes.y), "first_logo", sf::Vector2f(-1, 55), sf::Vector2f(600, -1));
		Panel welcome(mainwindow, sf::Vector2f(0, 0), sf::Vector2f(mainwindow.getSize().x, mainwindow.getSize().y), sf::Color::Transparent);
		Panel records_panel(mainwindow, sf::Vector2f(0, 0), sf::Vector2f(mainwindow.getSize().x, mainwindow.getSize().y), sf::Color::Transparent);

		welcome.addComponent(std::make_shared <Texts>(mainwindow, CurrRes.x / 45, font, "WELCOME TO", 50, sf::Color::White));
		welcome.addComponent(std::make_shared <Texts>(mainwindow, CurrRes.x / 8, font, "WORLD.INC", 70, sf::Color::Red));
		welcome.addComponent(std::make_shared <Texts>(mainwindow, CurrRes.x / 4, font, "Let's begin your shift", 30, sf::Color::White));
		welcome.addComponent(std::make_shared<Buttons>(mainwindow, CurrRes.x / 3, "Start", [&cont]() {
			cont = true;
		}));
		welcome.addComponent(std::make_shared<Buttons>(mainwindow, CurrRes.x / 2.5, "Tutorial", [&if_tutorial, &cont]() {
			if_tutorial = true;
			cont = true;
		}));

		welcome.addComponent(std::make_shared<Buttons>(mainwindow, CurrRes.x / 2.14, "Records", [&mainwindow, &records_panel]() {
			show_records(mainwindow, records_panel);
			}));
		
		while (!cont) {
			sf::Event event;
			while (mainwindow.pollEvent(event)) {
				if (event.type == sf::Event::Closed) {
					mainwindow.close();
					return 0;
				}
				welcome.handleEvent(event);
			}
			mainwindow.clear(sf::Color::Black);
			mainwindow.draw(logo.getSprite());
			welcome.drawing();
			mainwindow.display();
		}
		if(if_tutorial)
		tutorial(mainwindow, welcome, font, mainwindow.getSize().x/2);

		sf::Texture map;
		sf::Texture actionbutton;

		if (!map.loadFromFile("footpics/map.png"))
		{
			std::cout << "Could not load image" << std::endl;
			return -1; 
		}
		if (!actionbutton.loadFromFile("footpics/actionbutton.png"))
		{
			std::cout << "Could not load image" << std::endl;
			return -1; 
		}

		sf::Sprite sprite;
		sprite.setTexture(map);

		float windowWidth = mainwindow.getSize().x;
		float windowHeight = mainwindow.getSize().y;
		float mapWidth = map.getSize().x;
		float mapHeight = map.getSize().y;

		float maxAllowedX = windowWidth - (windowWidth / 9.0f); // 1/9 of screen width from the left
		float maxAllowedY = windowHeight - (windowHeight / 8.0f); // 1/8 of screen height from the top

		float scaleX = (maxAllowedX / mapWidth);
		float scaleY = (maxAllowedY / mapHeight);
		float scale = std::min(scaleX, scaleY);

		sprite.setScale(scale, scale);

		float posX = std::max(windowWidth - mapWidth * scale, maxAllowedX - mapWidth * scale);
		float posY = std::max(windowHeight * (1.0f / 8.0f), maxAllowedY - mapHeight * scale);

		sprite.setPosition(posX, posY);

		//tworzenie dziabek
		sf::Texture settings_icon;
		if (!settings_icon.loadFromFile("footpics/settings_icon.png")) {
			std::cerr << "Couldn't load image!\n";
		}

		sf::Texture wifi_icon;
		if (!wifi_icon.loadFromFile("footpics/wifi-logo.png")) {
			std::cerr << "Couldn't load image!\n";
		}

		sf::Texture leave_icon;
		if (!leave_icon.loadFromFile("footpics/leave_icon.png")) {
			std::cerr << "Couldn't load image!\n";
		}

		sf::Texture mail;
		if (!mail.loadFromFile("footpics/mail.png")) {
			std::cerr << "Couldn't load image!\n";
		}

		sf::Texture bell;
		if (!bell.loadFromFile("footpics/bell.png")) {
			std::cerr << "Couldn't load image!\n";
		}
		sf::Texture top_index_texture;
		if (!top_index_texture.loadFromFile("footpics/top_index.png")) {
			std::cerr << "Couldn't load image!\n";
		}
		sf::Texture left_index_texture;
		if (!left_index_texture.loadFromFile("footpics/left_index.png")) {
			std::cerr << "Couldn't load image!\n";
		}
		sf::Texture numbers_background;
		if (!numbers_background.loadFromFile("footpics/numbers1.png")) {
			std::cerr << "Couldn't load image!\n";
		}

		float numWidth = numbers_background.getSize().x;
		float numHeight = numbers_background.getSize().y;
		sf::Sprite num_bg(numbers_background);
		

		float scalenum = windowWidth / numWidth;
		num_bg.setScale(scalenum, scalenum);
		num_bg.setPosition(0, (windowHeight - numHeight * scalenum));

		Points point;
		std::string point_string = point.get_overall_points();
		Texts point_text(mainwindow, CurrRes.y/60, font, "Points: "+point_string + " m", 24, sf::Color::White);
		std::string pom;
		int invisible = 1.5f;

		sf::Vector2f top_index_size(CurrRes.x, (CurrRes.y) / 8);
		Panel top_index(mainwindow, sf::Vector2f(0, 0), top_index_size, top_index_texture);
		sf::Vector2f left_index_size((CurrRes.x) / 9, CurrRes.y); 
		Panel left_index(mainwindow, sf::Vector2f(0, 0), left_index_size, left_index_texture); 
		top_index.addComponent(std::make_shared<Buttons>(mainwindow, settings_icon, sf::Vector2f(90, -1), sf::Vector2f(100, 100), top_index_size, sf::Vector2f(0, 0), [&mainwindow, &framelimit]() {
			window_settings(mainwindow, framelimit);
			}));
		Buttons wifi_button(mainwindow, wifi_icon, sf::Vector2f(80, -1), sf::Vector2f(100, 100), top_index_size, sf::Vector2f(0, 0),
			[&mainwindow, &point]() {
				if (point.get_debuff() == true) {

					point.set_no_debuff();
					dontchangemywife(mainwindow, point);	
				}
			});
		float posX_clock = (mainwindow.getSize().x / 10) * 7;
		float posY_clock = mainwindow.getSize().y / 48;
		sf::Vector2f position(posX_clock, posY_clock);
		GameClock gameclock(mainwindow, position);


		left_index.addComponent(std::make_shared<Buttons>(mainwindow, leave_icon, sf::Vector2f(-1, 85), sf::Vector2f(100, 100), left_index_size, sf::Vector2f(0, 0), [&mainwindow]() {mainwindow.close(); })); 
		fs::path mailboxpath = "text/mail-box-guy-mail.txt";
		std::string mailboxname = "Mailbox Guy";
		left_index.addComponent(std::make_shared<Buttons>(mainwindow, mail, sf::Vector2f(-1, 75), sf::Vector2f(100, 100), left_index_size, sf::Vector2f(0, 0), [&mainwindow, &mailboxpath, &mailboxname]() {
			mailbox(mainwindow, mainwindow.getSize().x / 2, mailboxpath, mailboxname);
			}));
		left_index.addComponent(std::make_shared<Buttons>(mainwindow, bell, sf::Vector2f(-1, 65), sf::Vector2f(100, 100), left_index_size, sf::Vector2f(0, 0), [&mainwindow]() {
			notification(mainwindow);
			}));  

		sf::Sprite action(actionbutton);
		action.setScale(1.0f / 3.0f, 1.0f / 3.0f);

		bool showAction = false;
		bool minigame_finished = false;

		sf::Vector2f spritePos = sprite.getPosition();
		sf::Vector2f actionSize = static_cast<sf::Vector2f>(action.getTexture()->getSize());

		int hardness = 3;

		while (mainwindow.isOpen()) {
			sf::Event event;
			while (mainwindow.pollEvent(event)) {
				if (event.type == sf::Event::Closed)
					mainwindow.close();
				wifi_button.handleEvent(event);
				top_index.handleEvent(event);
				left_index.handleEvent(event);

				if (event.type == sf::Event::MouseButtonPressed) {
					if (event.mouseButton.button == sf::Mouse::Left) {
						sf::Vector2i mousePos = sf::Mouse::getPosition(mainwindow);
						sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
						if (action.getGlobalBounds().contains(mousePosF)) {
							showAction = false;
							int randomNum = std::rand() % 2 + 1;
							std::srand(static_cast<unsigned int>(std::time(nullptr)));

							if (randomNum == 1) {
								minigame(mainwindow, gameclock, point, framelimit, hardness);
								hardness++;
							}
							else {
								minigame2(mainwindow, gameclock, point, framelimit);
							}

							pom =  "Points: "+ point.get_overall_points()+" m";
							point_text.update(pom);
							
						}
					}
				}

				if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::G) {
					game_is_on = false;
					gameclock.stop();

					float im_sorry = CurrRes.x/1200.0f;
					mainwindow.clear(sf::Color::Black);
					Panel game_over_panel(mainwindow, sf::Vector2f(0, 0), sf::Vector2f(CurrRes.x, CurrRes.y), sf::Color::Transparent); 
					game_over_panel.addComponent(std::make_shared <Texts>(mainwindow, static_cast<float>(im_sorry*60), font, "YOU ARE FIRED!", static_cast<float>(im_sorry * 100), sf::Color::Red));
					Texts points_text(mainwindow, static_cast<float>(im_sorry * 300), font, "", 60, sf::Color::White);
					//gameclock.move(sf::Vector2f(720, 320));
					game_over_panel.addComponent(std::make_shared <Buttons>(mainwindow, static_cast<float>(sqrt(im_sorry) * 710), font, "Try again", static_cast<float>(im_sorry * 40), sf::Color::White, [&game_is_on, &mainwindow, &map, &sprite, &top_index, &left_index, &gameclock, &actionbutton, &action, &showAction, &wifi_icon, &settings_icon, &leave_icon, &bell, &mail, &CurrRes, &point, &point_text, &framelimit, &mailboxname, &mailboxpath]() {
						game_is_on = true;
						resetGame(mainwindow, map, sprite, top_index, left_index, gameclock, showAction, wifi_icon, settings_icon, leave_icon, bell, mail, CurrRes, point, point_text, framelimit, mailboxname, mailboxpath); 
					}));
					SpriteManager fired_stamp(CurrRes, "fired", sf::Vector2f(40, -1), sf::Vector2f(-1, static_cast<float>(sqrt(im_sorry) * 200)));
					sf::Sprite stamp_sprite = fired_stamp.getSprite();
					stamp_sprite.setRotation(30);
					std::string points_text_string = "Time: " + gameclock.get_clock_as_text();
					points_text.update(points_text_string); 
					while (!game_is_on) {
						sf::Event event;
						while (mainwindow.pollEvent(event)) {
							if (event.type == sf::Event::Closed) {
								mainwindow.close();
								return 0;
							}
							game_over_panel.handleEvent(event);
						}
						mainwindow.clear(sf::Color::Black);
						mainwindow.draw(stamp_sprite);  
						game_over_panel.drawing();
						points_text.drawing();
						mainwindow.display();
					}
				}

				if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::W) {
					game_is_on = false;
					gameclock.stop();

					mainwindow.clear(sf::Color::Black);
					Panel game_won(mainwindow, sf::Vector2f(0, 0), sf::Vector2f(mainwindow.getSize().x, mainwindow.getSize().y), sf::Color::Black);
					game_won.addComponent(std::make_shared <Texts>(mainwindow, mainwindow.getSize().y / 30, font, "Your shift is over", 60, sf::Color::White));
					game_won.addComponent(std::make_shared<Texts>(mainwindow, mainwindow.getSize().y / 9, font, "You can go home now... ", 50, sf::Color::White));
					game_won.addComponent(std::make_shared<Texts>(mainwindow, mainwindow.getSize().y / 4.5, font, point.get_overall_points(), 50, sf::Color::Green));

					gameclock.move(sf::Vector2f(CurrRes.x/2, CurrRes.y/2));
					std::string username = save_records(mainwindow, point, game_won);
					endgame(mainwindow, point, username);
					

					while (mainwindow.isOpen()) {
						sf::Event event;
						while (mainwindow.pollEvent(event)) {
							if (event.type == sf::Event::Closed) {
								mainwindow.close();
								return 0;
							}
							game_won.handleEvent(event);
						}
						game_won.drawing();
						gameclock.drawing(mainwindow);
						mainwindow.display();
					}
				}


			}
			if (gameclock.hour() == 4) {
				game_is_on = false; 
				gameclock.stop(); 

				mainwindow.clear(sf::Color::Black); 
				Panel game_won(mainwindow, sf::Vector2f(0, 0), sf::Vector2f(mainwindow.getSize().x, mainwindow.getSize().y), sf::Color::Black); 
				game_won.addComponent(std::make_shared <Texts>(mainwindow, mainwindow.getSize().y / 30, font, "Your shift is over", 60, sf::Color::White)); 
				game_won.addComponent(std::make_shared<Texts>(mainwindow, mainwindow.getSize().y / 9, font, "You can go home now... ", 50, sf::Color::White)); 
				game_won.addComponent(std::make_shared<Texts>(mainwindow, mainwindow.getSize().y / 4.5, font, point.get_overall_points(), 50, sf::Color::Green)); 

				gameclock.move(sf::Vector2f(CurrRes.x / 2, CurrRes.y / 2));
				std::string username = save_records(mainwindow, point, game_won);
				endgame(mainwindow, point, username);


				while (mainwindow.isOpen()) {
					sf::Event event;
					while (mainwindow.pollEvent(event)) {
						if (event.type == sf::Event::Closed) {
							mainwindow.close();
							return 0;
						}
						game_won.handleEvent(event);
					}
					game_won.drawing();
					gameclock.drawing(mainwindow);
					mainwindow.display();
				}
			}
			if (std::stoi(point.get_overall_points()) < 0) {
				game_is_on = false;
				gameclock.stop();

				float im_sorry = CurrRes.x / 1200.0f;
				mainwindow.clear(sf::Color::Black);
				Panel game_over_panel(mainwindow, sf::Vector2f(0, 0), sf::Vector2f(CurrRes.x, CurrRes.y), sf::Color::Transparent);
				game_over_panel.addComponent(std::make_shared <Texts>(mainwindow, static_cast<float>(im_sorry * 60), font, "YOU ARE FIRED!", static_cast<float>(im_sorry * 100), sf::Color::Red));
				Texts points_text(mainwindow, static_cast<float>(im_sorry * 300), font, "", 60, sf::Color::White);
				game_over_panel.addComponent(std::make_shared <Buttons>(mainwindow, static_cast<float>(sqrt(im_sorry) * 710), font, "Try again", static_cast<float>(im_sorry * 40), sf::Color::White, [&game_is_on, &mainwindow, &map, &sprite, &top_index, &left_index, &gameclock, &actionbutton, &action, &showAction, &wifi_icon, &settings_icon, &leave_icon, &bell, &mail, &CurrRes, &point, &point_text, &framelimit, &mailboxname, &mailboxpath]() {
					game_is_on = true;
					resetGame(mainwindow, map, sprite, top_index, left_index, gameclock, showAction, wifi_icon, settings_icon, leave_icon, bell, mail, CurrRes, point, point_text, framelimit, mailboxname, mailboxpath);
					}));
				SpriteManager fired_stamp(CurrRes, "fired", sf::Vector2f(40, -1), sf::Vector2f(-1, static_cast<float>(sqrt(im_sorry) * 200)));
				sf::Sprite stamp_sprite = fired_stamp.getSprite();
				stamp_sprite.setRotation(30);
				std::string points_text_string = "Time: " + gameclock.get_clock_as_text();
				points_text.update(points_text_string);
				while (!game_is_on) {
					sf::Event event;
					while (mainwindow.pollEvent(event)) {
						if (event.type == sf::Event::Closed) {
							mainwindow.close();
							return 0;
						}
						game_over_panel.handleEvent(event);
					}
					mainwindow.clear(sf::Color::Black);
					mainwindow.draw(stamp_sprite);
					game_over_panel.drawing();
					points_text.drawing();
					mainwindow.display();
				}
			}

			if ((gameclock.hour() == 23 || gameclock.hour() == 0 || gameclock.hour() == 1 || gameclock.hour() == 2 || gameclock.hour() == 3)&&gameclock.minute()==0) {
				point.set_yes_debuff();
			}
			point.debuff_animation(wifi_button, point_text);

			gameclock.update();
			mainwindow.clear(sf::Color::Color(0, 0, 30));
			mainwindow.draw(num_bg);
			mainwindow.draw(sprite);
			left_index.drawing();
			top_index.drawing();
			wifi_button.drawing();
			point_text.drawing();
			gameclock.drawing(mainwindow);
			if (showAction) {
				mainwindow.draw(action);
			}
			else {
				float randomX = spritePos.x + (rand() % static_cast<int>(CurrRes.x - spritePos.x - actionSize.x));
				float randomY = spritePos.y + (rand() % static_cast<int>(CurrRes.y - spritePos.y - actionSize.y));
				action.setPosition(randomX, randomY);
			}
			if (rand() % 100 == 1) {
				showAction = true;
			}

			mainwindow.display();
		}
	}
	return 0;

}