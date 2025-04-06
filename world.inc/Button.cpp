#include "Button.h"

//#include "Functions.h"


////////////////////////////////////////fonts
sf::Font fonts() {
	sf::Font font;
	if (!font.loadFromFile("fonts/ARCHITXT.ttf"))
	{
		std::cerr << "not working UmU" << std::endl;
		// Error handling
	}
	return font;
}
sf::Font& ResourceManager::getFont() {
	static sf::Font font;
	static bool loaded = false;
	if (!loaded) {
		if (!font.loadFromFile("fonts/ARCHITXT.ttf")) {
			std::cerr << "Failed to load font." << std::endl;
		}
		loaded = true;
	}
	return font;
}

///////////////////////////////////////PANEL

Panel::Panel(sf::RenderWindow& window, const sf::Vector2f& position, const sf::Vector2f& size, sf::Color color) : window(window), is_background_sprite(false) {
	background.setPosition(position);
	background.setSize(size);
	background.setFillColor(sf::Color(color));
}

Panel::Panel(sf::RenderWindow& window, const sf::Vector2f& position, const sf::Vector2f& size, sf::Texture& texture) : window(window), is_background_sprite(true) {
	sprite.setTexture(texture);
	sprite.setPosition(position);

	// Scale the sprite to fit the panel size, maintaining aspect ratio
	float scaleX = size.x / texture.getSize().x;
	float scaleY = size.y / texture.getSize().y;
	sprite.setScale(scaleX, scaleY);
}

void Panel::addComponent(std::shared_ptr<UIComponent> component) {
	components.push_back(component);
}

void Panel::clear() {
	components.clear();
}

void Panel::remove(std::shared_ptr<UIComponent> component) {
	components.erase(
		std::remove_if(components.begin(), components.end(),
			[&](const std::shared_ptr<UIComponent>& comp) { return comp == component; }),
		components.end());
}

////////////////////////////////////////BUTTONS

Buttons::Buttons(sf::RenderWindow& window, int y, sf::Font& font, const std::string& str, const int& char_size, const sf::Color& color, std::function<void()> callback)
	: Buttons(window, sf::Vector2f(static_cast<float>(window.getSize().x/2), static_cast<float>(y)), font, str, char_size, color, callback) {}

// Default font, color, and size
Buttons::Buttons(sf::RenderWindow& window, int y, const std::string& str, std::function<void()> callback)
	: Buttons(window, y, ResourceManager::getFont(), str, def_FontSize, def_FontColor, callback) {}

Buttons::Buttons(sf::RenderWindow& window, sf::Vector2f position, sf::Font& font, const std::string& str, const int& char_size, const sf::Color& color, std::function<void()> callback)
	: window(window), onClick(callback), isText(true) {

	scale = NULL;
	text.setFont(font); // Set the font to our 
	text.setString(str); // Set the string
	float scaled_char_size = char_size;
	if (window.getSize().y > 500) {
		scaled_char_size = char_size * (static_cast<float>(window.getSize().x) / 1200.0f);
	}

	text.setCharacterSize(scaled_char_size); // Set the character size
	text.setFillColor(color); // Set the text color

	// Center text
	sf::FloatRect textBounds = text.getLocalBounds();
	text.setOrigin(textBounds.left + textBounds.width / 2.f, textBounds.top + textBounds.height / 2.f);
	text.setPosition(position);

	// Set button size and position based on text size
	button.setSize(sf::Vector2f(textBounds.width + 10.f, textBounds.height + 20.f)); // Added padding around text
	button.setOrigin(button.getSize().x / 2.f, button.getSize().y / 2.f);
	button.setPosition(position);

	button.setOutlineColor(sf::Color(96, 96, 96));
	button.setOutlineThickness(1.f);
	button.setFillColor(sf::Color::Transparent);
}


