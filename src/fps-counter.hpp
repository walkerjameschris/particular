#pragma once

#include <SFML/Graphics.hpp>
#include <string>

struct FPS {
    
    sf::Text text;

    FPS() {
        text.setCharacterSize(16);
        text.setPosition(10, 10);
        text.setFillColor(sf::Color::White);
    }

    float render(sf::RenderWindow& window, sf::Clock& clock) {
        float fps = 1 / clock.getElapsedTime().asSeconds();
        text.setString("FPS: " + std::to_string(int(fps)));
        window.draw(text);
        clock.restart();
        return fps;
    }
};
