#include <SFML/Graphics.hpp>
#include <string>

struct HUD {

    int font_size = 10;
    int font_offset = 10;

    std::string font_path = "../font/jetbrains.ttf";
    sf::Color font_color = sf::Color::White;
    
    sf::Font font;
    sf::Text text;

    void init() {
        font.loadFromFile(font_path);
        text.setFont(font);
        text.setCharacterSize(font_size);
        text.setPosition(font_offset, font_offset);
        text.setFillColor(font_color);
    }

    void render(
        sf::RenderWindow& window,
        sf::Clock& clock,
        bool keypress = true,
        int gravity_x = 0,
        int gravity_y = 1000,
        int n_particle = 500,
        int n_grid = 10
    ) {

        int fps = 1 / clock.getElapsedTime().asSeconds();
        clock.restart();

        if (keypress) {

            std::string a = std::to_string(fps);
            std::string b = std::to_string(n_particle);
            std::string c = std::to_string(n_grid);
            std::string d = std::to_string(gravity_x);
            std::string e = std::to_string(gravity_y);

            text.setString(
                "FPS: " + a + "\n" +
                "Particles: " + b + "\n" +
                "Grid Cells: " + c + "x" + c + "\n" +
                "Gravity: (" + d + ", " + e + ")"
            );

            window.draw(text);
        }
    }
};