Buttons::Buttons(sf::RenderWindow& window, sf::Texture &texture, sf::Vector2f position, sf::Vector2f size, std::function<void()> callback)
	: window(window), isText(false), onClick(callback){
	buttonTexture.setTexture(texture);
	//buttonTexture.setPosition(position);
	sf::Vector2f scaled_pos(window.getSize().x*position.x/100, window.getSize().y * position.y/100);

	float res_multiplier = sqrt(static_cast<float>(window.getSize().x) / 1200.0f);

	originalPosition = scaled_pos;
	originalSize = size * res_multiplier;
	scale = size.x / texture.getSize().x * res_multiplier;
	
	buttonTexture.setPosition(scaled_pos);
	//float scaleY = size.y / texture.getSize().y;
	buttonTexture.setScale(scale, scale);
	button.setSize(originalSize);
	button.setPosition(scaled_pos);
	button.setFillColor(sf::Color::Transparent);

	//if (!buffer.loadFromFile(soundFile)) {
	//	std::cerr << "Couldn't load from file!\n";
	//}
	//sound.setBuffer(buffer);
}
Buttons::Buttons(sf::RenderWindow& window, sf::Texture& texture, sf::Vector2f position, sf::Vector2f size, sf::Vector2f parent_panel_size, sf::Vector2f parent_panel_pos, std::function<void()> callback)
	: window(window), isText(false), onClick(callback){
		buttonTexture.setTexture(texture);
		//buttonTexture.setPosition(position);

		float res_multiplier = sqrt(static_cast<float>(window.getSize().x) / 1200.0f);
		originalSize = size * res_multiplier;
		
		float x, y;
		if(position.x<0){
			x = parent_panel_pos.x + (parent_panel_size.x / 2.0f) - (originalSize.x / 2.0f);
			y = window.getSize().y * position.y / 100;
		}
		else if(position.y < 0){
			x = window.getSize().x * position.x / 100;
			y = parent_panel_pos.y + (parent_panel_size.y / 2.0f) - (originalSize.y / 2.0f);
		}
		else {
			x = window.getSize().x * position.x / 100;
			y = window.getSize().y * position.y / 100;
		}
		sf::Vector2f scaled_pos(x, y);
		

		originalPosition = scaled_pos;
		scale = size.x / texture.getSize().x * res_multiplier;

		buttonTexture.setPosition(scaled_pos);
		//float scaleY = size.y / texture.getSize().y;
		buttonTexture.setScale(scale, scale);
		button.setSize(originalSize);
		button.setPosition(originalPosition);
		button.setFillColor(sf::Color::Transparent);

		//if (!buffer.loadFromFile(soundFile)) {
		//	std::cerr << "Couldn't load from file!\n";
		//}
		//sound.setBuffer(buffer);
}

bool Buttons::isShown() {
	return showButton;
}

void Buttons::handleEvent(const sf::Event& event) {
	if (event.type == sf::Event::MouseMoved) {
		sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
		sf::Vector2f mousePositionF(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));
		if (button.getGlobalBounds().contains(mousePositionF)) {
			showButton = true; // Hide the action sprite
			if(!isText){
				float newSize = scale * 1.1f; 
				sf::Vector2f scaleFactor(newSize, newSize); 
				buttonTexture.setScale(scaleFactor); 
				sf::FloatRect bounds = buttonTexture.getGlobalBounds();
				sf::Vector2f newPosition = originalPosition - (sf::Vector2f(bounds.width, bounds.height) - originalSize) / 2.0f;
				buttonTexture.setPosition(newPosition); 
			}
		}
		else if (button.getOutlineColor() != sf::Color::Red) {
			showButton = false;
			if (!isText) {
			buttonTexture.setScale(scale, scale);
			buttonTexture.setPosition(originalPosition);
			}
		}
	}
	if (event.type == sf::Event::MouseButtonPressed) {
		if (event.mouseButton.button == sf::Mouse::Left) {
			sf::Vector2i mousePos = sf::Mouse::getPosition(window);
			sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

			// Check if the click is within the bounds of the action sprite
			if (button.getGlobalBounds().contains(mousePosF)) {
				button.setOutlineColor(sf::Color::Red);
				pressed = true;
				//sound.play();
				onClick();
			}
			else if (pressed) {
				button.setOutlineColor(sf::Color(96, 96, 96));
				pressed = false;
			}

		}
	}
};
void Buttons::drawing() {
	if (isText) {
		window.draw(text);
		if (showButton) {
			window.draw(button);
		}
	}
	else {
		window.draw(button);
		window.draw(buttonTexture);
	}
};
bool Buttons::isPressed() {
	return pressed;
}

