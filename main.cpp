#include <SFML/Graphics.hpp>
#include "src/simulation.hpp"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {

    const int frame_rate = 60;
    const int display_x = 1280;
    const int display_y = 720;
    
    Simulation simulation(display_x, display_y, frame_rate, argc, argv);
    
    sf::VideoMode window_scale(display_x, display_y);
    sf::RenderWindow window(window_scale, "Particular");
    sf::Clock clock;
    sf::Event event;

    window.setFramerateLimit(frame_rate);

    while (window.isOpen()) {

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        simulation.advance(
            window,
            clock,
            sf::Mouse::getPosition(window),
            sf::Keyboard::isKeyPressed(sf::Keyboard::R),
            sf::Keyboard::isKeyPressed(sf::Keyboard::W),
            sf::Keyboard::isKeyPressed(sf::Keyboard::A),
            sf::Keyboard::isKeyPressed(sf::Keyboard::D),
            sf::Keyboard::isKeyPressed(sf::Keyboard::Z),
            sf::Keyboard::isKeyPressed(sf::Keyboard::X),
            sf::Keyboard::isKeyPressed(sf::Keyboard::C),
            sf::Keyboard::isKeyPressed(sf::Keyboard::U)
        );
    }

    return 0;
}
