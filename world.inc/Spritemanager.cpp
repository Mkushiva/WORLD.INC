#include "Spritemanager.h"

SpriteManager::SpriteManager(sf::Vector2f windowSize, const std::string& fileName, sf::Vector2f pos, sf::Vector2f size)
    : windowSize(windowSize), fileName(fileName), pos(pos), size(size) {
    if (!texture.loadFromFile("footpics/" + fileName + ".png")) {
        std::cerr << "Could not load image" << std::endl;
    }
    else {
        texture.setSmooth(false);
        sprite.setTexture(texture);
        /*float res_multiplier = sqrt(static_cast<float>(windowSize.x) / 1200.0f);
        size = size * res_multiplier;*/
        float windowWidth = windowSize.x;
        float windowHeight = windowSize.y;

        float spriteWidth = texture.getSize().x;
        float spriteHeight = texture.getSize().y;
        float scale;
        if (size.y < 0) {
            scale = size.x / spriteWidth;
            size.x = scale;
            size.y = scale;
        }
        else if (size.x < 0) {
            scale = size.y / spriteHeight;
            size.x = scale;
            size.y = scale;
        }
        else if (size.x < 0 && size.y < 0) {
            scale = 1;
            size.x = scale;
            size.y = scale;
        }
        else {
            size.x = size.x / spriteWidth;
            size.y = size.y / spriteHeight;
        }
        sprite.setScale(size);

        float posX = pos.x >= 0 ? (pos.x / 100.0f) * windowSize.x : (windowSize.x - sprite.getGlobalBounds().width) / 2.0f;
        float posY = pos.y >= 0 ? (pos.y / 100.0f) * windowSize.y : (windowSize.y - sprite.getGlobalBounds().height) / 2.0f;

        sprite.setPosition(posX, posY);
    }
}

sf::Sprite SpriteManager::getSprite() const {
    return sprite;
}