void Buttons::update_color(sf::Color color) {
	buttonTexture.setColor(color);
}

sf::Color Buttons::getColor() {
	return buttonTexture.getColor();
}

void Buttons::dziabka(const sf::Color& color, int& thicc) {
	button.setOutlineColor(color);
	button.setOutlineThickness(thicc);
}

////////////////////////////////////////TEXTS

Texts::Texts(sf::RenderWindow& window, int y, sf::Font& font, const std::string& str, const int& char_size, const sf::Color& color)
	: Texts(window, y, font, str, char_size, color, []() {}) {}

Texts::Texts(sf::RenderWindow& window, int y, sf::Font& font, const std::string& str, const int& char_size, const sf::Color& color, std::function<void()> callback)
	: onClick(callback), window(window), y(y) {
	sf::Vector2u size = window.getSize();
	int x = size.x;
	text.setFont(font); 
	text.setString(str); 
	text.setCharacterSize(char_size); 
	text.setFillColor(color); 
	sf::FloatRect set_textSize = text.getLocalBounds();
	int width = set_textSize.width;
	text.setPosition(x / static_cast<float>(2) - width / static_cast<float>(2), y);
}

void Texts::drawing() {
	window.draw(text);
}

void Texts::update(std::string& str) {
	text.setString(str);
	sf::Vector2u size = window.getSize();
	int x = size.x;
	sf::FloatRect set_textSize = text.getLocalBounds();
	int width = set_textSize.width;
	text.setPosition(x / static_cast<float>(2) - width / static_cast<float>(2), y);
}

////////////////////////////////////////countdown

Countdown::Countdown(sf::RenderWindow& window, int y, sf::Font& font, int startValue, float intervalSec, const sf::Color& color)
	: Texts(window, y, font, std::to_string(startValue), 24, color), countdown(startValue), interval(intervalSec), finished(false) {
	text.setString(std::to_string(countdown));
}

void Countdown::update() {
	if (!finished && clock.getElapsedTime().asSeconds() >= interval) {
		countdown--;
		if (countdown < 0) {
			finished = true;
			countdown = 0;
		}
		else {
			text.setString(std::to_string(countdown));
			clock.restart();
		}
	}
}

bool Countdown::isFinished() const {
	return finished;
}

int Countdown::get_n() {
	return 10 - countdown;
}

////////////////////////////////////////obstacle

Obstacle::Obstacle(sf::Vector2f pos, sf::RenderWindow& win, std::shared_ptr<sf::Texture>& tex, sf::Vector2f velo)
	: window(win), position(pos), x_velocity(velo.x), y_velocity(velo.y) {
	sprite.setTexture(*tex);
	sprite.setScale(0.8f, 0.8f);
	sprite.setPosition(position);
}

// Konstruktor kopiuj¹cy
Obstacle::Obstacle(const Obstacle& other)
	: window(other.window), position(other.position), sprite(other.sprite), x_velocity(other.x_velocity), y_velocity(other.y_velocity) {}


Obstacle::~Obstacle() {
	stop();
}

void Obstacle::start() {
	obstacle_thread = std::thread(&Obstacle::run, this);
}

void Obstacle::stop() {
	running = false;
	if (obstacle_thread.joinable()) {
		obstacle_thread.join();
	}
}

void Obstacle::run() {
	while (running) {
		std::this_thread::sleep_for(std::chrono::milliseconds(16)); // Ustawienie opóŸnienia dla w¹tku
		movement();
	}
}

void Obstacle::movement() {
	std::lock_guard<std::mutex> lock(mtx); // Synchronizacja aktualizacji
	position.x += x_velocity;
	position.y += y_velocity;

	// Odbicie od krawêdzi
	if ((position.x <= 0 && x_velocity < 0) || (position.x + sprite.getGlobalBounds().width >= window.getSize().x) && x_velocity > 0) {
		x_velocity = -x_velocity;
	}

	if ((position.y <= 0 && y_velocity < 0) || (position.y + sprite.getGlobalBounds().width >= window.getSize().y) && y_velocity > 0) {
		y_velocity = -y_velocity;
	}

	sprite.setPosition(position);
}

