#pragma once
#include <SFML/Graphics.hpp>
#include <string>

struct FPS {

    int font_size = 16;
    int font_offset = 10;

    std::string font_path = "../font/jetbrains.ttf";
    sf::Color font_color = sf::Color::White;
    
    sf::Font font;
    sf::Text text;

    FPS() {
        font.loadFromFile(font_path);
        text.setFont(font);
        text.setCharacterSize(font_size);
        text.setPosition(font_offset, font_offset);
        text.setFillColor(font_color);
    }

    float render(sf::RenderWindow& window, sf::Clock& clock) {
        float fps = 1 / clock.getElapsedTime().asSeconds();
        clock.restart();
        text.setString("FPS: " + std::to_string(int(fps)));
        window.draw(text);
        return fps;
    }
};
