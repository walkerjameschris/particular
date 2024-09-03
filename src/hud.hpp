#include <SFML/Graphics.hpp>
#include <string>

struct HUD {

    int font_size = 16;
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
        int gravity_y = 0,
        int current_particle = 0,
        int n_particle = 0,
        int n_grid = 0
    ) {

        int fps = 1 / clock.getElapsedTime().asSeconds();
        clock.restart();

        if (keypress) {

            std::string fps_ch = std::to_string(fps);
            std::string curr_chr = std::to_string(current_particle);
            std::string part_ch = std::to_string(n_particle);
            std::string grid_ch = std::to_string(n_grid);
            std::string gx_ch = std::to_string(gravity_x);
            std::string gy_ch = std::to_string(gravity_y);

            text.setString(
                "FPS: " + fps_ch + "\n" +
                "Particles: " + curr_chr + " of " + part_ch + "\n" +
                "Grid Cells: " + grid_ch + " by " + grid_ch + "\n" +
                "Gravity: (" + gx_ch + ", " + gy_ch + ")"
            );

            window.draw(text);
        }
    }
};