bool Obstacle::checkCollision(const sf::Sprite& other) const {
	std::lock_guard<std::mutex> lock(mtx); // Synchronizacja odczytu
	return sprite.getGlobalBounds().intersects(other.getGlobalBounds());
}

bool Obstacle::checkCollision(const Obstacle& other) const {
	std::lock_guard<std::mutex> lock(mtx); // Synchronizacja odczytu
	return sprite.getGlobalBounds().intersects(other.sprite.getGlobalBounds());
}

bool Obstacle::checkCollision(const sf::RectangleShape& other) const {
	std::lock_guard<std::mutex> lock(mtx); // Synchronizacja odczytu
	return sprite.getGlobalBounds().intersects(other.getGlobalBounds());
}

void Obstacle::handleCollision(Obstacle& other) {
	if (checkCollision(other)) {
		std::lock_guard<std::mutex> lock(mtx); // Synchronizacja aktualizacji
		std::lock_guard<std::mutex> other_lock(other.mtx);

		sf::FloatRect thisBounds = sprite.getGlobalBounds();
		sf::FloatRect otherBounds = other.sprite.getGlobalBounds();

		float overlapLeft = thisBounds.left + thisBounds.width - otherBounds.left;
		float overlapRight = otherBounds.left + otherBounds.width - thisBounds.left;
		float overlapTop = thisBounds.top + thisBounds.height - otherBounds.top;
		float overlapBottom = otherBounds.top + otherBounds.height - thisBounds.top;

		// Obliczenie wektora normalnego do powierzchni kolizji
		sf::Vector2f normalVector;
		if (std::abs(overlapLeft) < std::abs(overlapRight)) {
			normalVector.x = overlapLeft < 0 ? -1.0f : 1.0f;
		}
		else {
			normalVector.x = overlapRight < 0 ? 1.0f : -1.0f;
		}
		if (std::abs(overlapTop) < std::abs(overlapBottom)) {
			normalVector.y = overlapTop < 0 ? -1.0f : 1.0f;
		}
		else {
			normalVector.y = overlapBottom < 0 ? 1.0f : -1.0f;
		}

		// Obliczenie prêdkoœci wzglêdem osi x i y
		float v_x1_initial = x_velocity;
		float v_x2_initial = other.x_velocity;
		float v_y1_initial = y_velocity;
		float v_y2_initial = other.y_velocity;
		float m1 = 1.0f, m2 = m1;

		// Obliczenie nowych prêdkoœci po kolizji
		float v_x1_final = ((m1 - m2) * v_x1_initial + 2 * m2 * v_x2_initial) / (m1 + m2);
		float v_x2_final = ((m2 - m1) * v_x2_initial + 2 * m1 * v_x1_initial) / (m1 + m2);
		float v_y1_final = ((m1 - m2) * v_y1_initial + 2 * m2 * v_y2_initial) / (m1 + m2);
		float v_y2_final = ((m2 - m1) * v_y2_initial + 2 * m1 * v_y1_initial) / (m1 + m2);

		// Ograniczenie prêdkoœci
		float min_speed = 1.0f;
		float max_speed = 3.0f;

		// Ograniczenie prêdkoœci na osi x
		if (std::abs(v_x1_final) < min_speed) {
			v_x1_final = std::copysign(min_speed, v_x1_final);
		}
		else if (std::abs(v_x1_final) > max_speed) {
			v_x1_final = std::copysign(max_speed, v_x1_final);
		}
		if (std::abs(v_x2_final) < min_speed) {
			v_x2_final = std::copysign(min_speed, v_x2_final);
		}
		else if (std::abs(v_x2_final) > max_speed) {
			v_x2_final = std::copysign(max_speed, v_x2_final);
		}

		// Ograniczenie prêdkoœci na osi y
		if (std::abs(v_y1_final) < min_speed) {
			v_y1_final = std::copysign(min_speed, v_y1_final);
		}
		else if (std::abs(v_y1_final) > max_speed) {
			v_y1_final = std::copysign(max_speed, v_y1_final);
		}
		if (std::abs(v_y2_final) < min_speed) {
			v_y2_final = std::copysign(min_speed, v_y2_final);
		}
		else if (std::abs(v_y2_final) > max_speed) {
			v_y2_final = std::copysign(max_speed, v_y2_final);
		}

		// Aktualizacja prêdkoœci obiektów
		x_velocity = v_x1_final;
		other.x_velocity = v_x2_final;
		y_velocity = v_y1_final;
		other.y_velocity = v_y2_final;

		sprite.setPosition(position);
		other.sprite.setPosition(other.position);
	}
}

