#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <fmt/core.h>

struct Button {

    bool previous = false;
    bool state = false;

    void toggle(bool press = false) {
        if (press && !previous) {
            previous = true;
            state = !state;
        } else if (press) {
            previous = true;
        } else {
            previous = false;
        }
    }
};

struct HUD {

    int font_size = 16;
    int font_offset = 10;
    bool previous = false;
    bool state = false;

    std::string font_path = "../font/jetbrains.ttf";
    sf::Color font_color = sf::Color::White;
    
    sf::Font font;
    sf::Text text;

    Button show_state;

    HUD() {
        font.loadFromFile(font_path);
        text.setFont(font);
        text.setCharacterSize(font_size);
        text.setPosition(font_offset, font_offset);
        text.setFillColor(font_color);
    }

    void render(
        sf::RenderWindow& window,
        sf::Clock& clock,
        int gravity_x = 0,
        int gravity_y = 0,
        int particle = 0,
        int particle_max = 0,
        int n_grid_x = 0,
        int n_grid_y = 0,
        bool press = false,
        bool center = false,
        bool explode = false
    ) {

        show_state.toggle(press);

        int fps = 1 / clock.getElapsedTime().asSeconds();
        clock.restart();

        if (show_state.state) {

            std::string starter = "";
            std::string gx_ch = std::to_string(gravity_x);
            std::string gy_ch = std::to_string(gravity_y);

            if (center || explode) {
                gx_ch = "Dynamic";
                gy_ch = "Dynamic";
            }

            std::string message = fmt::format(
                starter +
                "FPS: {}\n" +
                "Particles: {} of {}\n" +
                "Grid Cells: {} by {}\n" +
                "Gravity: ({}, {})\n\n" +
                "Gravity: W, A, S, D, Z, X, C\n" +
                "Add Particles: E\n" +
                "Remove Particles: Q\n" +
                "Toggle Velocity View: V",
                fps,
                particle,
                particle_max,
                n_grid_x,
                n_grid_y,
                gx_ch,
                gy_ch
            );

            text.setString(message);
            window.draw(text);
        }
    }
};
