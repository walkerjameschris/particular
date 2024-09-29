#include <SFML/Graphics.hpp>
#include "src/simulation.hpp"

int main() {

    const int frame_rate = 60;
    const int display_x = 1280;
    const int display_y = 720;

    sf::VideoMode window_scale(display_x, display_y);
    sf::RenderWindow window(window_scale, "Particular");
    sf::Clock clock;
    sf::Event event;

    Simulation simulation(display_x, display_y, frame_rate);

    window.setFramerateLimit(frame_rate);

    while (window.isOpen()) {

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        simulation.advance(
            window,
            sf::Keyboard::isKeyPressed(sf::Keyboard::W),
            sf::Keyboard::isKeyPressed(sf::Keyboard::A),
            sf::Keyboard::isKeyPressed(sf::Keyboard::D),
            sf::Keyboard::isKeyPressed(sf::Keyboard::Z),
            sf::Keyboard::isKeyPressed(sf::Keyboard::C),
            sf::Keyboard::isKeyPressed(sf::Keyboard::X)
        );
    }
}