sf::Vector2f Obstacle::get_position() const {
	std::lock_guard<std::mutex> lock(mtx); // Synchronizacja odczytu
	return position;
}

void Obstacle::drawing_const() const {
	std::lock_guard<std::mutex> lock(mtx); // Synchronizacja rysowania
	window.draw(sprite);
}

void Obstacle::drawing() {
	std::lock_guard<std::mutex> lock(mtx); // Synchronizacja rysowania
	window.draw(sprite);
}

void Obstacle::handleEvent(const sf::Event& event) {}


///////////////////////////////////////////////////heart

Heart::Heart() : window(nullptr) {
	if (!texture.loadFromFile("footpics/heart.png")) {
		std::cerr << "Couldn't load the image!\n";
	}
	sprite.setTexture(texture);
	sprite.setColor(sf::Color::Red);
	sprite.setScale(0.1f, 0.1f);
	n = 0;
}

Heart::Heart(sf::RenderWindow& win, sf::Texture& texture, float scale) : window(&win) {
	sprite.setTexture(texture);
	sprite.setColor(sf::Color::Red);
	sprite.setScale(scale, scale);
	n = 0;
	setPos();
}
void Heart::setWindow(sf::RenderWindow& win) {
	window = &win;
	setPos();
}

void Heart::setPos() {
	sf::Vector2f pom(window->getSize());
	position = sf::Vector2f(pom.x / 2.0f - sprite.getGlobalBounds().width / 2.0f, pom.y / 2.0f - sprite.getGlobalBounds().height / 2.0f);
	sprite.setPosition(position);
}

void Heart::handleEvent(const sf::Event& event) {
	if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased) {
		updateVelocity();
	}
}

void Heart::update(float deltaTime) {
	if (collision && clock.getElapsedTime().asSeconds() >= invisibleDuration) {
		collision = false;
		sprite.setColor(sf::Color::Red);
	}

	position += velocity * deltaTime;
	bounderies();
	sprite.setPosition(position);
}

void Heart::checkCollision(std::vector<Obstacle>& obstacles) {
	if (collision && clock.getElapsedTime().asSeconds() >= invisibleDuration) {
		collision = false;
		sprite.setColor(sf::Color::Red);
	}
	for (auto& obstacle : obstacles) {
		if (obstacle.checkCollision(sprite) && clock.getElapsedTime().asSeconds() > 1.0) {
			collision = true;
			n++;
			clock.restart();
			sprite.setColor(sf::Color::Transparent);
			break;
		}
	}
}

int Heart::get_n() {
	return n;
}

void Heart::bounderies() {
	if (position.x < 0) {
		position.x = 0;
	}
	if (position.y < 0) {
		position.y = 0;
	}
	if (position.x + sprite.getGlobalBounds().width > window->getSize().x) {
		position.x = window->getSize().x - sprite.getGlobalBounds().width;
	}
	if (position.y + sprite.getGlobalBounds().height > window->getSize().y) {
		position.y = window->getSize().y - sprite.getGlobalBounds().height;
	}
}

void Heart::drawing() {
	window->draw(sprite);
}







ModifiedHeart::ModifiedHeart(sf::RenderWindow& win, sf::Texture& texture, float scale) : Heart(win, texture, scale) {
	setPos();
	sprite.setColor(sf::Color::White);
}

void ModifiedHeart::setPos() {
	if (window) {
		sf::Vector2f pom(window->getSize());
		position = sf::Vector2f(pom.x / 2.0f - sprite.getGlobalBounds().width / 2.0f, pom.y - sprite.getGlobalBounds().height - 10.0f);
		sprite.setPosition(position);
	}
}

void ModifiedHeart::set_sprite(sf::Texture textur) {
	sprite.setTexture(textur);
	sprite.setScale(0.2f, 0.2f);
	setPos();
}

sf::Sprite ModifiedHeart::getSprite() const {
	return sprite;
}
