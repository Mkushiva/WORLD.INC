//#include "Functions.h"
//
//
//
//namespace fs = std::filesystem;
//void minigame2(sf::RenderWindow& window, GameClock& gameclock, Points& point) {
//	//std::random_device rd;  // Urz¹dzenie losowe do inicjalizacji generatora 
//	//std::mt19937 gen(rd()); // Mersenne Twister generator 
//	//std::uniform_real_distribution<> dis(1.0, 2.0); // Rozk³ad jednostajny od 1.0 do 2.0  
//
//	float angle = 30;
//	sf::Vector2f size(600, 600);
//	sf::Vector2f position(0, 0);
//	sf::Font font = fonts();
//	sf::RenderWindow minigame_window(sf::VideoMode(size.x, size.y), "Cyberattack", sf::Style::Titlebar);
//	minigame_window.setFramerateLimit(120);
//
//	Panel minigame_panel(window, sf::Vector2f(0, 0), size, sf::Color::Transparent);
//	minigame_panel.addComponent(std::make_shared<Texts>(minigame_window, 20, font, "Shoot those viruses down!", 20, sf::Color::White));
//	SpriteManager background(size, "minigame_background", sf::Vector2f(-1, -1), sf::Vector2f(-1, 900));
//	auto texture = std::make_shared<sf::Texture>();
//	auto texture1 = std::make_shared<sf::Texture>();
//	auto texture2 = std::make_shared<sf::Texture>();
//
//	if (!texture->loadFromFile("footpics/new_wirus.png") ||
//		!texture1->loadFromFile("footpics/new_wirus.png") ||
//		!texture2->loadFromFile("footpics/new_wirus.png")) {
//		std::cerr << "Couldn't load the image!\n";
//	}
//	sf::Texture aship;
//	if (!aship.loadFromFile("footpics/starship_red.png")) {
//		std::cerr << "Couldn't load the image!\n";
//	}
//	ModifiedHeart heart(minigame_window, aship, 2.0f);
//	std::vector<Obstacle> obstacles;
//
//	int rows = 3, cols = 4;
//	sf::Vector2f obstacleSize(texture->getSize());
//	obstacleSize.x *= 0.7f;
//	obstacleSize.y *= 0.7f;
//
//	float horizontalSpacing = obstacleSize.x + 70.0f; // Odstêp miêdzy przeszkodami w poziomie
//	float verticalSpacing = obstacleSize.y + 40.0f;   // Odstêp miêdzy przeszkodami w pionie
//
//	for (int i = 0; i < rows; ++i) {
//		for (int j = 0; j < cols; ++j) {
//			sf::Vector2f pos(100.0f + j * horizontalSpacing, 50.0f + i * verticalSpacing);
//			std::shared_ptr<sf::Texture> tex;
//			if (i % 3 == 0)
//				tex = texture;
//			else if (i % 3 == 1)
//				tex = texture1;
//			else
//				tex = texture2;
//
//			obstacles.emplace_back(pos, minigame_window, tex, sf::Vector2f(1.0f, 0.0f));
//		}
//	}
//
//	sf::RectangleShape bullet(sf::Vector2f(5, 10));
//	bullet.setFillColor(sf::Color::White);
//	std::vector<sf::RectangleShape> bullets;
//	sf::Clock bulletClock;
//	sf::Time bulletCooldown = sf::seconds(0.5f);
//	Countdown count_down(minigame_window, 70, font, 16, 1.0f, sf::Color::White);
//	Countdown game_timer(minigame_window, 70, font, 16, 1.0f, sf::Color::White);
//
//	sf::Clock frameClock;
//	sf::Clock startClock;
//	int countdown = 5; // Start countdown from 5
//	bool spacePressed = false;
//	bool gameStarted = false;
//
//	while (minigame_window.isOpen()) {
//		float deltaTime = frameClock.restart().asSeconds();
//
//		if (!gameStarted) {
//			int elapsed = static_cast<int>(startClock.getElapsedTime().asSeconds());
//			countdown = 5 - elapsed;
//			if (countdown <= 0) {
//				gameStarted = true;
//				startClock.restart();
//			}
//		}
//		else {
//			game_timer.update();
//			sf::Event event;
//			while (minigame_window.pollEvent(event)) {
//				if (event.type == sf::Event::Closed) {
//					minigame_window.close();
//				}
//				heart.handleEvent(event);
//				if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
//					spacePressed = true;
//				}
//
//				if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Space) {
//					spacePressed = false;
//				}
//			}
//		}
//
//		if (gameStarted) {
//			if (spacePressed && bulletClock.getElapsedTime() >= bulletCooldown) {
//				bullet.setPosition(heart.getSprite().getPosition().x + heart.getSprite().getGlobalBounds().width / 2, heart.getSprite().getPosition().y);
//				bullets.push_back(bullet);
//				bulletClock.restart();
//			}
//
//			heart.update(deltaTime);
//
//			for (auto& bullet : bullets) {
//				bullet.move(0, -5);
//			}
//
//			bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [&](const sf::RectangleShape& b) {
//				return b.getPosition().y < 0;
//				}), bullets.end());
//
//			for (auto& obstacle : obstacles) {
//				obstacle.movement();
//			}
//
//			for (auto it = bullets.begin(); it != bullets.end();) {
//				bool hit = false;
//				for (auto ob_it = obstacles.begin(); ob_it != obstacles.end() && !hit;) {
//					if (ob_it->checkCollision(*it)) {
//						ob_it = obstacles.erase(ob_it);
//						it = bullets.erase(it);
//						hit = true;
//					}
//					else {
//						++ob_it;
//					}
//				}
//				if (!hit) {
//					++it;
//				}
//			}
//		}
//
//		minigame_window.clear();
//		minigame_window.draw(background.getSprite());
//		minigame_panel.drawing();
//		heart.drawing();
//		for (const auto& bullet : bullets) {
//			minigame_window.draw(bullet);
//		}
//		for (const auto& obstacle : obstacles) {
//			obstacle.drawing_const();
//		}
//		if (gameStarted) {
//			game_timer.drawing();
//			/*for (size_t i = 0; i < obstacles.size(); ++i) {
//				for (size_t j = i + 1; j < obstacles.size(); ++j) {
//					obstacles[i].handleCollision(obstacles[j]);
//				}
//			}*/
//		}
//		else {
//			// Display the countdown
//			Texts countdownText(minigame_window, 200, font, std::to_string(countdown), 100, sf::Color::White);
//			countdownText.drawing();
//		}
//		minigame_window.display();
//
//		if (obstacles.empty() || game_timer.isFinished()) {
//			Texts points_string(minigame_window, 150, font, "", 20, sf::Color::White);
//			std::string points_text = "Your points: ";
//			point.add(game_timer.get_n());
//			std::string pom = point.get_minigame_points();
//			std::string your_points = points_text + pom;
//			points_string.update(your_points);
//
//			std::string choosing_stamp;
//			Panel minigame_ended(minigame_window, sf::Vector2f(0, 0), size, sf::Color::Transparent);
//			if (std::stoi(pom) <= 0) {
//				choosing_stamp = "fail";
//				minigame_ended.addComponent(std::make_shared<Texts>(minigame_window, 50, font, "You successfully failed the minigame!", 24, sf::Color::Red));
//				minigame_ended.addComponent(std::make_shared<Texts>(minigame_window, 80, font, "You bring dishonor to our company!", 20, sf::Color::White));
//				minigame_ended.addComponent(std::make_shared<Buttons>(minigame_window, 300, font, "I am a crap!", 30, sf::Color::Red, [&minigame_window]() {
//					minigame_window.close();
//					}));
//			}
//			else {
//				choosing_stamp = "pass";
//				minigame_ended.addComponent(std::make_shared<Texts>(minigame_window, 50, font, "You successfully beat the minigame!", 25, sf::Color::Green));
//				minigame_ended.addComponent(std::make_shared<Texts>(minigame_window, 80, font, "You bring honor to our company!", 20, sf::Color::White));
//				minigame_ended.addComponent(std::make_shared<Buttons>(minigame_window, 300, font, "I am ready to bring some more!", 30, sf::Color::White, [&minigame_window]() {
//					minigame_window.close();
//					}));
//			}
//			SpriteManager stamp(size, choosing_stamp, sf::Vector2f(60, 60), sf::Vector2f(-1, 100));
//			sf::Sprite stamp_sprite = stamp.getSprite();
//			stamp_sprite.setRotation(angle);
//
//			while (minigame_window.isOpen()) {
//				sf::Event event1;
//				while (minigame_window.pollEvent(event1)) {
//					minigame_ended.handleEvent(event1);
//				}
//				minigame_window.clear(sf::Color::Black);
//				minigame_ended.drawing();
//				points_string.drawing();
//				minigame_window.draw(stamp_sprite);
//				minigame_window.display();
//			}
//
//		}																																				  5
//			; gameclock.update();
//	}
//}
//
//void minigame(sf::RenderWindow& window, GameClock& gameclock, Points& point) {
//	std::random_device rd;  // Urz¹dzenie losowe do inicjalizacji generatora 
//	std::mt19937 gen(rd()); // Mersenne Twister generator 
//	std::uniform_real_distribution<float> pos_dis(100.0f, 550.0f); // Rozk³ad jednostajny dla pozycji od 100 do 550 
//	std::uniform_real_distribution<float> vel_dis(-3.0f, 3.0f);
//
//	float angle = 30;
//	sf::Vector2f size(600, 600);
//	sf::RenderWindow minigame_window(sf::VideoMode(size.x, size.y), "Epidemy", sf::Style::Titlebar);
//	minigame_window.setFramerateLimit(120);
//
//	SpriteManager background(size, "minigame_background", sf::Vector2f(-1, -1), sf::Vector2f(-1, 900));
//
//	std::vector<std::shared_ptr<sf::Texture>> textures;
//
//	// Dodawanie tekstur do vector'a
//	textures.push_back(std::make_shared<sf::Texture>());
//	textures.push_back(std::make_shared<sf::Texture>());
//	textures.push_back(std::make_shared<sf::Texture>());
//	int i = 1;
//	for (auto texture : textures) {
//		if (!texture->loadFromFile("footpics/new_wirus.png")) {
//			std::cerr << "Couldn't load the images!\n";
//			return;
//		}
//	}
//	/*if (!textures[0]->loadFromFile("footpics/virusix_1.png") || !textures[1]->loadFromFile("footpics/virusix_2.png") || !texture2->loadFromFile("footpics/virusix_3.png")) {
//		std::cerr << "Couldn't load the images!\n";
//		return;
//	}*/
//	Panel minigame_panel(window, sf::Vector2f(0, 0), size, sf::Color::Transparent);
//	sf::Font font = fonts();
//	minigame_panel.addComponent(std::make_shared<Texts>(minigame_window, 50, font, "Dodge those viruses!", 20, sf::Color::White));
//
//	std::vector<Obstacle> obstacles;
//	for (int i = 0; i < 3; ++i) {
//		sf::Vector2f random_position;
//		bool valid_position;
//
//		do {
//			valid_position = true;
//			random_position = sf::Vector2f(pos_dis(gen), pos_dis(gen));
//			for (const auto& obstacle : obstacles) {
//				if (std::hypot(random_position.x - obstacle.get_position().x, random_position.y - obstacle.get_position().y) < 50) {
//					valid_position = false;
//					break;
//				}
//			}
//		} while (!valid_position);
//
//		sf::Vector2f random_velocity(vel_dis(gen) + 0.1 * i, vel_dis(gen) + 0.1 * i); // Losowanie prêdkoœci
//		obstacles.emplace_back(random_position, minigame_window, textures[i], random_velocity);
//	}
//	sf::Texture heart_texture;
//	if (!heart_texture.loadFromFile("footpics/heart.png")) {
//		std::cerr << "Couldn't load the image!\n";
//	}
//	Heart heart(minigame_window, heart_texture, 0.1f);
//	Countdown countdown(minigame_window, 70, font, 5, 1.0f, sf::Color::White);
//	Countdown game_timer(minigame_window, 70, font, 21, 1.0f, sf::Color::White);
//
//	Panel minigame_ended(minigame_window, sf::Vector2f(0, 0), size, sf::Color::Transparent);
//	Texts points_string(minigame_window, 150, font, "", 20, sf::Color::White);
//	std::string points_text = "Your points: ";
//
//	sf::Clock frameClock;
//
//	while (minigame_window.isOpen()) {
//		float deltaTime = frameClock.restart().asSeconds();
//
//		sf::Event event;
//		while (minigame_window.pollEvent(event)) {
//			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
//				minigame_window.close();
//			}
//			heart.handleEvent(event);
//		}
//
//		heart.update(deltaTime);
//
//		countdown.update();
//		if (countdown.isFinished()) {
//			game_timer.update();
//			heart.checkCollision(obstacles);
//			for (auto& obstacle : obstacles) {
//				obstacle.movement();
//			}
//			for (size_t i = 0; i < obstacles.size(); ++i) {
//				for (size_t j = i + 1; j < obstacles.size(); ++j) {
//					obstacles[i].handleCollision(obstacles[j]);
//				}
//			}
//		}
//
//		minigame_window.clear();
//		minigame_window.draw(background.getSprite());
//		minigame_panel.drawing();
//
//		for (auto& obstacle : obstacles) {
//			obstacle.drawing();
//		}
//		heart.drawing();
//
//		if (!countdown.isFinished()) {
//			countdown.drawing();
//		}
//		else {
//			game_timer.drawing();
//			if (game_timer.isFinished()) {
//				point.add(heart.get_n());
//
//				std::string pom = point.get_minigame_points();
//				std::string your_points = points_text + pom;
//				points_string.update(your_points);
//
//				std::string choosing_stamp;
//				if (std::stoi(pom) < 0) {
//					choosing_stamp = "fail";
//					minigame_ended.addComponent(std::make_shared<Texts>(minigame_window, 50, font, "You successfully failed the minigame!", 24, sf::Color::Red));
//					minigame_ended.addComponent(std::make_shared<Texts>(minigame_window, 80, font, "You bring dishonor to our company!", 20, sf::Color::White));
//					minigame_ended.addComponent(std::make_shared<Buttons>(minigame_window, 300, font, "I am a crap!", 30, sf::Color::Red, [&minigame_window]() {
//						minigame_window.close();
//						}));
//				}
//				else {
//					choosing_stamp = "pass";
//					minigame_ended.addComponent(std::make_shared<Texts>(minigame_window, 50, font, "You successfully beat the minigame!", 25, sf::Color::Green));
//					minigame_ended.addComponent(std::make_shared<Texts>(minigame_window, 80, font, "You bring honor to our company!", 20, sf::Color::White));
//					minigame_ended.addComponent(std::make_shared<Buttons>(minigame_window, 300, font, "I am ready to bring some more!", 30, sf::Color::White, [&minigame_window]() {
//						minigame_window.close();
//						}));
//				}
//
//				SpriteManager stamp(size, choosing_stamp, sf::Vector2f(60, 60), sf::Vector2f(-1, 100));
//				sf::Sprite stamp_sprite = stamp.getSprite();
//				stamp_sprite.setRotation(angle);
//
//				while (minigame_window.isOpen()) {
//					sf::Event event1;
//					while (minigame_window.pollEvent(event1)) {
//						minigame_ended.handleEvent(event1);
//					}
//					minigame_window.clear(sf::Color::Black);
//					minigame_window.draw(stamp_sprite);
//					minigame_ended.drawing();
//					points_string.drawing();
//					minigame_window.display();
//				}
//			}
//		}
//
//		gameclock.update();
//		minigame_window.display();
//	}
//}
//
//std::string generatestring() {
//	const std::string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-=+[]{}\|;:<>,./?!@#$%^&*()";
//	const int length = 8;
//	std::random_device rd;
//	std::mt19937 gen(rd());
//	std::uniform_int_distribution<> dis(0, characters.size() - 1);
//
//	std::string random;
//	for (int i = 0; i < length; ++i) {
//		random += characters[dis(gen)];
//	}
//	return random;
//}
//
//void dontchangemywife(sf::RenderWindow& window, Points& points) {
//	sf::Vector2u CurrRes = window.getSize();
//	sf::Vector2f size(700, 600);
//	sf::Vector2f position(0, 0);
//	sf::Font font = fonts();
//	sf::RenderWindow wifi_window(sf::VideoMode(size.x, size.y), "Wifi", sf::Style::Titlebar);
//	wifi_window.setFramerateLimit(120);
//
//	bool finished = false;
//	Panel wifi(wifi_window, sf::Vector2f(210, 20), sf::Vector2f(CurrRes.x * (3 / 2), CurrRes.y / 2), sf::Color::Transparent);
//	std::string random_string = generatestring();
//	std::regex random(random_string);
//
//	try {
//		random = std::regex(random_string);
//	}
//	catch (const std::regex_error& e) {
//		std::cerr << "Regex error: " << e.what() << std::endl;
//		wifi.addComponent(std::make_shared<Texts>(wifi_window, 180, font, "Error in generating the password. Please try again.", 20, sf::Color::Red));
//		wifi.addComponent(std::make_shared<Buttons>(wifi_window, 230, "Try again", [&window, &points]() {
//			dontchangemywife(window, points);
//			}));
//		wifi_window.display();
//		while (wifi_window.isOpen()) {
//			sf::Event event;
//			while (wifi_window.pollEvent(event)) {
//				if (event.type == sf::Event::Closed) {
//					wifi_window.close();
//				}
//				wifi.handleEvent(event);
//			}
//		}
//		return;
//	}
//
//	wifi.addComponent(std::make_shared <Texts>(wifi_window, 80, font, "Enter the password: " + random_string, 30, sf::Color::White));
//	//wifi.addComponent(std::make_shared <Texts>(mainwindow, 100, font, "Your sequence: ", 20, sf::Color::White));
//	std::shared_ptr<Texts> input_text_component = std::make_shared<Texts>(wifi_window, 120, font, "Your password: ", 30, sf::Color::White);
//	wifi.addComponent(input_text_component);
//	//nie dzia³a :<
//	SpriteManager keypad(size, "keypad", sf::Vector2f(-1, 50), sf::Vector2f(-1, 200));
//	std::string input_string;
//	//bool correct = false;
//
//	while (wifi_window.isOpen()) {
//		sf::Event event;
//		while (wifi_window.pollEvent(event)) {
//			if (event.type == sf::Event::Closed) {
//				wifi_window.close();
//			}
//			if (event.type == sf::Event::TextEntered) {
//				if (event.text.unicode < 128 && event.text.unicode != 8) {  // 8 to kod Backspace
//					input_string += static_cast<char>(event.text.unicode);
//				}
//				else if (event.text.unicode == 8 && !input_string.empty()) {  // Obs³uga Backspace
//					input_string.pop_back();
//				}
//				//wifi.addComponent(std::make_shared <Texts>(mainwindow, 120, font, input_string, 20, sf::Color::White));
//				wifi.remove(input_text_component);
//				input_text_component = std::make_shared<Texts>(wifi_window, 120, font, "Your password: " + input_string, 30, sf::Color::White);
//				wifi.addComponent(input_text_component);
//			}
//			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return) {
//				finished = true;
//				if (finished == true) {
//					if (std::regex_match(input_string, random) == true) {
//						points.set_no_debuff();
//						wifi.addComponent(std::make_shared<Texts>(wifi_window, 180, font, "Success! You prevented the enemy from attacking.\n", 16, sf::Color::Green));
//						wifi.addComponent(std::make_shared<Buttons>(wifi_window, 230, "Yaay!", [&wifi, &wifi_window, &points]() {
//							//wifi.clear();
//							wifi_window.close();
//
//							//correct = true;
//							}));
//					}
//					else {
//						wifi.addComponent(std::make_shared<Texts>(wifi_window, 180, font, "Are you stupid? You're gonna get fired soon!.", 20, sf::Color::Red));
//						wifi.addComponent(std::make_shared<Buttons>(wifi_window, 230, "Try again", [&window, &wifi_window, &points]() {
//							wifi_window.close();
//							dontchangemywife(window, points);
//							//correct = false;
//							}));
//					}
//				}
//			}
//			wifi.handleEvent(event);
//		}
//		wifi_window.clear(sf::Color::Black);
//		wifi.drawing();
//		wifi_window.draw(keypad.getSprite());
//		wifi_window.display();
//	}
//	//panel.clear();
//	//random = generatestring();
//	//input.clear();
//	//panel.addComponent(std::make_shared<Texts>(mainwindow, 80, font, "Enter the sequence: " + random, 30, sf::Color::White));
//	//panel.addComponent(std::make_shared<Texts>(mainwindow, 120, font, "Your sequence: ", 30, sf::Color::White));
//}
//
//// Funkcja do animacji pojawiania i znikania tekstu
//void animateText(sf::RenderWindow& window, const std::string& textToAnimate, sf::Font& font) {
//	sf::Text text("", font, 30);
//	text.setFillColor(sf::Color::White);
//	text.setPosition(10.f, 10.f);
//
//	for (size_t i = 0; i < textToAnimate.size(); ++i) {
//		text.setString(textToAnimate.substr(0, i + 1));
//		window.clear(sf::Color::Black);
//		window.draw(text);
//		window.display();
//		sf::sleep(sf::milliseconds(100)); // OpóŸnienie dla animacji
//	}
//
//	bool removeText = false;
//	while (window.isOpen()) {
//		sf::Event event;
//		while (window.pollEvent(event)) {
//			if (event.type == sf::Event::Closed)
//				window.close();
//			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
//				removeText = true;
//		}
//	}
//}
//
//std::vector<std::string> loadtext(const fs::path& filepath, sf::RenderWindow& window) {
//	std::vector<std::string> lines;
//	std::ifstream file(filepath);
//	float width = (window.getSize().y) / static_cast<float>(2);
//
//	if (!file.is_open()) {
//		std::cerr << "Couldn't load file!\n";
//		return lines;
//	}
//
//	std::string pom;
//	while (std::getline(file, pom)) {
//		lines.push_back(pom);
//	}
//	file.close();
//	return lines;
//}
//
//void tutorial(sf::RenderWindow& mainwindow, Panel& welcome, sf::Font& font, float maxWidth) {
//	welcome.clear();
//	fs::path tutorialpath = "text/tutorial.txt";
//	std::vector<std::string> lines = loadtext(tutorialpath, mainwindow);
//	sf::Text text;
//	text.setFont(font);
//	text.setCharacterSize(30);
//	text.setFillColor(sf::Color::White);
//	sf::Texture mailbox;
//	if (!mailbox.loadFromFile("footpics/mailbox.png"))
//	{
//		std::cout << "Could not load image" << std::endl;
//	}
//
//	float width = mailbox.getSize().x;
//	float height = mailbox.getSize().y;
//	sf::Sprite sprite(mailbox);
//
//	sf::Vector2u windowSize = mainwindow.getSize();
//	float windowWidth = static_cast<float>(windowSize.x);
//	float windowHeight = static_cast<float>(windowSize.y);
//	float scale = windowWidth / width / 2;
//	sprite.setScale(scale, scale);
//	sprite.setPosition((windowWidth - width * scale) / 2, (windowHeight - height * scale) / 2);
//
//	// Draw sprite to the window
//
//	// Function to wrap text
//	auto wrapText = [&font, maxWidth](const std::string& str) {
//		std::vector<std::string> wrappedLines;
//		std::istringstream words(str);
//		std::string word;
//		std::string line;
//		sf::Text tempText;
//		tempText.setFont(font);
//		tempText.setCharacterSize(30);
//
//		while (words >> word) {
//			std::string testLine = line + (line.empty() ? "" : " ") + word;
//			tempText.setString(testLine);
//			if (tempText.getLocalBounds().width > maxWidth) {
//				wrappedLines.push_back(line);
//				line = word;
//			}
//			else {
//				line = testLine;
//			}
//		}
//		if (!line.empty()) {
//			wrappedLines.push_back(line);
//		}
//		return wrappedLines;
//	};
//
//	size_t currentLineIndex = 0;
//	size_t currentCharIndex = 0;
//	sf::Clock charClock;
//	bool waitingForSpace = false;
//	std::vector<std::string> wrappedLines;
//
//	// Initialize wrappedLines for the first set of lines
//	if (!lines.empty()) {
//		wrappedLines = wrapText(lines.front());
//	}
//
//	// Get window dimensions for positioning text
//
//	// Calculate positions
//	float startX = windowWidth / 4.0f; // X position at 1/4 of window width
//	float startY = 2.0f * windowHeight / 5.0f; // Y position at 2/5 of window height
//
//	while (!lines.empty()) { // Continue loop until all lines are displayed
//		sf::Event event;
//		while (mainwindow.pollEvent(event)) {
//			if (event.type == sf::Event::Closed) {
//				mainwindow.close();
//				return; // Exit function if window is closed
//			}
//			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
//				if (waitingForSpace) {
//					// Move to the next line
//					waitingForSpace = false;
//					currentLineIndex++;
//					currentCharIndex = 0;
//					charClock.restart();
//					if (currentLineIndex >= wrappedLines.size()) {
//						lines.erase(lines.begin());
//						if (!lines.empty()) {
//							wrappedLines = wrapText(lines.front());
//							currentLineIndex = 0;
//						}
//					}
//				}
//			}
//		}
//
//		if (!lines.empty() && currentLineIndex < wrappedLines.size()) {
//			std::string& currentLine = wrappedLines[currentLineIndex];
//			if (currentCharIndex < currentLine.length()) {
//				waitingForSpace = true;
//				if (charClock.getElapsedTime().asSeconds() >= 0.03f) {
//					currentCharIndex++;
//					charClock.restart();
//				}
//			}
//		}
//
//		mainwindow.clear(sf::Color::Black);
//		mainwindow.draw(sprite);
//		// Display all wrapped lines up to the current one
//		float yOffset = startY;
//		for (size_t i = 0; i <= currentLineIndex && i < wrappedLines.size(); ++i) {
//			std::string displayString = wrappedLines[i];
//			if (i == currentLineIndex) {
//				displayString = wrappedLines[i].substr(0, currentCharIndex);
//			}
//			text.setString(displayString);
//			text.setPosition(startX, yOffset);
//			mainwindow.draw(text);
//			yOffset += text.getCharacterSize() + 5; // Adjust line spacing as needed
//		}
//		mainwindow.display();
//	}
//
//	// Clear window and exit function when all text is displayed
//	mainwindow.clear(sf::Color::Black);
//	mainwindow.display();
//}
//
//void mailbox(sf::RenderWindow& window, float maxWidth, fs::path& path, std::string& name) {
//	sf::Vector2u CurrRes = window.getSize();
//	sf::Vector2f size(800, 800);
//	sf::Vector2f position(0, 0);
//	sf::Font font = fonts();
//	sf::RenderWindow mailbox_window(sf::VideoMode(size.x, size.y), "Mailbox", sf::Style::Close);
//	mailbox_window.setFramerateLimit(120);
//
//	bool finished = false;
//	Panel mailbox_panel(mailbox_window, sf::Vector2f(210, 20), sf::Vector2f(CurrRes.x * (3 / 2), CurrRes.y / 2), sf::Color::Black);
//	//mailbox_panel.addComponent()
//	mailbox_panel.addComponent(std::make_shared <Texts>(mailbox_window, 30, font, name, 35, sf::Color::Red));
//	std::vector<std::string> lines = loadtext(path, mailbox_window);
//	sf::Text text;
//	text.setFont(font);
//	text.setCharacterSize(30);
//	text.setFillColor(sf::Color::White);
//	sf::Texture mailbox;
//	if (!mailbox.loadFromFile("footpics/mailbox.png"))
//	{
//		std::cout << "Could not load image" << std::endl;
//	}
//
//	float width = mailbox.getSize().x;
//	float height = mailbox.getSize().y;
//	sf::Sprite sprite(mailbox);
//
//	sf::Vector2u windowSize = mailbox_window.getSize();
//	float windowWidth = static_cast<float>(windowSize.x);
//	float windowHeight = static_cast<float>(windowSize.y);
//	float scale = windowWidth / width;
//	sprite.setScale(scale, scale);
//	sprite.setPosition((windowWidth - width * scale) / 2, (windowHeight - height * scale) / 2);
//
//	// Draw sprite to the window
//
//	// Function to wrap text
//	auto wrapText = [&font, maxWidth](const std::string& str) {
//		std::vector<std::string> wrappedLines;
//		std::istringstream words(str);
//		std::string word;
//		std::string line;
//		sf::Text tempText;
//		tempText.setFont(font);
//		tempText.setCharacterSize(30);
//
//		while (words >> word) {
//			std::string testLine = line + (line.empty() ? "" : " ") + word;
//			tempText.setString(testLine);
//			if (tempText.getLocalBounds().width > maxWidth) {
//				wrappedLines.push_back(line);
//				line = word;
//			}
//			else {
//				line = testLine;
//			}
//		}
//		if (!line.empty()) {
//			wrappedLines.push_back(line);
//		}
//		return wrappedLines;
//	};
//
//	size_t currentLineIndex = 0;
//	size_t currentCharIndex = 0;
//	sf::Clock charClock;
//	bool waitingForSpace = false;
//	std::vector<std::string> wrappedLines;
//
//	// Initialize wrappedLines for the first set of lines
//	if (!lines.empty()) {
//		wrappedLines = wrapText(lines.front());
//	}
//
//	// Get window dimensions for positioning text
//
//	// Calculate positions
//	float startX = windowWidth / 6.0f; // X position at 1/4 of window width
//	float startY = 1.0f * windowHeight / 4.0f; // Y position at 1/4 of window height
//
//	while (!lines.empty()) { // Continue loop until all lines are displayed
//		sf::Event event;
//		while (mailbox_window.pollEvent(event)) {
//			if (event.type == sf::Event::Closed) {
//				mailbox_window.close();
//				return; // Exit function if window is closed
//			}
//			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
//				if (waitingForSpace) {
//					// Move to the next line
//					waitingForSpace = false;
//					currentLineIndex++;
//					currentCharIndex = 0;
//					charClock.restart();
//					if (currentLineIndex >= wrappedLines.size()) {
//						lines.erase(lines.begin());
//						if (!lines.empty()) {
//							wrappedLines = wrapText(lines.front());
//							currentLineIndex = 0;
//						}
//					}
//				}
//			}
//		}
//
//		if (!lines.empty() && currentLineIndex < wrappedLines.size()) {
//			std::string& currentLine = wrappedLines[currentLineIndex];
//			if (currentCharIndex < currentLine.length()) {
//				waitingForSpace = true;
//				if (charClock.getElapsedTime().asSeconds() >= 0.03f) {
//					currentCharIndex++;
//					charClock.restart();
//				}
//			}
//		}
//
//		mailbox_window.clear(sf::Color::Black);
//		mailbox_panel.drawing();
//		mailbox_window.draw(sprite);
//		// Display all wrapped lines up to the current one
//		float yOffset = startY;
//		for (size_t i = 0; i <= currentLineIndex && i < wrappedLines.size(); ++i) {
//			std::string displayString = wrappedLines[i];
//			if (i == currentLineIndex) {
//				displayString = wrappedLines[i].substr(0, currentCharIndex);
//			}
//			text.setString(displayString);
//			text.setPosition(startX, yOffset);
//			mailbox_window.draw(text);
//			yOffset += text.getCharacterSize() + 5; // Adjust line spacing as needed
//		}
//		mailbox_window.display();
//	}
//
//	// Clear window and exit function when all text is displayed
//	mailbox_window.clear(sf::Color::Black);
//	mailbox_window.display();
//}
//
//void window_settings(sf::RenderWindow& window) {
//	sf::Vector2f size(800, 800);
//	sf::Vector2f position(0, 0);
//	sf::Font font = fonts();
//	sf::RenderWindow settings(sf::VideoMode(size.x, size.y), "Settings", sf::Style::Close);
//	settings.setFramerateLimit(120);
//	sf::Vector2u CurrRes = window.getSize();
//
//	Panel settings_panel(settings, sf::Vector2f(210, 20), sf::Vector2f(window.getSize().x * (3 / 2), window.getSize().y / 2), sf::Color::Black);
//	settings_panel.addComponent(std::make_shared <Texts>(settings, 10, font, "Settings", 50, sf::Color::White));
//	settings_panel.addComponent(std::make_shared <Texts>(settings, 70, font, "Resolution:", 30, sf::Color::White));
//
//
//	settings_panel.addComponent(std::make_shared<Buttons>(settings, 100, "HD (800x600)", [&window]() {
//		window.setSize(sf::Vector2u(800, 600));
//		}));
//	settings_panel.addComponent(std::make_shared<Buttons>(settings, 150, "Full HD (1200x900)", [&window]() {
//		window.setSize(sf::Vector2u(1200, 900));
//		}));
//	settings_panel.addComponent(std::make_shared<Buttons>(settings, 200, "4K (1920x1080)", [&window]() {
//		window.setSize(sf::Vector2u(1920, 1080));
//		}));
//	settings_panel.addComponent(std::make_shared<Buttons>(settings, 250, "OK", [&settings]() {
//		settings.close();
//		}));
//
//	settings_panel.addComponent(std::make_shared<Texts>(settings, 300, font, "FPS limit:", 30, sf::Color::White));
//	settings_panel.addComponent(std::make_shared<Buttons>(settings, 340, "30", [&window]() {
//		window.setFramerateLimit(30);
//		}));
//	settings_panel.addComponent(std::make_shared<Buttons>(settings, 380, "45", [&window]() {
//		window.setFramerateLimit(45);
//		}));
//	settings_panel.addComponent(std::make_shared<Buttons>(settings, 420, "60", [&window]() {
//		window.setFramerateLimit(60);
//		}));
//	settings_panel.addComponent(std::make_shared<Buttons>(settings, 460, "OK", [&settings]() {
//		settings.close();
//		}));
//
//	while (settings.isOpen()) {
//		sf::Event event;
//		while (settings.pollEvent(event)) {
//			if (event.type == sf::Event::Closed) {
//				settings.close();
//			}
//			settings_panel.handleEvent(event);
//		}
//		settings.clear();
//		settings_panel.drawing();
//		settings.display();
//	}
//}
//
//void scale_with_window(sf::RenderWindow& mainwindow, sf::Texture& map, sf::Sprite& sprite, Panel& top_index, Panel& left_index, sf::Texture& wifi_icon, sf::Texture& settings_icon, sf::Texture& leave_icon, sf::Texture& bell, sf::Texture& mail, sf::Vector2f& CurrRes, Points& points) {
//	//top_index.clear();
//	//left_index.clear();
//	//sprite.setTexture(map);
//	//float windowWidth = mainwindow.getSize().x;
//	//float windowHeight = mainwindow.getSize().y;
//	//float mapWidth = map.getSize().x;
//	//float mapHeight = map.getSize().y;
//	//float scaleX = windowWidth / mapWidth;
//	//float scaleY = windowHeight / mapHeight;
//	//float scale = std::min(scaleX, scaleY);
//	//sprite.setScale(scale, scale);
//	//sprite.setPosition((windowWidth - mapWidth * scale) / 2, (windowHeight - mapHeight * scale) / 2);
//	//sf::Vector2f left_index_size((CurrRes.x) / 9, CurrRes.y);
//	////top_index.addComponent(std::make_shared<Buttons>(mainwindow, settings_icon, sf::Vector2f(((CurrRes.x - 110)), 10), sf::Vector2f(100, 100), []() {}));
//	//sf::Vector2f top_index_size(CurrRes.x, (CurrRes.y) / 8);
//	//top_index.addComponent(std::make_shared<Buttons>(mainwindow, settings_icon, sf::Vector2f(90, -1), sf::Vector2f(100, 100), top_index_size, sf::Vector2f(0, 0), [&mainwindow, &map, &sprite, &top_index, &left_index, &wifi_icon, &settings_icon, &leave_icon, &bell, &mail, &CurrRes, &points]() {
//	//	window_settings(mainwindow);
//	//	scale_with_window(mainwindow, map, sprite, top_index, left_index, wifi_icon, settings_icon, leave_icon, bell, mail, CurrRes, points);
//	//	}));
//	//Buttons wifi_button(mainwindow, wifi_icon, sf::Vector2f(80, -1), sf::Vector2f(100, 100), top_index_size, sf::Vector2f(0, 0),
//	//	[&mainwindow, &points]() {
//	//		if (points.get_debuff() == true) {
//	//
//	//			dontchangemywife(mainwindow, points);
//	//			points.set_no_debuff();
//	//		}
//	//	});
//	//points.clear_points();
//	//left_index.addComponent(std::make_shared<Buttons>(mainwindow, leave_icon, sf::Vector2f(-1, 85), sf::Vector2f(100, 100), left_index_size, sf::Vector2f(0, 0), [&mainwindow]() {mainwindow.close(); }));
//	//fs::path mailboxpath = "text/mail-box-guy-mail.txt";
//	//std::string mailboxname = "Mailbox Guy";
//	//left_index.addComponent(std::make_shared<Buttons>(mainwindow, mail, sf::Vector2f(-1, 75), sf::Vector2f(100, 100), left_index_size, sf::Vector2f(0, 0), [&mainwindow, &mailboxpath, &mailboxname]() {
//	//	mailbox(mainwindow, mainwindow.getSize().x / 2, mailboxpath, mailboxname);
//	//	}));
//	//left_index.addComponent(std::make_shared<Buttons>(mainwindow, bell, sf::Vector2f(-1, 65), sf::Vector2f(100, 100), left_index_size, sf::Vector2f(0, 0), [&mainwindow]() {
//	//	notification(mainwindow);
//	//	}));
//	//mainwindow.display();
//}
//
//void notification(sf::RenderWindow& window) {
//	sf::Vector2f size(800, 800);
//	sf::Vector2f position(0, 0);
//	sf::Font font = fonts();
//	sf::RenderWindow notification_window(sf::VideoMode(size.x, size.y), "Notifications", sf::Style::Close);
//	notification_window.setFramerateLimit(120);
//	sf::Vector2u CurrRes = window.getSize();
//	fs::path mailboxpath = "text/mail-box-guy-mail.txt";
//	std::string mailboxname = "Mailbox Guy";
//	fs::path bosspath = "text/boss_text.txt";
//	std::string bossname = "B O S S";
//	fs::path unknownpath = "text/unknown_text.txt";
//	std::string unknownname = "Unknown";
//
//	//Panel notification_panel(notification_window, sf::Vector2f(210, 20), sf::Vector2f(window.getSize().x * (3 / 2), window.getSize().y / 2), sf::Color::Black);
//	Buttons message1(notification_window, (0, 10), font, "MAILBOX GUY            (3 minutes ago)\nYou got a mail from Mailbox guy!",
//		30, sf::Color::White, [&window, &mailboxpath, &mailboxname]() {
//			mailbox(window, window.getSize().x / 2, mailboxpath, mailboxname);
//		});
//	int dziabka = 1;
//	message1.dziabka(sf::Color::Blue, dziabka);
//	Buttons message2(notification_window, (0, 70), font, "B        0      S        S            (10 minutes ago)\nGet to work. Don't make me regret hiring you.",
//		30, sf::Color::White, [&window, &bosspath, &bossname]() {
//			mailbox(window, window.getSize().x / 2, bosspath, bossname);
//		});
//	message2.dziabka(sf::Color::Blue, dziabka);
//
//	Buttons message3(notification_window, (0, 130), font, "Unknown            (1 hour ago)\nWe've found you.",
//		30, sf::Color::White, [&window, &unknownpath, &unknownname]() {
//			mailbox(window, window.getSize().x / 2, unknownpath, unknownname);
//		});
//	message3.dziabka(sf::Color::Blue, dziabka);
//
//	while (notification_window.isOpen()) {
//		sf::Event event;
//		while (notification_window.pollEvent(event)) {
//			if (event.type == sf::Event::Closed) {
//				notification_window.close();
//			}
//			message1.handleEvent(event);
//			message2.handleEvent(event);
//			message3.handleEvent(event);
//		}
//		notification_window.clear();
//		message1.drawing();
//		message2.drawing();
//		message3.drawing();
//		notification_window.display();
//	}
//}
//
//void endgame(sf::RenderWindow& window, Points& points, std::string& name) {
//	fs::path record_path = "text/records.txt";
//
//	std::ofstream records(record_path, std::ios_base::app);
//	if (!records.is_open())
//	{
//		std::cerr << "Couldn't load file!\n";
//	}
//
//	if (records.is_open()) {
//		records << name << "\n";
//		records << points.get_overall_points() << "\n";
//		records.close();
//	}
//}
//
//std::string save_records(sf::RenderWindow& window, Points& points, Panel& records_panel) {
//	sf::Font font = fonts();
//
//	std::vector<std::string> lines = loadtext("text/records.txt", window);
//
//	std::string username;
//	//wifi.addComponent(std::make_shared <Texts>(mainwindow, 100, font, "Your sequence: ", 20, sf::Color::White));
//	std::shared_ptr<Texts> input_text_component = std::make_shared<Texts>(window, window.getSize().y / 2.25, font, "Enter username and press OK: ", 30, sf::Color::White);
//	records_panel.addComponent(input_text_component);
//
//	while (window.isOpen()) {
//		sf::Event event;
//		while (window.pollEvent(event)) {
//			if (event.type == sf::Event::Closed) {
//				window.close();
//			}
//			if (event.type == sf::Event::TextEntered) {
//				if (event.text.unicode < 128 && event.text.unicode != 8) {  // 8 to kod Backspace
//					username += static_cast<char>(event.text.unicode);
//				}
//				else if (event.text.unicode == 8 && !username.empty()) {  // Obs³uga Backspace
//					username.pop_back();
//				}
//				//wifi.addComponent(std::make_shared <Texts>(mainwindow, 120, font, input_string, 20, sf::Color::White));
//				records_panel.remove(input_text_component);
//				input_text_component = std::make_shared<Texts>(window, window.getSize().y / 2.25, font, "Enter username and press OK: " + username, 30, sf::Color::White);
//				records_panel.addComponent(input_text_component);
//				records_panel.addComponent(std::make_shared<Buttons>(window, window.getSize().y / 2, "Ok", [&window]() {
//					window.close();
//					}));
//			}
//			//if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
//			//
//			//	endgame(window, points, username);
//			//	records_panel.addComponent(std::make_shared<Buttons>(window, window.getSize().y / 2, "Exit", [&window]() {
//			//				window.close();
//			//	}));
//			//}
//			records_panel.handleEvent(event);
//		}
//		window.clear(sf::Color::Black);
//		records_panel.drawing();
//		window.display();
//	}
//	return username;
//}
//
//void show_records(sf::RenderWindow& window, Panel& panel) {
//	sf::Font font = fonts();
//	bool finished = false;
//
//	// Dodanie nag³ówka
//	sf::Text header;
//	header.setFont(font);
//	header.setString("USERNAME\t\t|\t\tPOINTS");
//	header.setCharacterSize(30);
//	header.setFillColor(sf::Color::White);
//	header.setPosition(window.getSize().x / 4.f, 50.f);
//
//	std::vector<std::string> lines = loadtext("text/records.txt", window);
//
//	std::vector<std::pair<std::string, int>> records;
//
//	// Wczytywanie danych na zmianê
//	for (size_t i = 0; i + 1 < lines.size(); i += 2) {
//		std::string username = lines[i];
//		int points = std::stoi(lines[i + 1]);
//		records.emplace_back(username, points);
//	}
//
//	// Sortowanie wyników malej¹co wed³ug punktów
//	std::sort(records.begin(), records.end(), [](const auto& a, const auto& b) {
//		return a.second > b.second;
//		});
//
//	std::vector<sf::Text> usernameTexts;
//	std::vector<sf::Text> pointTexts;
//	float startY = 100.f;
//	float lineHeight = 40.f;
//
//	// Ustalenie rozmiaru okna przewijania
//	sf::View view(sf::FloatRect(0.f, 0.f, static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)));
//
//	for (const auto& record : records) {
//		sf::Text usernameText;
//		usernameText.setFont(font);
//		usernameText.setString(record.first);
//		usernameText.setCharacterSize(25);
//		usernameText.setFillColor(sf::Color::White);
//		usernameText.setPosition(200.f, startY);  // Przesuniêcie bardziej na prawo
//
//		sf::Text pointText;
//		pointText.setFont(font);
//		pointText.setString(std::to_string(record.second));
//		pointText.setCharacterSize(25);
//		pointText.setFillColor(sf::Color::White);
//		pointText.setPosition(window.getSize().x - 200.f, startY);
//
//		usernameTexts.push_back(usernameText);
//		pointTexts.push_back(pointText);
//		startY += lineHeight;
//	}
//
//	float maxScroll = std::max(0.f, startY - window.getSize().y);
//
//	// Dodanie przycisku Exit na koñcu listy
//	std::shared_ptr<Buttons> exitButton = std::make_shared<Buttons>(window, window.getSize().y / 300, "<- Exit", [&window, &finished]() {
//		finished = true;
//		});
//	panel.addComponent(exitButton);
//
//	while (window.isOpen() && !finished) {
//		sf::Event event;
//		while (window.pollEvent(event)) {
//			if (event.type == sf::Event::Closed) {
//				window.close();
//			}
//
//			// Obs³uga scrollowania
//			if (event.type == sf::Event::MouseWheelScrolled) {
//				if (event.mouseWheelScroll.delta < 0 && view.getCenter().y < maxScroll + window.getSize().y / 2) {  // Przewijanie w dó³
//					view.move(0.f, 40.f);
//				}
//				else if (event.mouseWheelScroll.delta > 0 && view.getCenter().y > window.getSize().y / 2) {  // Przewijanie w górê
//					view.move(0.f, -40.f);
//				}
//			}
//
//			panel.handleEvent(event);
//		}
//
//		window.clear(sf::Color::Transparent);
//		window.setView(view); // Ustawienie widoku na okno przewijania
//
//		panel.drawing();
//		window.draw(header);
//
//		// Rysowanie tekstu
//		for (size_t i = 0; i < usernameTexts.size(); ++i) {
//			window.draw(usernameTexts[i]);
//			window.draw(pointTexts[i]);
//		}
//
//		window.display();
//	}
//}
//
//
//void resetGame(sf::RenderWindow& mainwindow, sf::Texture& map, sf::Sprite& sprite, Panel& top_index, Panel& left_index, GameClock& gameclock, bool& showAction, sf::Texture& wifi_icon, sf::Texture& settings_icon, sf::Texture& leave_icon, sf::Texture& bell, sf::Texture& mail, sf::Vector2f& CurrRes, Points& points, Texts& point_text) {
//	gameclock.reset();
//	//gameclock.start();
//	sf::Vector2f position(mainwindow.getSize().x - 370, 40);
//	gameclock.move(position);
//	top_index.clear();
//	left_index.clear();
//
//	// Recreate map sprite
//	sprite.setTexture(map);
//	float windowWidth = mainwindow.getSize().x;
//	float windowHeight = mainwindow.getSize().y;
//	float mapWidth = map.getSize().x;
//	float mapHeight = map.getSize().y;
//	float scaleX = windowWidth / mapWidth;
//	float scaleY = windowHeight / mapHeight;
//	float scale = std::min(scaleX, scaleY);
//	sprite.setScale(scale, scale);
//	float maxAllowedX = windowWidth - (windowWidth / 9.0f);
//	float maxAllowedY = windowHeight - (windowHeight / 8.0f);
//
//	float posX = std::max(windowWidth - mapWidth * scale, maxAllowedX - mapWidth * scale);
//	float posY = std::max(windowHeight * (1.0f / 8.0f), maxAllowedY - mapHeight * scale);
//	sprite.setPosition(posX, posY);
//
//	// Reset UI panels
//	sf::Vector2f left_index_size((CurrRes.x) / 9, CurrRes.y);
//	//top_index.addComponent(std::make_shared<Buttons>(mainwindow, settings_icon, sf::Vector2f(((CurrRes.x - 110)), 10), sf::Vector2f(100, 100), []() {}));
//	sf::Vector2f top_index_size(CurrRes.x, (CurrRes.y) / 8);
//	top_index.addComponent(std::make_shared<Buttons>(mainwindow, settings_icon, sf::Vector2f(90, -1), sf::Vector2f(100, 100), top_index_size, sf::Vector2f(0, 0), []() {}));
//	Buttons wifi_button(mainwindow, wifi_icon, sf::Vector2f(80, -1), sf::Vector2f(100, 100), top_index_size, sf::Vector2f(0, 0),
//		[&mainwindow, &points]() {
//			if (points.get_debuff() == true) {
//
//				dontchangemywife(mainwindow, points);
//				points.set_no_debuff();
//			}
//		});
//	points = Points();
//	std::string	pom = "Points: " + points.get_overall_points() + " m";
//	point_text.update(pom);
//
//	left_index.addComponent(std::make_shared<Buttons>(mainwindow, leave_icon, sf::Vector2f(-1, 85), sf::Vector2f(100, 100), left_index_size, sf::Vector2f(0, 0), [&mainwindow]() {mainwindow.close(); }));
//	fs::path mailboxpath = "text/mail-box-guy-mail.txt";
//	std::string mailboxname = "Mailbox Guy";
//	left_index.addComponent(std::make_shared<Buttons>(mainwindow, mail, sf::Vector2f(-1, 75), sf::Vector2f(100, 100), left_index_size, sf::Vector2f(0, 0), [&mainwindow, &mailboxpath, &mailboxname]() {
//		mailbox(mainwindow, mainwindow.getSize().x / 2, mailboxpath, mailboxname);
//		}));
//	left_index.addComponent(std::make_shared<Buttons>(mainwindow, bell, sf::Vector2f(-1, 65), sf::Vector2f(100, 100), left_index_size, sf::Vector2f(0, 0), [&mainwindow]() {
//		notification(mainwindow);
//		}));
//
//	showAction = false;
//}